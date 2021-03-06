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

/**
 * @file gdb.h
 * @brief Class that run gdb for various purpose.
 */

#ifndef SOFTWARE_GDB_GDB_SERVER_H_
#define SOFTWARE_GDB_GDB_SERVER_H_

#include <2lgc/compat.h>
#include <2lgc/pattern/singleton.h>

namespace llgc::pattern::publisher
{
template <typename T>
class PublisherDirect;
}

namespace llgc::protobuf::software
{
class Gdb;
}

namespace llgc::software::gdb
{
class GdbServer
    : public llgc::pattern::Singleton<llgc::pattern::publisher::PublisherDirect<
          llgc::protobuf::software::Gdb>>
{
 public:
  bool Forward(const llgc::protobuf::software::Gdb &message) CHK;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_GDB_SERVER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
