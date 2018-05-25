/*
 *    Copyright 2018 LE GARREC Vincent
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <2lgc/error/show.h>
#include <2lgc/filesystem/files.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/singleton.h>
#include <2lgc/poco/software_gdb.pb.h>
#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/function.h>
#include <2lgc/software/gdb/gdb_server.h>
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <cxxabi.h>
#include <algorithm>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/singleton.cc>

template class llgc::pattern::publisher::PublisherInterface<
    llgc::protobuf::software::Gdb,
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<
        llgc::protobuf::software::Gdb>>>;
template class llgc::pattern::Singleton<
    llgc::pattern::publisher::PublisherDirect<llgc::protobuf::software::Gdb>>;
template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::software::Gdb>;

llgc::software::gdb::SetStack::SetStack(bool with_source_only, size_t top_frame,
                                        size_t bottom_frame,
                                        bool print_one_by_group)
    : stack_(LocalCompare(with_source_only, top_frame, bottom_frame)),
      print_one_by_group_(print_one_by_group)
{
}

llgc::software::gdb::SetStack::~SetStack() = default;

llgc::software::gdb::SetStack::LocalCompare::LocalCompare(bool with_source_only,
                                                          size_t top_frame,
                                                          size_t bottom_frame)
    : with_source_only_(with_source_only),
      top_frame_(top_frame),
      bottom_frame_(bottom_frame)
{
}

ssize_t llgc::software::gdb::SetStack::LocalCompare::CompareFrom(
    const size_t nb_max_frames, FunctionGetBacktrace get_backtraces,
    const std::unique_ptr<Stack>& i, const std::unique_ptr<Stack>& j)
{
  uint32_t ii = 0, jj = 0;

  // Compare from top
  const size_t icount = i->NumberOfBacktraces();
  const size_t jcount = j->NumberOfBacktraces();
  size_t nb = 0;
  while (ii < icount && jj < jcount && nb < nb_max_frames)
  {
    if (with_source_only_)
    {
      while (ii < icount && !(*i.*get_backtraces)(ii).HasSource())
      {
        ii++;
      }
      while (jj < jcount && !(*j.*get_backtraces)(jj).HasSource())
      {
        jj++;
      }
    }

    // Same from top.
    if (ii == icount && jj == jcount)
    {
      return 0;
    }
    // i stack too short.
    if (ii == icount)
    {
      return -1;
    }
    // j stack too short.
    if (jj == jcount)
    {
      return 1;
    }

    const Backtrace& bti = (*i.*get_backtraces)(ii);
    const Backtrace& btj = (*j.*get_backtraces)(jj);
    int compare = bti.GetFile().compare(btj.GetFile());  // NS

    if (compare < 0)
    {
      return -1;
    }
    if (compare > 0)
    {
      return 1;
    }

    // First compare the line.
    if (bti.GetLine() < btj.GetLine())
    {
      return -1;
    }
    if (bti.GetLine() > btj.GetLine())
    {
      return 1;
    }
    // If both lines are unknown, compare the name of the function.
    if (bti.GetLine() == std::numeric_limits<size_t>::max())
    {
      if (bti.GetName().compare(btj.GetName()) < 0)
      {
        return -1;
      }
      if (bti.GetName().compare(btj.GetName()) > 0)
      {
        return 1;
      }
    }

    nb++;
    ii++;
    jj++;
  }

  // Same from top.
  if (ii == icount && jj == jcount)
  {
    return 0;
  }
  // i stack too short.
  if (ii == icount)
  {
    return -1;
  }
  // j stack too short.
  if (jj == jcount)
  {
    return 1;
  }

  return 0;
}

bool llgc::software::gdb::SetStack::LocalCompare::operator()(
    const std::unique_ptr<Stack>& i, const std::unique_ptr<Stack>& j)
{
  // If it's the same file.
  if (i->GetFilename() == j->GetFilename())
  {
    return false;
  }

  ssize_t val = CompareFrom(top_frame_, &Stack::GetBacktraceFromTop, i, j);
  if (val < 0)
  {
    return true;
  }
  if (val > 0)
  {
    return false;
  }

  val = CompareFrom(bottom_frame_, &Stack::GetBacktraceFromBottom, i, j);
  if (val < 0)
  {
    return true;
  }
  if (val > 0)
  {
    return false;
  }

  return false;
}

bool llgc::software::gdb::SetStack::TellError(const std::string& filename)
{
  llgc::protobuf::software::Gdb messages;
  auto message = messages.add_msg();
  auto add_stack_failed =
      std::make_unique<llgc::protobuf::software::Gdb::Msg::AddStackFailed>();
  std::string* filename_gdb = add_stack_failed->add_file();
  filename_gdb->assign(filename);
  message->set_allocated_add_stack_failed(add_stack_failed.release());
  std::string add_stack_in_string;
  BUGLIB(std::cout, messages.SerializeToString(&add_stack_in_string), false,
         "protobuf");
  BUGCONT(std::cout, Forward(add_stack_in_string), false);
  return true;
}

bool llgc::software::gdb::SetStack::Add(const std::string& filename)
{
  std::ifstream file(filename);

  if (!file.is_open())
  {
    BUGCONT(std::cout, TellError(filename), false);
    return false;
  }

  std::unique_ptr<Stack> stack_gdb = std::make_unique<Stack>(filename);
  std::string line;

  // Wrong line is allow after a valid variable line and after an invalid line.
  // A wrong line is possible if the variable line is wrapped.
  bool allow_wrong_line = false;

  while (getline(file, line))
  {
    if (stack_gdb->InterpretLine(line))
    {
      if (stack_gdb->GetBacktraceFromBottom(0).GetIndex() + 1 !=
          stack_gdb->NumberOfBacktraces())
      {
        BUGCONT(std::cout, TellError(filename), false);
        return false;
      }
      allow_wrong_line = false;
    }
    else if (Function::IsValidVariableLine(line) || allow_wrong_line)
    {
      allow_wrong_line = Function::IsVariableLineWrappable(line);
    }
    else if (!allow_wrong_line)
    {
      BUGCONT(std::cout, TellError(filename), false);
      return false;
    }
  }

  std::lock_guard<std::mutex> lck(mutex_stack_);
  if (!print_one_by_group_ || stack_.find(stack_gdb) == stack_.end())
  {
    stack_.insert(std::move(stack_gdb));
  }

  return true;
}

bool llgc::software::gdb::SetStack::ParallelAdd(
    const std::vector<std::string>& all_files, unsigned int nthread)
{
  bool retval = true;
  const unsigned int nthreads =  // NS
      std::min(std::min(nthread, std::thread::hardware_concurrency()),
               static_cast<unsigned int>(all_files.size()));
  std::vector<std::future<bool>> threads(nthreads);
  for (size_t t = 0; t < nthreads; t++)
  {
    threads[t] = std::async(
        std::launch::async,
        std::bind(
            [&all_files, this, nthreads](const size_t i_start) {
              bool retval2 = true;
              for (size_t i = i_start; i < all_files.size(); i += nthreads)
              {
                retval2 &= Add(all_files[i]);
              }
              return retval2;
            },  // NS
            t));
  }
  for (std::future<bool>& t : threads)
  {
    retval &= t.get();
  }

  return retval;
}

bool llgc::software::gdb::SetStack::AddRecursive(const std::string& folder,
                                                 unsigned int nthread,
                                                 const std::string& regex)
{
  std::vector<std::string> all_files;
  if (!llgc::filesystem::Files::SearchRecursiveFiles(folder, regex, &all_files))
  {
    return false;
  }

  BUGCONT(std::cout, ParallelAdd(all_files, nthread), false);
  return true;
}

bool llgc::software::gdb::SetStack::AddList(const std::string& list,
                                            unsigned int nthread)
{
  std::vector<std::string> all_files;
  std::string line;
  std::ifstream f(list);
  BUGUSER(std::cout, f.is_open(), false, "Failed to open " << list << ".\n");

  while (std::getline(f, line))
  {
    all_files.push_back(line);
  }

  BUGCONT(std::cout, ParallelAdd(all_files, nthread), false);
  return true;
}

void llgc::software::gdb::SetStack::Print()
{
  std::multiset<std::unique_ptr<Stack>, LocalCompare>::const_iterator m_it,
      s_it;

  size_t num = 0;
  for (m_it = stack_.begin(); m_it != stack_.end(); m_it = s_it)
  {
    std::pair<
        std::multiset<std::unique_ptr<Stack>, LocalCompare>::const_iterator,
        std::multiset<std::unique_ptr<Stack>, LocalCompare>::const_iterator>
        keyRange = stack_.equal_range(*m_it);

    std::cout << "Groupe " << num << std::endl;

    for (s_it = keyRange.first; s_it != keyRange.second; ++s_it)
    {
      std::cout << "  " << (*s_it)->GetFilename() << std::endl;
    }
    num++;
  }
}

size_t llgc::software::gdb::SetStack::Count() const
{
  std::lock_guard<std::mutex> lck(mutex_stack_);
  return stack_.size();
}

const llgc::software::gdb::Stack& llgc::software::gdb::SetStack::Get(
    size_t i) const
{
  auto it = stack_.begin();
  std::advance(it, i);
  return **it;
}

bool llgc::software::gdb::SetStack::Forward(const std::string& message)
{
  // Check if instance.
  if (server_.IsInstance())
  {
    // If the instance if freed, GetInstance will create it.
    auto singleton_ = server_.GetInstance();
    BUGCONT(std::cout, singleton_->Forward(message), false);
  }
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
