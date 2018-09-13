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

/** \fn llgc::pattern::Strategy::~Strategy()
 * \brief Virtual destructor because operator*() is virtual.
 *
 *
 * \fn llgc::pattern::Strategy::Strategy(Strategy&& other)
 * \brief Delete move constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn llgc::pattern::Strategy::Strategy(Strategy const& other)
 * \brief Delete copy constructor.
 * \param[in] other Don't care.
 *
 *
 * \fn Strategy& llgc::pattern::Strategy::operator=(Strategy&& other)
 * \brief Delete move operator.
 * \param[in] other Don't care.
 * \return Nothing.
 *
 *
 * \fn Strategy& llgc::pattern::Strategy::operator=(Strategy const& other)
 * \brief Delete copy operator.
 * \param[in] other Don't care.
 * \return Nothing.
 */

/** \fn bool llgc::pattern::Strategy::Do()
 * \brief Execute the command. No argument possible. If argument needs, pass it to the children's constructor.
 * \return true if function success.
 *
 *
 * \var llgc::pattern::Strategy::instance_
 * \brief Instance to work with.
 */

#endif  // PATTERN_STRATEGY_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
