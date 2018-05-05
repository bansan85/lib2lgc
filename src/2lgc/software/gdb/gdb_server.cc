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
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/publisher.h>
#include <2lgc/pattern/publisher/publisher_direct.h>
#include <2lgc/pattern/singleton/singleton.h>
#include <2lgc/software/gdb/gdb_server.h>
#include <memory>
#include <string>

#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/publisher.cc>
#include <2lgc/pattern/singleton/singleton.cc>

template class llgc::pattern::publisher::ConnectorInterface<
    msg::software::Gdbs>;
template class llgc::pattern::publisher::Publisher<
    msg::software::Gdbs,
    std::weak_ptr<
        llgc::pattern::publisher::ConnectorInterface<msg::software::Gdbs>>>;
template class llgc::pattern::singleton::Local<llgc::pattern::publisher::PublisherDirect<msg::software::Gdbs>>;

bool llgc::software::gdb::GdbServer::Forward(const std::string& message)
{
  // Check if instance.
  if (IsInstance())
  {
    // If the instance if freed, GetInstance will create it.
    auto singleton_ = GetInstance();
    BUGCONT(singleton_->Forward(message), false);
  }

  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
