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

#include <2lgc/pattern/publisher/publisher_remote.h>
#include <2lgc/software/gdb/gdb_server.h>
#include <2lgc/pattern/publisher/publisher_base.cc>
#include <2lgc/pattern/publisher/publisher_remote.cc>
#include <2lgc/pattern/singleton/singleton.cc>
#include <memory>
#include <string>

void llgc::software::gdb::GdbServer::Forward(
    const std::shared_ptr<const std::string>& message)
{
  // Check if instance.
  if (IsInstance())
  {
    // If the instance if freed, GetInstance will create it.
    auto singleton_ = GetInstance();
    singleton_->Forward(message);
  }
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
