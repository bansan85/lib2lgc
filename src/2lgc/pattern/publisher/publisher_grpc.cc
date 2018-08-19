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
#include <2lgc/pattern/publisher/publisher_grpc.h>
#include <2lgc/pattern/publisher/publisher_ip.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <utility>

template <typename T, typename S>
llgc::pattern::publisher::PublisherGrpc<T, S>::PublisherGrpc(uint16_t port)
    : llgc::pattern::publisher::PublisherIp<T>(port), service_(this)
{
}

template <typename T, typename S>
llgc::pattern::publisher::PublisherGrpc<T, S>::~PublisherGrpc()
{
  Stop();
}

template <typename T, typename S>
bool llgc::pattern::publisher::PublisherGrpc<T, S>::Listen()
{
  std::stringstream ss;

  ss << "0.0.0.0:" << this->port_;

  grpc::ServerBuilder builder_;

  builder_.AddListeningPort(ss.str(), grpc::InsecureServerCredentials());
  builder_.RegisterService(&service_);

  server_ = std::move(builder_.BuildAndStart());

  BUGCRIT(std::cout, server_ != nullptr, false,
          "Fail to start server at port " << this->port_ << " for 0.0.0.0.\n");

  return true;
}

template <typename T, typename S>
bool llgc::pattern::publisher::PublisherGrpc<T, S>::Wait()
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

template <typename T, typename S>
void llgc::pattern::publisher::PublisherGrpc<T, S>::Stop()
{
  if (server_ != nullptr)
  {
    server_->Shutdown();
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
