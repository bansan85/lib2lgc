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
#include <2lgc/utils/tree.h>
#include <algorithm>  // IWYU pragma: keep
#include <cstddef>
#include <iostream>
#include <string>

/** \namespace llgc::utils
 * \brief Common namespace for miscellaneous functionality.
 *
 *
 * \class llgc::utils::Tree
 * \brief Node of the bidirectional tree. Composite pattern.
 * \tparam T The type of the elements.
 */

/** \brief Constructor of the node.
 * \param[in] id The id of the new node.
 * \param[in] data The element.
 * \param[in] parent Parent of the node. nullptr if root of the tree.
 */
template <typename T>
llgc::utils::Tree<T>::Tree(size_t id, std::unique_ptr<T> data, Tree<T>* parent)
    : id_(id), data_(std::move(data)), parent_(parent), children_()
{
}

/** \brief Add a child to the current node.
 * \param[in] child The new child.
 * \return A pointer to the new child with the new id. nullptr if AddChild
 *         fails.
 */
template <typename T>
llgc::utils::Tree<T>* llgc::utils::Tree<T>::AddChild(std::unique_ptr<T> child)
{
  size_t max_id = FindMaxId(id_, id_);
  children_.emplace_back(
      std::make_unique<Tree<T>>(max_id + 1, std::move(child), this));
  return children_.back().get();
}

/** \brief Add a child to the specific node.
 * \param[in] child The new child.
 * \param[in] id The id of the parent node.
 * \return A pointer to the new child with the new id. nullptr if AddChild
 *         fails.
 */
template <typename T>
llgc::utils::Tree<T>* llgc::utils::Tree<T>::AddChild(std::unique_ptr<T> child,
                                                     size_t id)
{
  size_t max_id = FindMaxId(id_, id_);
  llgc::utils::Tree<T>* parent = FindNode(id);

  BUGCRIT(std::cout, parent != nullptr, nullptr,
          "Failed to find node " + std::to_string(id) + ".\n");
  parent->children_.emplace_back(
      std::make_unique<Tree<T>>(max_id + 1, std::move(child), parent));

  return parent->children_.back().get();
}

/** \brief Find by recursion a path from the current node to the end of the
 *         path.
 * \param[in] end The end of the path.
 * \param[out] path The result of the path. Unmodified if the path is not found.
 *             Clear then fill if path is found.
 * \return true if found.
 */
template <typename T>
bool llgc::utils::Tree<T>::FindPath(size_t end, std::deque<T*>* path) const
{
  BUGCONT(std::cout, FindPath(nullptr, end, path), false);

  return true;
}

/** \brief Find by recursion a path between two nodes.
 * \param[in] begin The begin of the path.
 * \param[in] end The end of the path.
 * \param[out] path The result of the path. Unmodified if the path is not found.
 *             Clear then fill if path is found.
 * \return true if found.
 */
template <typename T>
bool llgc::utils::Tree<T>::FindPath(size_t begin, size_t end,
                                    std::deque<T*>* path) const
{
  const Tree<T>* begin_node = FindNode(begin);

  BUGCRIT(std::cout, begin_node != nullptr, false,
          "Failed to find the start node " + std::to_string(begin) + ".\n");
  BUGCRIT(std::cout, begin_node->FindPath(nullptr, end, path), false,
          "Failed to find the end node " + std::to_string(end) + ".\n");

  return true;
}

/** \brief Find a node based on the id.
 * \param[in] id The id of the node.
 * \return The node if found. nullptr if failed.
 */
template <typename T>
const llgc::utils::Tree<T>* llgc::utils::Tree<T>::FindNode(size_t id) const
{
  return FindNode(id, id_);
}

/** \brief Find a node based on the id.
 * \param[in] id The id of the node
 * \return The node if found. nullptr if failed.
 */
template <typename T>
llgc::utils::Tree<T>* llgc::utils::Tree<T>::FindNode(size_t id)
{
  return FindNode(id, id_);
}

/** \brief Find the max id.
 * \return Return the max id.
 */
template <typename T>
size_t llgc::utils::Tree<T>::FindMaxId() const
{
  return FindMaxId(id_, id_);
}

/** \fn size_t llgc::utils::Tree::GetId() const
 * \brief Get the id of the node.
 * \return The id of the node.
 *
 *
 * \fn T* llgc::utils::Tree::GetData()
 * \brief Return the data.
 * \return The pointer of a data.
 */

/** \brief Find by recursion a path from the current node to the end of the
 *         path.
 * \param[in] previous For recursion: the node caller.
 * \param[in] end The end of the path.
 * \param[out] path The result of the path.
 * \return true if found. path is fill only if return value is true.
 */
template <typename T>
bool llgc::utils::Tree<T>::FindPath(const Tree<T>* previous, size_t end,
                                    std::deque<T*>* path) const
{
  BUGPARAM(std::cout, 0, path != nullptr, false);

  // Find the end.
  if (id_ == end)
  {
    path->clear();
    path->push_front(data_.get());
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
      path->push_front(data_.get());
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
    path->push_front(data_.get());
    return true;
  }
  return false;
}

/** \brief Find a node based on the id.
 * \param[in] id The id of the node.
 * \param[in] previous For recursion: the id caller.
 * \return The node if found. nullptr if failed.
 */
template <typename T>
const llgc::utils::Tree<T>* llgc::utils::Tree<T>::FindNode(
    size_t id, size_t previous) const
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
    const Tree<T>* retval = children_[i]->FindNode(id, id_);
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
    const Tree<T>* retval = parent_->FindNode(id, id_);
    if (retval != nullptr)
    {
      return retval;
    }
  }

  return nullptr;
}

/** \brief Find a node based on the id.
 * \param[in] id The id of the node.
 * \param[in] previous For recursion: the id caller.
 * \return The node if found. nullptr if failed.
 */
template <typename T>
llgc::utils::Tree<T>* llgc::utils::Tree<T>::FindNode(size_t id, size_t previous)
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
    Tree<T>* retval = children_[i]->FindNode(id, id_);
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
    Tree<T>* retval = parent_->FindNode(id, id_);
    if (retval != nullptr)
    {
      return retval;
    }
  }

  return nullptr;
}

/** \brief Find the max id.
 * \param[in] max_id Previous max id.
 * \param[in] previous Previous id of the caller.
 * \return Return the max id.
 */
template <typename T>
size_t llgc::utils::Tree<T>::FindMaxId(size_t max_id, size_t previous) const
{
  // Start by children
  for (size_t i = 0; i < children_.size(); i++)
  {
    // Do not go back.
    if (children_[i].get()->id_ != previous)
    {
      max_id = children_[i]->FindMaxId(max_id, id_);
    }
  }

  // Do not go back.
  if (parent_ != nullptr && parent_->id_ != previous)
  {
    max_id = parent_->FindMaxId(max_id, id_);
  }

  return std::max(max_id, id_);
}

/** \var llgc::utils::Tree::id_
 * \brief Unique id of the node.
 *
 *
 * \var llgc::utils::Tree::data_
 * \brief Data of the node.
 *
 *
 * \var llgc::utils::Tree::parent_
 * \brief Parent of the node.
 *
 *
 * \var llgc::utils::Tree::children_
 * \brief List of children.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
