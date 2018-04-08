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
 * @file iterator.h
 * @brief Interface for the ranged-base loop iterator.
 */

#ifndef PATTERN_ITERATOR_ITERATOR_H_
#define PATTERN_ITERATOR_ITERATOR_H_

namespace llgc::pattern::iterator
{
/**
 * @brief Iterator for Stack.
 */
/**
 * @brief 
 *
 * @tparam T Class that will be loop.
 * @tparam U Object that will be return by the loop.
 */
template <class T, class U>
class Iterator
{
 public:
  /**
   * @brief The constructor
   *
   * @param[in] data The data contains the list of object.
   * @param[in] pos The position of the current object.
   */
  Iterator(const T& data, size_t pos) : data_(data), pos_(pos) { }

  /**
   * @brief Compare operator
   *
   * @param[in] other The iterator to compare with.
   *
   * @return true if different.
   */
  bool operator!=(const Iterator& other) const { return pos_ != other.pos_; }

  /**
   * @brief Dereference an iterator return the current object.
   *
   * @return Return the current object.
   */
  virtual const U& operator*() const = 0;

  /**
   * @brief Increment the current object.
   *
   * @return Himself.
   */
  const Iterator& operator++()
  {
    ++pos_;
    return *this;
  }

 protected:
  /**
   * @brief The data that contains all objects.
   */
  const T& data_;

  /**
   * @brief The position of the current object.
   */
  size_t pos_;
};

}  // namespace llgc::software::gdb

#endif  // SOFTWARE_GDB_STACK_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
