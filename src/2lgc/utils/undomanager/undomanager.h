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

namespace llgc
{
namespace pattern
{
template <typename T, typename U>
class AbstractFactory;
}
namespace utils
{
template <typename T>
class Tree;
}
}  // namespace llgc

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
 *
 * @tparam T The class message from protobuf class.
 * @tparam U The command interface.
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
   *
   * @return A pointer to the new child. nullptr if AddChild fails.
   */
  llgc::utils::Tree<U>* AddCommand(const std::string& command);

  /**
   * @brief Add a command to the undomanager.
   *
   * @param[in] id The id of the parent command.
   * @param[in] command The new command.
   *
   * @return A pointer to the new child. nullptr if AddChild fails.
   */
  llgc::utils::Tree<U>* AddCommand(size_t id, const std::string& command);

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
   * @brief Do one command.
   *
   * @param[in] id Id of the command to do.
   *
   * @return true if do is a success.
   */
  bool DoCommand(size_t id) CHK;

  /**
   * @brief Do one command.
   *
   * @param[in] start Id of the first command to do.
   * @param[in] end Id of the last command to do.
   *
   * @return true if do is a success for all commands.
   */
  bool DoCommands(size_t start, size_t end) CHK;

  /**
   * @brief Undo one command.
   *
   * @param[in] id Id of the command to undo.
   *
   * @return true if undo is a success.
   */
  bool UndoCommand(size_t id) CHK;

  /**
   * @brief Undo one command.
   *
   * @param[in] start Id of the first command to undo.
   * @param[in] end Id of the last command to undo.
   *
   * @return true if undo is a success for all commands.
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
  std::vector<size_t> GetType() const;

  /**
   * @brief eiuae
   *
   * @param[in] i eiu
   *
   * @return eiua
   */
  std::vector<T> FindByZone(int i) const;

  /**
   * @brief Start the modification for a new set of commands.
   *
   * @param[in] id id of the command to append this command.
   *
   * @return true if no problem.
   */
  bool BeginNewCommand(size_t id);

  /**
   * @brief End the modification for a new set of commands.
   *
   * @return true if no problem.
   */
  bool EndNewCommand();

 private:
  /**
   * @brief Storage of all commands in protobuf message format.
   */
  std::unique_ptr<llgc::utils::Tree<U>> memory_;

  /**
   * @brief Abstract factory that convert string to Command.
   */
  std::unique_ptr<llgc::pattern::AbstractFactory<T, U>> abstract_factory_;

  /**
   * @brief If user can undo action of other user.
   */
  bool undo_only_local_ = false;

  /**
   * @brief Id of the user for having the description of the user.
   */
  std::map<size_t, std::string> users_;

  /**
   * @brief Add a node to the unique_ptr tree.
   *
   * @param[in] child The data to add.
   *
   * @return Return the new node.
   */
  llgc::utils::Tree<U>* Add(std::unique_ptr<U> child);

  /**
   * @brief Add a node to the unique_ptr tree.
   *
   * @param[in] id The id of the parent.
   * @param[in] child The data to add.
   *
   * @return Return the new node.
   */
  llgc::utils::Tree<U>* Add(size_t id, std::unique_ptr<U> child);
};

}  // namespace llgc::utils::undomanager

#endif  // UTILS_UNDOMANAGER_UNDOMANAGER_H_

#ifndef TEMPLATE_CLASS
#include <2lgc/utils/undomanager/undomanager.cc>
#endif

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
