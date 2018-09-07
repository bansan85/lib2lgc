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

// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep
#include <2lgc/error/show.h>
#include <2lgc/filesystem/files.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
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
#include <future>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#ifdef TEMPLATE_CLASS
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/singleton.h>

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
#endif

/** \class llgc::software::gdb::SetStack
 * \brief Store and sort all stacks.
 * \details Criterea of sort must be defined on constructor.
 */

/** \brief Default constructor.
 * \param[in] with_source_only Ignore backtrace where source file is unknown.
 * \param[in] top_frame Number of frames from the top that must be identical so
 *            two stacks are the same. Internally, this number can not exceeded
 *            the number of frames.
 * \param[in] bottom_frame Number of frames from the bottom that must be
 *            identical so two stacks are the same. Internally, this number can
 *            not exceeded the number of frames.
 * \param[in] print_one_by_group Keep only one identical backtrace.
 */
llgc::software::gdb::SetStack::SetStack(bool with_source_only, size_t top_frame,
                                        size_t bottom_frame,
                                        bool print_one_by_group)
    : stack_(LocalCompare(with_source_only, top_frame, bottom_frame)),
      print_one_by_group_(print_one_by_group)
{
}

/** \fn llgc::software::gdb::SetStack::~SetStack()
 * \brief Default destructor.
 *
 *
 * \fn llgc::software::gdb::SetStack::SetStack(SetStack&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::software::gdb::SetStack::SetStack(SetStack const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn SetStack& llgc::software::gdb::SetStack::operator=(SetStack&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn SetStack& llgc::software::gdb::SetStack::operator=(SetStack const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 */

/** \brief Add a new stack. The file must contains only the full backtrace from
 *         GDB.
 * \param[in] filename The file to add.
 * \return True if the file is a valid backtrace.
 */
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
    if (!Function::IsValidVariableLine(line) && stack_gdb->InterpretLine(line))
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

/** \brief Add new stacks based on files from a folder. All files must contains
 *         only the full backtrace from GDB.
 * \param[in] folder The folder where all *.btfull files are.
 * \param[in] nthread The number of threads if parallel is allowed.
 * \param[in] regex Regex that match file to read. If empty, all files will be
 *            read.
 * \return true if no problem.
 */
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

/** \brief Add new stacks based on files from a list. All files must contains
 *         only the full backtrace from GDB.
 * \param[in] list The folder where all *.btfull files are.
 * \param[in] nthread The number of threads if parallel is allowed.
 * \return true if no problem.
 */
bool llgc::software::gdb::SetStack::AddList(const std::string& list,
                                            unsigned int nthread)
{
  std::vector<std::string> all_files;
  std::string line;
  std::ifstream f(list);
  BUGUSER(std::cout, f.is_open(), false, "Failed to open " + list + ".\n");

  while (std::getline(f, line))
  {
    all_files.push_back(line);
  }

  BUGCONT(std::cout, ParallelAdd(all_files, nthread), false);
  return true;
}

/** \brief Get the number of stack.
 * \return The number of stack.
 */
size_t llgc::software::gdb::SetStack::Count() const
{
  std::lock_guard<std::mutex> lck(mutex_stack_);
  return stack_.size();
}

/// \brief Show all stacks grouped by condition passed with the constructor.
void llgc::software::gdb::SetStack::Print() const
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

/** \brief Get the nth stack of the set.
 * \param[in] i the nth stack. First is at 0.
 * \return A reference to the stack. The ith stack must exists.
 */
const llgc::software::gdb::Stack& llgc::software::gdb::SetStack::Get(
    size_t i) const
{
  auto it = stack_.begin();
  std::advance(it, i);
  return **it;
}

/** \fn Stack::Iter llgc::software::gdb::SetStack::begin() const
 * \brief Return of the top stack.
 * \return Begin of the const iterator.
 *
 *
 * \fn Stack::Iter llgc::software::gdb::SetStack::end() const
 * \brief Return after of the last stack.
 * \return End of the const iterator.
 */

/** \var llgc::software::gdb::SetStack::server_
 * \brief Server publisher.
 */

/** \struct llgc::software::gdb::SetStack::LocalCompare
 * \brief A local class that compare to stack and says if two stack looks to be
 *        the same.
 *
 *
 * \typedef llgc::software::gdb::SetStack::LocalCompare::FunctionGetBacktrace
 * \brief Prototype to get the backtrace from the top or from the bottom.
 * \param[in] i The n-th backtrace.
 * \return The backtrace. Throw an exception if out of boundary.
 */

/** \brief Constructor with parameter of comparaison.
 * \param[in] with_source_only Ignore backtrace where source file is unknown.
 * \param[in] top_frame Number of frames from the top that must be identical so
 *            two stacks are the same. Internally, this number can not exceeded
 *            the number of frames.
 * \param[in] bottom_frame Number of frames from the bottom that must be
 *            identical so two stacks are the same. Internally, this number can
 *            not exceeded the number of frames.
 */
llgc::software::gdb::SetStack::LocalCompare::LocalCompare(bool with_source_only,
                                                          size_t top_frame,
                                                          size_t bottom_frame)
    : with_source_only_(with_source_only),
      top_frame_(top_frame),
      bottom_frame_(bottom_frame)
{
}

/** \brief Helper to operator(). Compare two stacks from the top or from the
 *         bottom.
 * \param[in] nb_max_frames Number maximum of backtraces to compare.
 * \param[in] get_backtraces Compare from the top or from the bottom.
 * \param[in] i The first stack.
 * \param[in] j The second stack.
 * \return -1 if i < j, 0 if i == j and -1 if i > j.
 */
ssize_t llgc::software::gdb::SetStack::LocalCompare::CompareFrom(
    size_t nb_max_frames, FunctionGetBacktrace get_backtraces,
    const std::unique_ptr<Stack>& i, const std::unique_ptr<Stack>& j) const
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
    int compare = bti.GetFile().compare(btj.GetFile());

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

/** \brief Function that compare two stacks.
 * \param[in] i The first stack.
 * \param[in] j The second stack.
 * \return If i < j based on argument from constructor.
 */
bool llgc::software::gdb::SetStack::LocalCompare::operator()(
    const std::unique_ptr<Stack>& i, const std::unique_ptr<Stack>& j) const
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

/** \var llgc::software::gdb::SetStack::LocalCompare::with_source_only_
 * \brief Ignore backtrace where source file is unknown.
 *
 *
 * \var llgc::software::gdb::SetStack::LocalCompare::top_frame_
 * \brief Number of frames from the top that must be identical so two stacks
 *        are the same. Internally, this number can not exceeded the number of
 *        frames.
 *
 *
 * \var llgc::software::gdb::SetStack::LocalCompare::bottom_frame_
 * \brief Number of frames from the bottom that must be identical so two
 *        stacks are the same. Internally, this number can not exceeded the
 *        number of frames.
 */

/** \brief Read in parallel a list of files that contains gdb backtraces.
 * \param[in] all_files The list of files that contains gdb backtraces.
 * \param[in] nthread The number of threads with a maximum of
 *            std::thread::hardware_concurrency().
 * \return true if no problem.
 */
bool llgc::software::gdb::SetStack::ParallelAdd(
    const std::vector<std::string>& all_files, unsigned int nthread)
{
  bool retval = true;
  unsigned int nthreads =
      std::min(std::min(nthread, std::thread::hardware_concurrency()),
               static_cast<unsigned int>(all_files.size()));
  std::vector<std::future<bool>> threads(nthreads);
  for (size_t t = 0; t < nthreads; t++)
  {
    threads[t] =
        std::async(std::launch::async, [&all_files, this, nthreads, t]() {
          bool retval2 = true;
          for (size_t i = t; i < all_files.size(); i += nthreads)
          {
            retval2 &= Add(all_files[i]);
          }
          return retval2;
        });
  }
  for (std::future<bool>& t : threads)
  {
    retval &= t.get();
  }

  return retval;
}

/** \brief Set a message throw the server to tell that this file is invalid.
 * \param[in] filename The filename that failed to be read.
 * \return true if no problem.
 */
bool llgc::software::gdb::SetStack::TellError(const std::string& filename)
{
  llgc::protobuf::software::Gdb messages;
  auto message = messages.add_msg();
  auto add_stack_failed =
      std::make_unique<llgc::protobuf::software::Gdb::Msg::AddStackFailed>();
  std::string* filename_gdb = add_stack_failed->add_file();
  filename_gdb->assign(filename);
  message->set_allocated_add_stack_failed(add_stack_failed.release());
  BUGCONT(std::cout, Forward(messages), false);
  return true;
}

/** \brief Send the message to all subscribers.
 * \param[in] message The message to send.
 * \return true if no problem.
 */
bool llgc::software::gdb::SetStack::Forward(
    const llgc::protobuf::software::Gdb& message)
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

/** \var llgc::software::gdb::SetStack::stack_
 * \brief Storage of all stacks sorted with parameter given by the constructor.
 *
 *
 * \var llgc::software::gdb::SetStack::mutex_stack_
 * \brief A internal mutex to use stack_ thread-safe.
 *
 *
 * \var llgc::software::gdb::SetStack::print_one_by_group_
 * \brief Add the file only if no equivalent already added.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
