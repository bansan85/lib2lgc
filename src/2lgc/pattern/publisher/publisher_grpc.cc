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

#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_grpc.h>
#include <2lgc/error/show.h>
#include <sstream>
#include <iostream>

template <typename T>
llgc::pattern::publisher::PublisherGrpc<T>::PublisherGrpc(uint16_t port, std::unique_ptr<grpc::Service> service) : llgc::pattern::publisher::PublisherIp<T>(port), service_(std::move(service))
{
}

template <typename T>
llgc::pattern::publisher::PublisherGrpc<T>::~PublisherGrpc()
{
  Stop();
  JoinWait();
}

template <typename T>
void llgc::pattern::publisher::PublisherGrpc<T>::JoinWait()
{
  PublisherIp<T>::JoinWait();
}

template <typename T>
bool llgc::pattern::publisher::PublisherGrpc<T>::Listen()
{
  std::stringstream ss;

  ss << "0.0.0.0:" << this->port_;

  builder_.AddListeningPort(ss.str(), grpc::InsecureServerCredentials());
  builder_.RegisterService(service_.get());

  server_ = std::move(builder_.BuildAndStart());

  BUGCRIT(std::cout, server_ != nullptr, false, "Fail to start server at port " << this->port_ << " for 127.0.0.1.\n");

  return true;
}

template <typename T>
bool llgc::pattern::publisher::PublisherGrpc<T>::Wait()
{
  std::thread t([this]() {
    if (server_ != nullptr)
    {
      server_->Wait();
    }
  });

  this->thread_wait_ = std::move(t);

  return true;
}

template <typename T>
void llgc::pattern::publisher::PublisherGrpc<T>::Stop()
{
  if (server_ != nullptr)
  {
    server_->Shutdown();
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
