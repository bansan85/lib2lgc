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

#ifndef VISITOR_H_
#define VISITOR_H_

// C++ system
#include <string>

// lib2lgcMath
#include <compat.h>

/**
 * @file
 *
 * This header is used has base for visitors in the pattern visitor.
 */
namespace pattern {
namespace visitor {

/**
 * @brief Common class for all visitors.
 */
class BaseVisitor {
 public:
  /**
   * @brief In case of some class based on it need a virtual destructor.
   */
  virtual ~BaseVisitor() {}
};

/**
 * @brief Class that is used to force the implementation of the Visit method
 *        with the right arguments.
 *
 * @tparam T The class that the visiting must be implement.
 */
template <class T>
class Visitor {
 public:
  /**
   * @brief In case of some class based on it need a virtual destructor.
   */
  virtual ~Visitor() {}
  /**
   * @brief The method that will use only data and the return will be put in
   *        retun_value.
   *
   * @param data The data used by the visitor.
   * @param return_value the return value generated by SerializeToString.
   *
   * @return false if there is a problem or if SerializeToString failed.
   *         true instead.
   */
  virtual bool Visit(const T &data, std::string *return_value) const CHK = 0;
};

}  // namespace visitor
}  // namespace pattern

#endif  // VISITOR_H_
