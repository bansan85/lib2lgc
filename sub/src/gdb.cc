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

#include "gdb.h"
#include <bits/stdint-intn.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <experimental/filesystem>
#include <functional>
#include <iostream>
#include <regex>
#include <thread>
#include <type_traits>
#include <vector>

bool Gdb::RunBtFull(const std::string& filename, unsigned int argc,
                    char* const argv[])
{
  const char** argvbis =
      static_cast<const char**>(malloc(sizeof(char*) * (argc + 22)));
  size_t len =
      sizeof(char) * (sizeof("set logging file .btfull") + filename.length());
  char* btfullfile = static_cast<char*>(malloc(len));
  snprintf(btfullfile, len, "%s%s%s", "set logging file ", filename.c_str(),
           ".btfull");
  argvbis[0] = "/usr/bin/gdb";
  argvbis[1] = "-batch-silent";
  argvbis[2] = "-ex";
  argvbis[3] = "run";
  argvbis[4] = "-ex";
  argvbis[5] = "set logging overwrite on";
  argvbis[6] = "-ex";
  argvbis[7] = btfullfile;
  argvbis[8] = "-ex";
  argvbis[9] = "set logging on";
  argvbis[10] = "-ex";
  argvbis[11] = "set pagination off";
  argvbis[12] = "-ex";
  argvbis[13] = "handle SIG33 pass nostop noprint";
  argvbis[14] = "-ex";
  argvbis[15] = "backtrace full";
  argvbis[16] = "-ex";
  argvbis[17] = "set logging off";
  argvbis[18] = "-ex";
  argvbis[19] = "quit";
  argvbis[20] = "--args";
  for (unsigned int i = 0; i < argc; i++)
  {
    if (strcmp("@@", argv[i]) == 0)
    {
      argvbis[21 + i] = const_cast<char*>(filename.c_str());
    }
    else
    {
      argvbis[21 + i] = argv[i];
    }
  }
  argvbis[21 + argc] = nullptr;
  pid_t child_pid = fork();
  if (child_pid != 0)
  {
    int status;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    int64_t elapsed_seconds;
    pid_t wait_pid;
    do
    {
      wait_pid = waitpid(child_pid, &status, WNOHANG);
      end = std::chrono::system_clock::now();
      elapsed_seconds =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      if (wait_pid == 0)
      {
        if (elapsed_seconds <= 40)
        {
          sleep(1);
        }
        else
        {
          kill(child_pid, SIGKILL);
          std::cout << "OUPS..." << filename << std::endl;
        }
      }
    } while (wait_pid == 0 && elapsed_seconds <= 40);
    free(argvbis);
    free(btfullfile);
  }
  else
  {
    execvp(argvbis[0], const_cast<char* const*>(argvbis));
  }
  return true;
}

bool Gdb::RunBtFullRecursive(const std::string& folder, unsigned int nthread,
                             const std::string& regex, unsigned int argc,
                             char* const argv[])
{
  std::vector<std::string> all_files;
  std::regex reg(regex);
  for (auto& p :
       std::experimental::filesystem::recursive_directory_iterator(folder))
  {
    std::string filename(p.path().filename().string());

    if (regex.length() == 0 || std::regex_match(filename, reg))
    {
      all_files.push_back(p.path().string());
    }
  }

  const unsigned int nthreads =
      std::min(nthread, std::thread::hardware_concurrency());
  std::vector<std::thread> threads(nthreads);
  for (size_t t = 0; t < nthreads; t++)
  {
    threads[t] = std::thread(std::bind(
        [&all_files, nthreads, argc, &argv](const size_t i_start) {
          for (size_t i = i_start; i < all_files.size(); i += nthreads)
          {
            RunBtFull(all_files[i], argc, argv);
          }
        },
        t));
  }
  std::for_each(threads.begin(), threads.end(),
                [](std::thread& x) { x.join(); });

  return true;
}
