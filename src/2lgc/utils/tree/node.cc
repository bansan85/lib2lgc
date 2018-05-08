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
 * @file hash.cc
 * @brief Use function hash based on OpenSSL presence.
 */

#include <2lgc/error/show.h>
#include <2lgc/utils/tree/node.h>

template <typename T>
llgc::utils::tree::Node<T>::Node(size_t id, std::unique_ptr<T> data,
                                 Node<T>* parent)
    : id_(id), data_(std::move(data)), parent_(parent), children_()
{
}

template <typename T>
llgc::utils::tree::Node<T>* llgc::utils::tree::Node<T>::AddChild(
    std::unique_ptr<T> child, size_t id)
{
  if (FindNode(id, id_) != nullptr)
  {
    return nullptr;
  }
  children_.push_back(
      std::move(std::make_unique<Node<T>>(id, std::move(child), this)));
  return children_.back().get();
}

template <typename T>
bool llgc::utils::tree::Node<T>::FindPath(const Node<T>* previous, size_t end,
                                          std::vector<T*>* path) const
{
  BUGPARAM(path, path != nullptr, false);
  // In case the start is the end.
  if (id_ == end)
  {
    path->clear();
    path->push_back(data_.get());
    return true;
  }

  // Start by children
  for (size_t i = 0; i < children_.size(); i++)
  {
    // Do not go back.
    if (previous != nullptr && children_[i].get()->id_ == previous->id_)
    {
      continue;
    }
    if (children_[i]->FindPath(this, end, path))
    {
      path->push_back(data_.get());
      return true;
    }
  }

  // Do not go back.
  if (parent_ == previous)
  {
    return false;
  }
  if (parent_ != nullptr && parent_->FindPath(this, end, path))
  {
    path->push_back(data_.get());
    return true;
  }
  return false;
}

template <typename T>
llgc::utils::tree::Node<T>* llgc::utils::tree::Node<T>::FindNode(
    size_t id, size_t previous)
{
  // In case the start is the end.
  if (id_ == id)
  {
    return this;
  }

  // Start by children
  for (size_t i = 0; i < children_.size(); i++)
  {
    // Do not go back.
    if (children_[i].get()->id_ == previous)
    {
      continue;
    }
    Node<T>* retval = children_[i]->FindNode(id, id_);
    if (retval != nullptr)
    {
      return retval;
    }
  }

  // Do not go back.
  if (parent_ != nullptr && parent_->id_ == previous)
  {
    return nullptr;
  }
  if (parent_ != nullptr)
  {
    Node<T>* retval = parent_->FindNode(id, id_);
    if (retval != nullptr)
    {
      return retval;
    }
  }

  return nullptr;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
