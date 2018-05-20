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

#ifndef UTILS_UNDOMANAGER_UNDOMANAGER_H_
#define UTILS_UNDOMANAGER_UNDOMANAGER_H_

#include <2lgc/compat.h>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief 
 */
namespace llgc::pattern
{
template <typename T, typename U>
class AbstractFactory;
}

/**
 * @brief 
 */
namespace llgc::utils::tree
{
template <typename T>
class Node;
}

/**
 * @brief Namespace for the pattern command.
 */
namespace llgc::utils::undomanager
{
/**
 * @brief Undomanager able you to undo or redo some commands.
 * Command have id. The id could also be the version of the project that use
 * the undomanager.
 *
 * The goal of the undomanager is to store command to undo or redo. Because data
 * could be transfert throw PubliserTcp, the storage is done in std::string. The
 * abstract factory will be used to convert std::string to Command.
 */
template <typename T, typename U>
class Undomanager
{
 public:
  /**
   * @brief Default constructor and create an empty undomanager.
   *
   * @param[in] abstract_factory The abstract factory that convert a
   * std::string from a protobuf message to a command interface.
   */
  explicit Undomanager(
      std::unique_ptr<llgc::pattern::AbstractFactory<T, U>> abstract_factory);

  /**
   * @brief Default constructor and load an undomanager from a file.
   *
   * @param[in] abstract_factory The abstract factory that convert a
   * std::string from a protobuf message to a command interface.
   * @param[in] file The file to load.
   */
  Undomanager(
      std::unique_ptr<llgc::pattern::AbstractFactory<T, U>> abstract_factory,
      const std::string& file);

  /**
   * @brief Add a command to the undomanager.
   *
   * @param[in] command The new command.
   * @param[in] id The id of the parent command.
   *
   * @return A pointer to the new child. nullptr if AddChild fails.
   */
  llgc::utils::tree::Node<std::string>* AddCommand(
      std::unique_ptr<std::string> command, size_t id) CHK;

  /**
   * @brief eiua
   *
   * @param[in] start eiua
   * @param[in] end eiua
   * @param[in] insert eiua
   *
   * @return eiua
   */
  std::vector<std::string> DuplicateCommand(size_t start, size_t end,
                                            size_t insert);

  /**
   * @brief i ei eiua ep
   *
   * @param[in,out] id2 a eiu aeiu a
   *
   * @return i a.éuàpi, à
   */
  bool UndoCommand(size_t id2) CHK;

  /**
   * @brief eiua
   *
   * @param[in,out] start eiua
   * @param[in,out] end eiua
   *
   * @return eiua
   */
  bool UndoCommands(size_t start, size_t end) CHK;

  /**
   * @brief eiua
   *
   * @param[in,out] before eiua
   * @param[in,out] after eiau
   */
  void DrawHistory(void* before, void* after);

  /**
   * @brief
   *
   * @return List of element modifyed by type and description. To
   * UI undo description.
   */
  virtual std::vector<size_t> GetType() const = 0;

  /**
   * @brief
   *
   * @param[in,out] i
   *
   * @return
   */
  virtual std::vector<T> FindByZone(int i) const = 0;

 private:
  /**
   * @brief Storage of all commands in protobuf message format.
   */
  std::unique_ptr<llgc::utils::tree::Node<std::string>> memory_;

  /**
   * @brief Abstract factory that convert string to Command.
   */
  std::unique_ptr<llgc::pattern::AbstractFactory<T, U>>& abstract_factory_;

  /**
   * @brief If user can undo action of other user.
   */
  bool undo_local_or_global_;

  /**
   * @brief Id of the user for having the description of the user.
   */
  std::map<size_t, std::string> users_;
};

}  // namespace llgc::utils::undomanager

#endif  // UTILS_UNDOMANAGER_UNDOMANAGER_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
