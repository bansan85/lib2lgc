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
 * @file gdb.h
 * @brief Class that run gdb for various purpose.
 */

#ifndef SOFTWARE_GDB_GDB_H_
#define SOFTWARE_GDB_GDB_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/singleton/singleton.h>
#include <bits/stdint-intn.h>
#include <memory>
#include <mutex>
#include <string>
namespace llgc::pattern::publisher
{
template <typename M>
class PublisherRemote;
}  // namespace llgc::pattern::publisher

namespace msg::software
{
class Gdbs;
}  // namespace msg::software

namespace llgc::software::gdb
{
/**
 * @brief Class to run gdb for various purpose.
 */
class Gdb : public llgc::pattern::singleton::Static<llgc::pattern::publisher::PublisherRemote<msg::software::Gdbs>>
{
 public:
  /**
   * @brief Run gdb to get the backtrace full. The output will be saved to
   * filename.btfull.
   *
   * @param[in] filename The file that make application crashes.
   * @param[in] argc Number of the arguments.
   * @param[in] argv Argument to run the program with the filename.
   * @param[in] timeout Timeout for the run of gdb.
   * replaced by @@. For example: /bin/prog @@
   *
   * @return true if no problem.
   */
  static bool RunBtFull(const std::string& filename, unsigned int argc,
                        char* const argv[], int64_t timeout) CHK;

  /**
   * @brief Find recursively all files and run gdb to get the backtrace full.
   *
   * @param[in] folder The root folder.
   * @param[in] nthread Number of parallel of gdb instance.
   * @param[in] regex Regex of the filename.
   * @param[in] argc Number of the arguments
   * @param[in] argv Argument to run the program with the filename
   * replaced by @@. For example: /bin/prog @@
   * @param[in] timeout Timeout for the run of gdb.
   *
   * @return true if no problem.
   */

  static bool RunBtFullRecursive(const std::string& folder,
                                 unsigned int nthread, const std::string& regex,
                                 unsigned int argc, char* const argv[],
                                 int64_t timeout) CHK;
  /**
   * @brief Get files from a list and run gdb to get the backtrace full.
   *
   * @param[in] list The file that have the list of files.
   * @param[in] nthread Number of parallel of gdb instance.
   * @param[in] argc Number of the arguments
   * @param[in] argv Argument to run the program with the filename
   * replaced by @@. For example: /bin/prog @@
   * @param[in] timeout Timeout for the run of gdb.
   *
   * @return true if no problem.
   */
  static bool RunBtFullList(const std::string& list, unsigned int nthread,
                            unsigned int argc, char* const argv[],
                            int64_t timeout) CHK;

  static void Forward(const std::shared_ptr<const std::string>& message);
};

}  // namespace llgc::software::gdb

template <typename T>
std::recursive_mutex llgc::pattern::singleton::Static<T>::mutex_static_;
template <typename T>
std::shared_ptr<T> llgc::pattern::singleton::Static<T>::instance_static_;

#endif  // SOFTWARE_GDB_GDB_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
