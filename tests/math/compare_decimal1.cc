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

#include <2lgc/math/compare_decimal.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>

int main(int /* argc */, char* /* argv */ [])
{
  assert(Math::AlmostEqualUlpsAndAbsF(0.5f, 1.f / 2.f, 1.e-10f, 4));
  assert(!Math::AlmostEqualUlpsAndAbsF(0.5f, -0.5f, 1.e-10f, 4));
  assert(!Math::AlmostEqualUlpsAndAbsF(-0.5f, 0.5f, 1.e-10f, 4));
  // Ulp: 2
  assert(Math::AlmostEqualUlpsAndAbsF(0.5f, 0.5000001f, 0.f, 4));

  assert(Math::AlmostEqualUlpsAndAbsD(0.5, 1. / 2., 1.e-15, 4));
  assert(!Math::AlmostEqualUlpsAndAbsD(0.5, -0.5, 1.e-15, 4));
  assert(!Math::AlmostEqualUlpsAndAbsD(-0.5, 0.5, 1.e-15, 4));
  // Ulp: 1
  assert(Math::AlmostEqualUlpsAndAbsD(0.5, 0.5000000000000001, 0., 5));

  assert(Math::AlmostEqualRelativeAndAbsF(0.5f, 1.f / 2.f, 1.e-10f, 1.e-10f));
  assert(!Math::AlmostEqualRelativeAndAbsF(0.5f, -0.5f, 1.e-10f, 1.e-10f));
  assert(!Math::AlmostEqualRelativeAndAbsF(-0.5f, 0.5f, 1.e-10f, 1.e-10f));
  assert(Math::AlmostEqualRelativeAndAbsF(0.5f, 0.5000001f, 1e-9f, 1.e-5f));

  assert(Math::AlmostEqualRelativeAndAbsD(0.5, 1. / 2., 1.e-15, 1.e-15));
  assert(!Math::AlmostEqualRelativeAndAbsD(0.5, -0.5, 1.e-15, 1.e-15));
  assert(!Math::AlmostEqualRelativeAndAbsD(-0.5, 0.5, 1.e-15, 1.e-15));
  assert(Math::AlmostEqualRelativeAndAbsD(0.5, 0.5000001, 1e-9, 1.e-5));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
