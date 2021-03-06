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

#ifndef PATTERN_COMMAND_UNDO_INTERFACE_H_
#define PATTERN_COMMAND_UNDO_INTERFACE_H_

#include <2lgc/compat.h>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Namespace for the pattern command.
 */
namespace llgc::pattern::command
{
/**
 * @brief Interface that define command stored in class.
 */
class UndoInterface
{
 public:
  /**
   * @brief Default constructor.
   */
  UndoInterface() : duration_start_(0.) {}

  /**
   * @brief Default destructor. Virtual because command is abstract.
   */
  virtual ~UndoInterface() = default;

#ifndef SWIG
  /**
   * @brief Delete move constructor.
   *
   * @param[in] other The original.
   */
  UndoInterface(UndoInterface &&other) = delete;

  /**
   * @brief Delete copy constructor.
   *
   * @param[in] other The original.
   */
  UndoInterface(UndoInterface const &other) = delete;

  /**
   * @brief Delete the move operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  UndoInterface &operator=(UndoInterface &&other) & = delete;

  /**
   * @brief Delete the copy operator.
   *
   * @param[in] other The original.
   *
   * @return Delete function.
   */
  UndoInterface &operator=(UndoInterface const &other) & = delete;
#endif  // !SWIG

  /**
   * @brief Execute the command.
   *
   * @return Fails if command have been already executed.
   */
  virtual bool Do() = 0;

  /**
   * @brief Undo the command.
   *
   * @return Fails if command have been already undoed.
   */
  virtual bool Undo() = 0;

  /**
   * @brief Return a command if redo is possible.
   *
   * @return Return the new command if possible. nullptr is not redoable.
   */
  virtual std::unique_ptr<UndoInterface> Redo() = 0;

  /**
   * @brief Tell if the command change the model or just the gui (and should
   * not notify others instances).
   *
   * @return true if command change the model.
   */
  virtual bool ChangeModel() = 0;

  /**
   * @brief Return the description of the command.
   *
   * @return The description.
   */
  virtual std::string GetDescription() const = 0;

  /**
   * @brief Get the position on the data where the modification are done.
   *
   * @return The position in protobuf format.
   */
  virtual std::string GetPosition() const = 0;

  /**
   * @brief Get the owner of the creator of the command.
   *
   * @return A number that the undomanager know more about it.
   */
  virtual size_t GetUser() const = 0;

  /**
   * @brief Convert the command to a protobuf serialization.
   *
   * @return The protobuf string.
   */
  virtual std::string ToProtobuf() const = 0;

 protected:
  /**
   * @brief Time of the execution of the command.
   */
  std::chrono::time_point<std::chrono::system_clock> time_start_;

  /**
   * @brief Duration of the execution of the command.
   */
  std::chrono::duration<double> duration_start_;

  /**
   * @brief Tell if the command was executed.
   */
  bool done = false;
};

}  // namespace llgc::pattern::command

#endif  // PATTERN_COMMAND_UNDO_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
