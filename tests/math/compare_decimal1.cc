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

#include <2lgc/math/compare_decimal.h>
#include <google/protobuf/stubs/common.h>
#include <cassert>

int main(int /* argc */, char * /* argv */ [])  // NS
{
  assert(
      llgc::math::Compare::AlmostEqualUlpsAndAbsF(0.5f, 1.f / 2.f, 1.e-10f, 4));
  assert(!llgc::math::Compare::AlmostEqualUlpsAndAbsF(0.5f, -0.5f, 1.e-10f, 4));
  assert(!llgc::math::Compare::AlmostEqualUlpsAndAbsF(-0.5f, 0.5f, 1.e-10f, 4));
  // Ulp: 2
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsF(0.5f, 0.5000001f, 0.f, 4));

  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(0.5, 1. / 2., 1.e-15, 4));
  assert(!llgc::math::Compare::AlmostEqualUlpsAndAbsD(0.5, -0.5, 1.e-15, 4));
  assert(!llgc::math::Compare::AlmostEqualUlpsAndAbsD(-0.5, 0.5, 1.e-15, 4));
  // Ulp: 1
  assert(llgc::math::Compare::AlmostEqualUlpsAndAbsD(0.5, 0.5000000000000001,
                                                     0., 5));

  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsF(0.5f, 1.f / 2.f,
                                                         1.e-10f, 1.e-10f));
  assert(!llgc::math::Compare::AlmostEqualRelativeAndAbsF(0.5f, -0.5f, 1.e-10f,
                                                          1.e-10f));
  assert(!llgc::math::Compare::AlmostEqualRelativeAndAbsF(-0.5f, 0.5f, 1.e-10f,
                                                          1.e-10f));
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsF(0.5f, 0.5000001f,
                                                         1e-9f, 1.e-5f));

  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(0.5, 1. / 2., 1.e-15,
                                                         1.e-15));
  assert(!llgc::math::Compare::AlmostEqualRelativeAndAbsD(0.5, -0.5, 1.e-15,
                                                          1.e-15));
  assert(!llgc::math::Compare::AlmostEqualRelativeAndAbsD(-0.5, 0.5, 1.e-15,
                                                          1.e-15));
  assert(llgc::math::Compare::AlmostEqualRelativeAndAbsD(0.5, 0.5000001, 1e-9,
                                                         1.e-5));

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
