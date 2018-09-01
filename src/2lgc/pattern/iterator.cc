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

/** \file iterator.h
 * \brief Interface for the ranged-base loop iterator.
 *
 *
 * \class llgc::pattern::Iterator
 * \brief Helper class to implement range-based for loop.
 * \tparam T Class that will be loop.
 * \tparam U Object that will be return by the loop.
 *
 *
 * \fn llgc::pattern::Iterator::Iterator(const T& data, size_t pos)
 * \brief The constructor
 * \param[in] data The data contains the list of object.
 * \param[in] pos The position of the current object.
 *
 *
 * \fn llgc::pattern::Iterator::Iterator(Iterator&& other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::Iterator::Iterator(Iterator const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn Iterator& llgc::pattern::Iterator::operator=(Iterator&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn Iterator& llgc::pattern::Iterator::operator=(Iterator const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn llgc::pattern::Iterator::~Iterator()
 * \brief Default destructor.
 *
 *
 * \fn bool llgc::pattern::Iterator::operator!=(const Iterator& other) const
 * \brief Compare operator.
 * \param[in] other The iterator to compare with.
 * \return true if different.
 *
 *
 * \fn const U& llgc::pattern::Iterator::operator*() const
 * \brief Dereference an iterator return the current object.
 * \return Return the current object.
 *
 *
 * \fn const Iterator& llgc::pattern::Iterator::operator++()
 * \brief Increment the current object.
 * \return Himself.
 *
 *
 * \var llgc::pattern::Iterator::data_
 * \brief This data contains the list of object.
 *
 *
 * \var llgc::pattern::Iterator::pos_
 * \brief The position of the current object.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
