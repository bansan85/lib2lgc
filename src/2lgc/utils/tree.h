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

#ifndef UTILS_TREE_H_
#define UTILS_TREE_H_

#include <2lgc/compat.h>
#include <cstddef>
#include <deque>
#include <memory>
#include <vector>

namespace llgc::utils
{
template <typename T>
class Tree
{
 public:
  Tree(size_t id, std::unique_ptr<T> data, Tree<T>* parent);

  Tree<T>* AddChild(std::unique_ptr<T> child) CHK;
  Tree<T>* AddChild(std::unique_ptr<T> child, size_t id) CHK;

  bool FindPath(size_t end, std::deque<T*>* path) const CHK;
  bool FindPath(size_t begin, size_t end, std::deque<T*>* path) const CHK;

  const Tree<T>* FindNode(size_t id) const;
  Tree<T>* FindNode(size_t id);

  size_t FindMaxId() const;
  size_t GetId() const { return id_; }
  T* GetData() { return data_.get(); }

 private:
  bool FindPath(const Tree<T>* previous, size_t end,
                std::deque<T*>* path) const CHK;

  const Tree<T>* FindNode(size_t id, size_t previous) const;
  Tree<T>* FindNode(size_t id, size_t previous);
  size_t FindMaxId(size_t max_id, size_t previous) const;

  size_t id_;
  std::unique_ptr<T> data_;
  Tree<T>* parent_;
  std::vector<std::unique_ptr<Tree<T>>> children_;
};

}  // namespace llgc::utils

#ifndef TEMPLATE_CLASS
#include <2lgc/utils/tree.cc>
#endif

#endif  // UTILS_TREE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
