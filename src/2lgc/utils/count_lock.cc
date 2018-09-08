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

/** \namespace llgc::utils::thread
 * \brief Namespace for all classes related with thread.
 *
 *
 * \class llgc::utils::thread::CountLock
 * \brief Class that increment variable with constructor and decrease with
 *        destrctor.
 *
 *
 * \fn llgc::utils::thread::CountLock::CountLock(M* const ref, std::recursive_mutex* mutex_forward, std::function<void()> function_zero)
 * \brief Default constructor.
 * \param[in] ref Pointer to the ref variable that will be increment on
 *            constructor and decrement on destructor.
 * \param[in] mutex_forward Pointer to the mutex that will be use to use
 *            thread-safe ref.
 * \param[in] function_zero Function that be be called if on destructor, ref
 *            value is zero.
 *
 *
 * \fn llgc::utils::thread::CountLock::~CountLock()
 * \brief Default destructor.
 */

// gcc-6.4 needs default.
// Move constructor must be noexcept but default doesn't support it.
/** \fn llgc::utils::thread::CountLock::CountLock(CountLock&& other)
 * \brief Move constructor with noexcept.
 * \param[in] other The original.
 *
 *
 * \fn llgc::utils::thread::CountLock::CountLock(CountLock const& other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn CountLock& llgc::utils::thread::CountLock::operator=(CountLock&& other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn CountLock& llgc::utils::thread::CountLock::operator=(CountLock const& other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \var llgc::utils::thread::CountLock::ref_
 * \brief Options for the behavious of server.
 *
 *
 * \var llgc::utils::thread::CountLock::mutex_forward_
 * \brief The mutex to lock ref.
 *
 *
 * \var llgc::utils::thread::CountLock::function_zero_
 * \brief Function to execute if a destruction, the ref is zero.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
