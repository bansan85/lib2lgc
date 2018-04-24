/* Copyright 2018 LE GARREC Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PATTERN_COMMAND_COMMAND_H_
#define PATTERN_COMMAND_COMMAND_H_

#include <2lgc/compatibility/visual_studio.h>
#include <chrono>
#include <memory>
#include <string>

/**
 * @brief Namespace for the pattern command.
 */
namespace llgc::pattern::command
{
/**
 * @brief Interface that define command stored in class.
 */
class Command
{
 public:
  Command(std::chrono::time_point<std::chrono::system_clock> time);

  virtual ~Command() = default;
  /**
   * @brief Execute the command.
   *
   * @return true if no problem.
   */
  virtual bool Do() = 0;

  /**
   * @brief Undo the command.
   *
   * @return true if no problem.
   */
  virtual void Undo() = 0;

  /**
   * @brief Repeat the command.
   *
   * @return true if no problem.
   */
  virtual void Redo() = 0;

  /**
   * @brief Return the description of the command.
   *
   * @return The description.
   */
  virtual const std::string & GetDescription() const = 0;

  /**
   * @brief Convert the command to a protobuf serialization.
   *
   * @return The protobuf string.
   */
  virtual const std::string * ToProtobuf() const = 0;

 private:
  /**
   * @brief Time of the execution of the command.
   */
  std::chrono::time_point<std::chrono::system_clock> time_start_;

  /**
   * @brief Duration of the execution of the command.
   */
  std::chrono::duration<double> duration_start_;

  /**
   * @brief Next simultaneous command.
   */
  std::unique_ptr<Command> next_;

  /**
   * @brief Sub command.
   */
  std::unique_ptr<Command> child_;
};

}  // namespace llgc::pattern::command

#endif  // PATTERN_COMMAND_COMMAND_INTERFACE_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
