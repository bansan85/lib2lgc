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

/**
 * @file set_stack.h
 * @brief Store and sort all stacks. Criterea of sort must be defined on
 * creation.
 */

#ifndef SET_STACK_H_
#define SET_STACK_H_

// C++ system
#include <memory>
#include <mutex>
#include <set>

// Compatibility
#include <compat.h>

#include "stack.h"

/**
 * @brief Store and sort all stacks.
 *
 * @details Criterea of sort must be defined on creation.
 */
class SetStack
{
 public:
  /**
   * @brief Default constructor.
   *
   * @param[in] with_source_only Ignore backtrace where source file is unknown.
   * @param[in] top_frame Number of frames from the top that must be identical
   * so two stacks are the same. Internally, this number can not exceeded the
   * number of frames.
   * @param[in] bottom_frame Number of frames from the bottom that must be
   * identical so two stacks are the same. Internally, this number can not
   * exceeded the number of frames.
   */
  SetStack(bool with_source_only, size_t top_frame, size_t bottom_frame);

  /**
   * @brief Add a new stack. The file must contains only the full backtrace from
   * GDB.
   *
   * @param[in] filename The file to add.
   *
   * @return True if the file is a valid backtrace.
   */
  bool Add(const std::string& filename) CHK;

  /**
   * @brief Add new stacks based on all files that end with .btfull. All files
   * must contains only the full backtrace from GDB.
   *
   * @param[in] folder The folder where all *.btfull files are.
   * @param[in] nthread The number of threads if parallel is allowed.
   * @param[in] regex Regex that match file to read.
   *
   * @return Always true even if a file is corrupted.
   */
  bool AddRecursive(const std::string& folder, unsigned int nthread,
                    const std::string& regex) CHK;

  /**
   * @brief Show all stacks grouped by condition passed with the constructor.
   */
  void Print(bool print_one_by_group);

 private:
  /**
   * @brief A local class that compare to stack and says if two stack looks to
   * be the same.
   */
  struct Local
  {
    /**
     * @brief Constructor with parameter of comparaison.
     *
     * @param[in] with_source_only Ignore backtrace where source file is
     * unknown.
     * @param[in] top_frame Number of frames from the top that must be identical
     * so two stacks are the same. Internally, this number can not exceeded the
     * number of frames.
     * @param[in] bottom_frame Number of frames from the bottom that must be
     * identical so two stacks are the same. Internally, this number can not
     * exceeded the number of frames.
     */
    Local(bool with_source_only, size_t top_frame, size_t bottom_frame);

    /**
     * @brief Function that compare two stacks.
     *
     * @param[in] i The first stack.
     * @param[in] j The second stack.
     *
     * @return If i < j based on argument from constructor.
     */
    bool operator()(const std::unique_ptr<Stack>& i,
                    const std::unique_ptr<Stack>& j);

    /**
     * @brief Helper to operator(). Compare two stacks from the top or from the
     * bottom.
     *
     * @param[in] nb_max_frames Number maximum of backtraces to compare.
     * @param[in] &Stack::GetBacktraceFromTop Compare from the top or from the
     * bottom.
     * @param[in] i The first stack.
     * @param[in] j The second stack.
     *
     * @return -1 if i < j, 0 if i == j and -1 if i > j.
     */
    int CompareFrom(const size_t nb_max_frames,
                    decltype(&Stack::GetBacktraceFromTop) get_backtraces,
                    const std::unique_ptr<Stack>& i,
                    const std::unique_ptr<Stack>& j);

    /**
     * @brief Ignore backtrace where source file is unknown.
     */
    bool with_source_only_;

    /**
     * @brief Number of frames from the top that must be identical so two stacks
     * are the same. Internally, this number can not exceeded the number of
     * frames.
     */
    size_t top_frame_;

    /**
     * @brief Number of frames from the bottom that must be identical so two
     * stacks are the same. Internally, this number can not exceeded the number
     * of frames.
     */
    size_t bottom_frame_;
  };

  /**
   * @brief Storage of all stacks sorted with parameter given by the
   * constructor.
   */
  std::multiset<std::unique_ptr<Stack>, Local> stack_;

  /**
   * @brief A internal mutex to use stack_ thread-safe.
   */
  std::mutex mutex_stack_;
};

#endif  // SET_STACK_H_
