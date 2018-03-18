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

#ifndef POCO_NUMBER_VISITOR_VALUE_H_
#define POCO_NUMBER_VISITOR_VALUE_H_

#include <2lgc/compatibility/visual_studio.h>
#include <2lgc/pattern/visitor/visitor.h>
#include <string>

namespace pattern
{
namespace visitor
{
// We can't include number.h because number.h need number_visitor_value.h.
class Number_Constant;
class Number_NumOpNum;

/**
 * @brief The visitor that will return the floating value.
 */
class NumberVisitorVal : public BaseVisitor,
                         public Visitor<Number_Constant>,
                         public Visitor<Number_NumOpNum>
{
 public:
  /**
   * @brief Default destructor.
   */
  virtual ~NumberVisitorVal() {}
  /**
   * @brief Return the floating value of a constant number.
   *
   * @param data The constant number.
   * @param return_value This parameter will be changed only if Visit success.
   *
   * @return true if success, false instead.
   */
  bool Visit(const Number_Constant &data,
             std::string *return_value) const override CHK;
  /**
   * @brief Return the floating value of a number based on an operation of two
   *        numbers.
   *
   * @param data The number.
   * @param return_value This parameter will be changed only if Visit success.
   *
   * @return true if success, false instead.
   */
  bool Visit(const Number_NumOpNum &data,
             std::string *return_value) const override CHK;
};

}  // namespace visitor
}  // namespace pattern

#endif  // POCO_NUMBER_VISITOR_VALUE_H_
