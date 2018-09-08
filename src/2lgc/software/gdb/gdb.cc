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
#include <2lgc/filesystem/files.h>
#include <2lgc/poco/software_gdb.pb.h>
#include <2lgc/software/gdb/gdb.h>
#include <2lgc/software/gdb/gdb_server.h>
#include <cxxabi.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstring>
#include <ext/alloc_traits.h>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <system_error>
#include <thread>
// No for GCC-8. GCC-6 wants.
#include <type_traits>  // IWYU pragma: keep
#include <vector>

#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/singleton.cc>

/** \class llgc::software::gdb::Gdb
 * \brief Class to run gdb for various purpose.
 */

/// \brief Server to send messages to subscribers.
llgc::software::gdb::GdbServer llgc::software::gdb::Gdb::server_;

/** \brief Run gdb to get the backtrace full. The output will be saved to
 *         filename.btfull.
 * \param[in] filename The file that make application crashes.
 * \param[in] argc Number of the arguments.
 * \param[in] argv Argument to run the program where the filename replaced by
              @@. For example: /bin/prog @@.
 * \param[in] timeout Timeout for the run of gdb.
 * \return true if no problem.
 */
bool llgc::software::gdb::Gdb::RunBtFull(const std::string &filename,
                                         unsigned int argc,
                                         const char *const argv[],
                                         int64_t timeout)
{
  auto argvbis = std::make_unique<const char *[]>(argc + 24);
  std::string btfullfile;
  btfullfile.assign("set logging file ");
  btfullfile.append(filename);
  btfullfile.append(".btfull");
  argvbis[0] = "/usr/bin/gdb";
  argvbis[1] = "-batch-silent";
  // See https://sourceware.org/ml/gdb/2018-03/msg00029.html for tty /dev/null.
  // and https://www.cs.ait.ac.th/~on/O/oreilly/unix/upt/ch12_07.htm
  argvbis[2] = "-ex";
  argvbis[3] = "tty /dev/null";
  argvbis[4] = "-ex";
  argvbis[5] = "run";
  argvbis[6] = "-ex";
  argvbis[7] = "set logging overwrite on";
  argvbis[8] = "-ex";
  argvbis[9] = btfullfile.c_str();
  argvbis[10] = "-ex";
  argvbis[11] = "set logging on";
  argvbis[12] = "-ex";
  argvbis[13] = "set pagination off";
  argvbis[14] = "-ex";
  argvbis[15] = "handle SIG33 pass nostop noprint";
  argvbis[16] = "-ex";
  argvbis[17] = "backtrace full";
  argvbis[18] = "-ex";
  argvbis[19] = "set logging off";
  argvbis[20] = "-ex";
  argvbis[21] = "quit";
  argvbis[22] = "--args";
  for (unsigned int i = 0; i < argc; i++)
  {
    if (strcmp("@@", argv[i]) == 0)
    {
      // a const_cast is necessary. argvbis must be not const.
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
      argvbis[23 + i] = const_cast<char *>(filename.c_str());
    }
    else
    {
      argvbis[23 + i] = argv[i];
    }
  }
  argvbis[23 + argc] = nullptr;
  pid_t child_pid = fork();
  BUGCRIT(std::cout, child_pid != -1, false,
          "Failed to create fork. Errno " + std::to_string(errno) + "\n.");
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
      BUGCRIT(std::cout, wait_pid != -1, false,
              "Failed to waitpid. Errno " + std::to_string(errno) + "\n.");
      end = std::chrono::system_clock::now();
      elapsed_seconds =
          std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
      if (wait_pid == 0)
      {
        if (elapsed_seconds < timeout)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else
        {
          BUGCRIT(std::cout, kill(child_pid, SIGKILL) == 0, false,
                  "Failed to kill. Errno " + std::to_string(errno) + ".\n");

          llgc::protobuf::software::Gdb messages_gdb;
          auto message = messages_gdb.add_msg();
          auto run_bt_full_time_out = std::make_unique<
              llgc::protobuf::software::Gdb::Msg::RunBtFullTimeOut>();
          std::string *filename_gdb = run_bt_full_time_out->add_file();
          filename_gdb->assign(filename);
          message->set_allocated_run_bt_full_time_out(
              run_bt_full_time_out.release());
          BUGCONT(std::cout, server_.Forward(messages_gdb), false);

          return false;
        }
      }
    } while (wait_pid == 0 && elapsed_seconds < timeout);
  }
  else
  {
    // a const_cast is necessary.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    BUGCRIT(std::cout,
            execvp(argvbis[0], const_cast<char *const *>(argvbis.get())) != -1,
            false, "Failed to run " + std::string(argvbis[0]) + ".\n");
  }
  return true;
}

namespace llgc::software::gdb
{
namespace
{
/** \brief Run parallel gdb for a given list of files.
 * \param[in,out] all_files The list of files.
 * \param[in,out] nthread The number of parallel instance of GDB.
 * \param[in,out] argc The number of arguments.
 * \param[in,out] argv[] The arguments.
 * \param[in,out] timeout Timeout in second.
 * \return true if all gdb instances are successfull.
 */
bool ParallelRun(const std::vector<std::string> &all_files,
                 unsigned int nthread, unsigned int argc,
                 const char *const argv[], int64_t timeout)
{
  bool retval = true;
  const unsigned int nthreads =
      std::min(std::min(nthread, std::thread::hardware_concurrency()),
               static_cast<unsigned int>(all_files.size()));
  std::vector<std::future<bool>> threads(nthreads);
  for (size_t t = 0; t < nthreads; t++)
  {
    threads[t] = std::async(
        std::launch::async, [&all_files, nthreads, argc, argv, timeout, t] {
          bool retval2 = true;
          for (size_t i = t; i < all_files.size(); i += nthreads)
          {
            retval2 &= llgc::software::gdb::Gdb::RunBtFull(all_files[i], argc,
                                                           argv, timeout);
          }
          return retval2;
        });
  }
  for (std::future<bool> &t : threads)
  {
    retval &= t.get();
  }

  return retval;
}

}  // namespace
}  // namespace llgc::software::gdb

/** \brief Find recursively all files and run gdb to get the backtrace full.
 * \param[in] folder The root folder.
 * \param[in] nthread Number of parallel of gdb instance.
 * \param[in] regex Regex of the filename.
 * \param[in] argc Number of the arguments.
 * \param[in] argv Argument to run the program with the filename replaced by
 *            @@. For example: /bin/prog @@
 * \param[in] timeout Timeout in second for the run of gdb.
 * \return true if no problem.
 */
bool llgc::software::gdb::Gdb::RunBtFullRecursive(
    const std::string &folder, unsigned int nthread, const std::string &regex,
    unsigned int argc, const char *const argv[], int64_t timeout)
{
  std::vector<std::string> all_files;
  BUGCRIT(
      std::cout,
      llgc::filesystem::Files::SearchRecursiveFiles(folder, regex, &all_files),
      false, "Failed to recursively read " + folder + ".\n");

  BUGCONT(std::cout, ParallelRun(all_files, nthread, argc, argv, timeout),
          false);
  return true;
}

/** \brief Get files from a list and run gdb to get the backtrace full.
 * \param[in] list The file that have the list of files.
 * \param[in] nthread Number of parallel of gdb instance.
 * \param[in] argc Number of the arguments.
 * \param[in] argv Argument to run the program with the filename replaced by @@.
 *            For example: /bin/prog @@
 * \param[in] timeout Timeout in second for the run of gdb.
 * \return true if no problem.
 */
bool llgc::software::gdb::Gdb::RunBtFullList(const std::string &list,
                                             unsigned int nthread,
                                             unsigned int argc,
                                             const char *const argv[],
                                             int64_t timeout)
{
  std::vector<std::string> all_files;
  std::string line;
  std::ifstream f(list);
  BUGUSER(std::cout, f.is_open(), false, "Failed to open " + list + ".\n");

  while (std::getline(f, line))
  {
    all_files.push_back(line);
  }

  BUGCONT(std::cout, ParallelRun(all_files, nthread, argc, argv, timeout),
          false);
  return true;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
