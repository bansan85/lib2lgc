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

#include <2lgc/software/gdb/backtrace.h>
#include <2lgc/software/gdb/set_stack.h>
#include <2lgc/software/gdb/stack.h>
#include <memory>
#include <utility>

llgc::software::gdb::Stack::Stack(std::string filename)
    : filename_(std::move(filename))
{
}

bool llgc::software::gdb::Stack::InterpretLine(const std::string& line)
{
  std::unique_ptr<Backtrace> bt = Backtrace::Factory(line);

  if (!bt)
  {
    return false;
  }

  backtraces_.emplace_back(bt.release());

  return true;
}

const llgc::software::gdb::Stack& llgc::software::gdb::Stack::Iter::operator*()
    const
{
  return data_.Get(pos_);
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
