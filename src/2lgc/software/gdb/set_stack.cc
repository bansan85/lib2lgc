/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <2lgc/filesystem/files.h>
#include <2lgc/pattern/publisher/publisher_remote.h>  // IWYU pragma: keep
#include <2lgc/pattern/singleton/singleton_local.h>
#include <2lgc/poco/gdb.pb.h>
#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/function.h>
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
#include <limits>
#include <memory>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#include <2lgc/pattern/singleton/singleton_local.cc>

template class llgc::pattern::singleton::Local<
    llgc::pattern::publisher::PublisherRemote<msg::software::Gdbs>>;

llgc::software::gdb::SetStack::SetStack(bool with_source_only, size_t top_frame,
                                        size_t bottom_frame)
    : stack_(Local(with_source_only, top_frame, bottom_frame))
{
}

llgc::software::gdb::SetStack::Local::Local(bool with_source_only,
                                            size_t top_frame,
                                            size_t bottom_frame)
    : with_source_only_(with_source_only),
      top_frame_(top_frame),
      bottom_frame_(bottom_frame)
{
}

ssize_t llgc::software::gdb::SetStack::Local::CompareFrom(
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
      while (ii < icount && !(*i.*get_backtraces)(ii)->HasSource())
      {
        ii++;
      }
      while (jj < jcount && !(*j.*get_backtraces)(jj)->HasSource())
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

    const Backtrace* bti = (*i.*get_backtraces)(ii);
    const Backtrace* btj = (*j.*get_backtraces)(jj);
    int compare = bti->GetFile().compare(btj->GetFile());  // NS

    if (compare < 0)
    {
      return -1;
    }
    if (compare > 0)
    {
      return 1;
    }

    // First compare the line.
    if (bti->GetLine() < btj->GetLine())
    {
      return -1;
    }
    if (bti->GetLine() > btj->GetLine())
    {
      return 1;
    }
    // If both lines are unknown, compare the name of the function.
    if (bti->GetLine() == std::numeric_limits<size_t>::max())
    {
      if (bti->GetName().compare(btj->GetName()) < 0)
      {
        return -1;
      }
      if (bti->GetName().compare(btj->GetName()) > 0)
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

// NOLINTNEXTLINE(fuchsia-overloaded-operator)
bool llgc::software::gdb::SetStack::Local::operator()(
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

void llgc::software::gdb::SetStack::TellError(const std::string& filename)
{
  msg::software::Gdbs messages_gdb = msg::software::Gdbs();
  msg::software::Gdb* message_gdb = messages_gdb.add_action();
  std::unique_ptr<msg::software::Gdb::AddStack> add_stack =
      std::make_unique<msg::software::Gdb::AddStack>();
  std::string* filename_gdb = add_stack->add_file();
  filename_gdb->assign(filename);
  message_gdb->set_allocated_add_stack(add_stack.release());
  std::shared_ptr<std::string> add_stack_in_string =
      std::make_shared<std::string>();
  messages_gdb.SerializeToString(add_stack_in_string.get());
  Forward(add_stack_in_string);
}

bool llgc::software::gdb::SetStack::Add(const std::string& filename,
                                        bool print_one_by_group)
{
  std::ifstream file(filename);

  if (!file.is_open())
  {
    TellError(filename);
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
      if (stack_gdb->GetBacktraceFromBottom(0)->GetIndex() + 1 !=
          stack_gdb->NumberOfBacktraces())
      {
        TellError(filename);
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
      TellError(filename);
      return false;
    }
  }

  std::lock_guard<std::mutex> lck(mutex_stack_);
  if (!print_one_by_group || stack_.find(stack_gdb) == stack_.end())
  {
    stack_.insert(std::move(stack_gdb));
  }

  return true;
}

bool llgc::software::gdb::SetStack::ParallelAdd(
    const std::vector<std::string>& all_files, unsigned int nthread,
    bool print_one_by_group)
{
  bool retval = true;
  const unsigned int nthreads =  // NS
      std::min(nthread, std::thread::hardware_concurrency());
  std::vector<std::future<bool>> threads(nthreads);
  for (size_t t = 0; t < nthreads; t++)
  {
    threads[t] = std::async(
        std::launch::async,
        std::bind(
            [&all_files, this, nthreads,
             print_one_by_group](const size_t i_start) {
              bool retval2 = true;
              for (size_t i = i_start; i < all_files.size(); i += nthreads)
              {
                retval2 &= Add(all_files[i], print_one_by_group);
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
                                                 const std::string& regex,
                                                 bool print_one_by_group)
{
  std::vector<std::string> all_files;
  if (!llgc::filesystem::Files::SearchRecursiveFiles(folder, regex, &all_files))
  {
    return false;
  }

  return ParallelAdd(all_files, nthread, print_one_by_group);
}

bool llgc::software::gdb::SetStack::AddList(const std::string& list,
                                            unsigned int nthread,
                                            bool print_one_by_group)
{
  std::vector<std::string> all_files;
  std::string line;
  std::ifstream f(list);
  if (!f.is_open())
  {
    return false;
  }

  while (std::getline(f, line))
  {
    all_files.push_back(line);
  }

  return ParallelAdd(all_files, nthread, print_one_by_group);
}

void llgc::software::gdb::SetStack::Print()
{
  std::multiset<std::unique_ptr<Stack>, Local>::const_iterator m_it, s_it;

  size_t num = 0;
  for (m_it = stack_.begin(); m_it != stack_.end(); m_it = s_it)
  {
    std::pair<std::multiset<std::unique_ptr<Stack>, Local>::const_iterator,
              std::multiset<std::unique_ptr<Stack>, Local>::const_iterator>
        keyRange = stack_.equal_range(*m_it);

    std::cout << "Groupe " << num << std::endl;

    for (s_it = keyRange.first; s_it != keyRange.second; ++s_it)
    {
      std::cout << "  " << (*s_it)->GetFilename() << std::endl;
    }
    num++;
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
