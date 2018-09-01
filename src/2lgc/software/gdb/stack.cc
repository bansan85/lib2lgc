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
#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <iostream>
#include <memory>
#include <utility>

/** \class llgc::software::gdb::Stack
 * \brief Store all informations about a stack.
 * \details Contains all backtraces of a dump.
 */

/** \class llgc::software::gdb::Stack::Iter
 * \brief Iterator for Stack.
 *
 *
 * \fn llgc::software::gdb::Stack::Iter::Iter(const SetStack& set_stack, size_t pos)
 * \brief The constructor.
 * \param[in] set_stack The set_stack contains the list of stack.
 * \param[in] pos The position of the current stack.
 */

/** \brief Dereference an iterator return the current stack.
 * \return Return the current backtrace.
 */
const llgc::software::gdb::Stack& llgc::software::gdb::Stack::Iter::operator*()
    const
{
  return data_.Get(pos_);
}

/** \brief Default constructor.
 * \param[in] filename Filename that contains all backtraces.
 */
llgc::software::gdb::Stack::Stack(std::string filename)
    : filename_(std::move(filename))
{
}

/** \brief Abstract factory that convert a line into a backtrace.
 * \param[in] line The line that contains the description of the current
 *            backtrace.
 * \return true if convertion is successfull.
 */
bool llgc::software::gdb::Stack::InterpretLine(const std::string& line)
{
  std::unique_ptr<Backtrace> bt = Backtrace::Factory(line);

  BUGCONT(std::cout, bt != nullptr, false);

  backtraces_.emplace_back(bt.release());

  return true;
}

/** \fn const std::string& llgc::software::gdb::Stack::GetFilename() const
 * \brief Get the name of the file from where the stack is stored.
 * \return The filename.
 *
 *
 * \fn size_t llgc::software::gdb::Stack::NumberOfBacktraces() const
 * \brief Get the number of backtraces of the stack.
 * \return The number of backtraces.
 *
 *
 * \fn const Backtrace& llgc::software::gdb::Stack::GetBacktraceFromTop(size_t i) const
 * \brief Get the backtrace from the top. Exception if out of the range.
 * \param[in] i The nth backtrace. 0 is the top of the stack.
 * \return A const pointer of the backtrace.
 *
 *
 * \fn const Backtrace& llgc::software::gdb::Stack::GetBacktraceFromBottom(size_t i) const
 * \brief Get the backtrace from the bottom. Exception if out of the range.
 * \param[in] i The nth backtrace. 0 is the bottom of the stack.
 * \return A const pointer of the backtrace.
 *
 *
 * \fn Backtrace::Iter llgc::software::gdb::Stack::begin() const
 * \brief Return of the top backtrace.
 * \return Begin of the const iterator.
 *
 *
 * \fn Backtrace::Iter llgc::software::gdb::Stack::end() const
 * \brief Return after of the last backtrace.
 * \return End of the const iterator.
 *
 *
 * \var llgc::software::gdb::Stack::filename_
 * \brief Storage of the filename.
 *
 *
 * \var llgc::software::gdb::Stack::backtraces_
 * \brief Vector that store all backtraces.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
