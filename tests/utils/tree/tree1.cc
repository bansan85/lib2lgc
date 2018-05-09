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

#include <2lgc/utils/tree/node.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <2lgc/utils/tree/node.cc>

/**
 * @brief Test class.
 */
class T
{
 public:
  /**
   * @brief Constructor.
   *
   * @param[in] nom Name of the node.
   */
  explicit T(std::string nom) : nom_(std::move(nom)) {}

  /**
   * @brief Name.
   */
  std::string nom_;
};

template class llgc::utils::tree::Node<T>;

int main(int /* argc */, char* /* argv */ [])  // NS
{
  std::unique_ptr<T> t0 = std::make_unique<T>("0");
  std::unique_ptr<T> t1 = std::make_unique<T>("1");
  std::unique_ptr<T> t2 = std::make_unique<T>("2");
  std::unique_ptr<T> t3 = std::make_unique<T>("3");
  std::unique_ptr<T> t4 = std::make_unique<T>("4");
  std::unique_ptr<T> t5 = std::make_unique<T>("5");

  //  0
  // / \
  // 1  2
  // |\
  // 3 5
  // |
  // 4

  llgc::utils::tree::Node<T> root(0, std::move(t0), nullptr);
  assert(root.AddChild(std::move(t1), 1) != nullptr);
  assert(root.AddChild(std::move(t1), 0) == nullptr);
  assert(root.AddChild(std::move(t2), 2) != nullptr);
  assert(root.AddChild(std::move(t2), 1) == nullptr);
  llgc::utils::tree::Node<T>* n1 = root.FindNode(1);
  assert(n1 != nullptr);
  assert(n1->AddChild(std::move(t3), 3) != nullptr);
  assert(n1->AddChild(std::move(t5), 5) != nullptr);
  llgc::utils::tree::Node<T>* n3 = root.FindNode(3);
  assert(n3 != nullptr);
  assert(n3->AddChild(std::move(t4), 4) != nullptr);
  assert(n3->FindNode(2) != nullptr);

  std::deque<T*> path;
  assert(n3->FindPath(2, &path));
  assert(path.size() == 4);
  std::cout << path[0]->nom_ << std::endl;
  assert(path[0]->nom_ == "3");
  assert(path[1]->nom_ == "1");
  assert(path[2]->nom_ == "0");
  assert(path[3]->nom_ == "2");

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
