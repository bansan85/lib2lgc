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

/**
 * @brief Common namespace for miscellaneous functionality.
 */
namespace llgc::utils
{
/**
 * @brief Node of the bidirectional tree. Composite pattern.
 *
 * @tparam T The type of the elements.
 */
template <typename T>
class Tree
{
 public:
  /**
   * @brief Constructor of the node.
   *
   * @param[in] id The id of the new node.
   * @param[in] data The element.
   * @param[in] parent Parent of the node. nullptr if root of the tree.
   */
  Tree(size_t id, std::unique_ptr<T> data, Tree<T>* parent);

  /**
   * @brief Add a child to the current node.
   *
   * @param[in] child The new child.
   *
   * @return A pointer to the new child with the new id.
   * nullptr if AddChild fails.
   */
  Tree<T>* AddChild(std::unique_ptr<T> child) CHK;

  /**
   * @brief Add a child to the specific node.
   *
   * @param[in] child The new child.
   * @param[in] id The id of the parent node.
   *
   * @return A pointer to the new child with the new id.
   * nullptr if AddChild fails.
   */
  Tree<T>* AddChild(std::unique_ptr<T> child, size_t id) CHK;

  /**
   * @brief Find by recursion a path from the current node to the end of the
   * path.
   *
   * @param[in] end The end of the path.
   * @param[out] path The result of the path. Unmodified if the path is not
   * found. Clear then fill if path is found.
   *
   * @return true if found.
   */
  bool FindPath(size_t end, std::deque<T*>* path) const CHK;

  /**
   * @brief Find by recursion a path between two nodes.
   *
   * @param[in] begin The begin of the path.
   * @param[in] end The end of the path.
   * @param[out] path The result of the path. Unmodified if the path is not
   * found. Clear then fill if path is found.
   *
   * @return true if found.
   */
  bool FindPath(size_t begin, size_t end, std::deque<T*>* path) const CHK;

  /**
   * @brief Find a node based on the id.
   *
   * @param[in] id The id of the node.
   *
   * @return The node if found. nullptr if failed.
   */
  const Tree<T>* FindNode(size_t id) const;

  /**
   * @brief Find a node based on the id.
   *
   * @param[in] id The id of the node.
   *
   * @return The node if found. nullptr if failed.
   */
  Tree<T>* FindNode(size_t id);

  /**
   * @brief Find the max id.
   *
   * @return Return the max id.
   */
  size_t FindMaxId() const;

  /**
   * @brief Get the id of the node.
   *
   * @return The id of the node.
   */
  size_t GetId() const { return id_; }

  /**
   * @brief Return the data.
   *
   * @return The pointer of a data.
   */
  T* GetData() { return data_.get(); }

 private:
  /**
   * @brief Unique id of the node.
   */
  size_t id_;

  /**
   * @brief Data of the node.
   */
  std::unique_ptr<T> data_;

  /**
   * @brief Parent of the node.
   */
  Tree<T>* parent_;

  /**
   * @brief List of children.
   */
  std::vector<std::unique_ptr<Tree<T>>> children_;

  /**
   * @brief Find by recursion a path from the current node to the end of the
   * path.
   *
   * @param[in] previous For recursion: the node caller.
   * @param[in] end The end of the path.
   * @param[out] path The result of the path.
   *
   * @return true if found. path is fill only if return value is true.
   */
  bool FindPath(const Tree<T>* previous, size_t end,
                std::deque<T*>* path) const CHK;

  /**
   * @brief Find a node based on the id.
   *
   * @param[in] id The id of the node.
   * @param[in] previous For recursion: the id caller.
   *
   * @return The node if found. nullptr if failed.
   */
  const Tree<T>* FindNode(size_t id, size_t previous) const;

  /**
   * @brief Find a node based on the id.
   *
   * @param[in] id The id of the node.
   * @param[in] previous For recursion: the id caller.
   *
   * @return The node if found. nullptr if failed.
   */
  Tree<T>* FindNode(size_t id, size_t previous);

  /**
   * @brief Find the max id.
   *
   * @param[in] max_id Previous max id.
   * @param[in] previous Previous id of the caller.
   *
   * @return Return the max id.
   */
  size_t FindMaxId(size_t max_id, size_t previous) const;
};

}  // namespace llgc::utils

#endif  // UTILS_TREE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
