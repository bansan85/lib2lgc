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

#ifndef PATTERN_STRATEGY_CC_
#define PATTERN_STRATEGY_CC_

#include <2lgc/compat.h>
#include <2lgc/pattern/strategy.h>

/** \class llgc::pattern::Strategy
 * \brief Interface for strategy pattern.
 * \tparam T Class manipulated.
 */

/// \brief Default constructor for no instance.
template <class T>
INLINE_TEMPLATE llgc::pattern::Strategy<T>::Strategy() : instance_(nullptr)
{
}

/** \brief Constructor with instance.
 * \param[in] instance Instance to work with.
 */
template <class T>
INLINE_TEMPLATE llgc::pattern::Strategy<T>::Strategy(T *instance)
    : instance_(instance)
{
}

/** \fn llgc::pattern::Strategy::bool Do()
 * \brief Execute the command. No argument possible. If argument needs, pass it to the children's constructor.
 *
 *
 * \var llgc::pattern::Strategy::instance_
 * \brief Instance to work with.
 */

#endif  // PATTERN_STRATEGY_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
