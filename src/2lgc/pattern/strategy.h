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

#ifndef PATTERN_STRATEGY_H_
#define PATTERN_STRATEGY_H_

#include <2lgc/compat.h>
// TEMPLATE_CLASS needs it.
#include <2lgc/config.h>  // IWYU pragma: keep

namespace llgc::pattern
{
template <class T>
class Strategy
{
 public:
  Strategy();
  explicit Strategy(T *instance);
  virtual bool Do() CHK = 0;

 protected:
  T *instance_;
};

}  // namespace llgc::pattern

#ifndef TEMPLATE_CLASS
#include <2lgc/pattern/strategy.cc>
#endif

#endif  // PATTERN_STRATEGY_H_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
