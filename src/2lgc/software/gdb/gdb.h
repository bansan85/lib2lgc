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
#include <bits/stdint-intn.h>
#include <string>

/**
 * @brief Class to run gdb for various purpose.
 */
class Gdb
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
   * @brief Run gdb to get the backtrace full and find recursively all files.
   *
   * @param[in] folder The root folder.
   * @param[in] nthread Number of parallel of gdb instance.
   * @param[in] regex Regex of the filename.
   * @param[in] argc Number of the arguments
   * @param[in] argv Argument to run the program with the filename
   * @param[in] timeout Timeout for the run of gdb.
   * replaced by @@. For example: /bin/prog @@
   *
   * @return true if no problem.
   */
  static bool RunBtFullRecursive(const std::string& folder,
                                 unsigned int nthread, const std::string& regex,
                                 unsigned int argc, char* const argv[],
                                 int64_t timeout) CHK;
};

#endif  // SOFTWARE_GDB_GDB_H_
