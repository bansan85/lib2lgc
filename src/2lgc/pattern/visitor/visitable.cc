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

/** \namespace llgc::pattern::visitor
 * \brief This namespace is used has base for every class that a visitor would
 *        like to visit.
 *
 *
 * \class llgc::pattern::visitor::BaseVisitable
 * \brief Common class to allow a class to be visited. This class is used to
 *        force the implementation of the Accept method with the right
 *        arguments.
 * \tparam Visitable The same name of the class that is visitable.
 *
 *
 * \fn llgc::pattern::visitor::BaseVisitable::BaseVisitable();
 * \brief Default constructor.
 *
 *
 * \fn llgc::pattern::visitor::BaseVisitable::~BaseVisitable()
 * \brief In case of some class based on it need a virtual destructor.
 *
 *
 * \fn llgc::pattern::visitor::BaseVisitable::BaseVisitable(BaseVisitable &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::visitor::BaseVisitable::BaseVisitable(BaseVisitable const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn BaseVisitable & llgc::pattern::visitor::BaseVisitable::operator=(BaseVisitable &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn BaseVisitable & llgc::pattern::visitor::BaseVisitable::operator=(BaseVisitable const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn bool llgc::pattern::visitor::BaseVisitable::Accept(const BaseVisitor &visitor, std::string *return_value) const
 * \brief This method is used to call the right Visit method on the visitor.
 * \param visitor The visitor that will used the data of the class that
 *        implement this class.
 * \param return_value The return value from the visitor from SerializeToString.
 * \return false if there is a problem or if SerializeToString failed.
 *         true instead.
 */

/** \class llgc::pattern::visitor::InterfaceVisitable
 * \brief Common class to implement interface visitable. This class only have
 *        the pure virtual methods Accept.
 * \tparam M The class that contains the data. It must be a ProtoBuf message.
 *
 *
 * \fn llgc::pattern::visitor::InterfaceVisitable::InterfaceVisitable();
 * \brief Default constructor.
 *
 *
 * \fn llgc::pattern::visitor::InterfaceVisitable::~InterfaceVisitable()
 * \brief In case of some class based on it need a virtual destructor.
 *
 *
 * \fn llgc::pattern::visitor::InterfaceVisitable::InterfaceVisitable(InterfaceVisitable &&other)
 * \brief Delete move constructor.
 * \param[in] other The original.
 *
 *
 * \fn llgc::pattern::visitor::InterfaceVisitable::InterfaceVisitable(InterfaceVisitable const &other)
 * \brief Delete copy constructor.
 * \param[in] other The original.
 *
 *
 * \fn InterfaceVisitable & llgc::pattern::visitor::InterfaceVisitable::operator=(InterfaceVisitable &&other)
 * \brief Delete the move operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn InterfaceVisitable & llgc::pattern::visitor::InterfaceVisitable::operator=(InterfaceVisitable const &other)
 * \brief Delete the copy operator.
 * \param[in] other The original.
 * \return Delete function.
 *
 *
 * \fn bool llgc::pattern::visitor::InterfaceVisitable::Accept(const BaseVisitor &visitor, std::string *return_value) const
 * \brief This method is used to call the right Visit method on the visitor.
 * \param visitor The visitor that will used the data of the class that
 *        implement this class.
 * \param return_value The return value from the visitor from SerializeToString.
 *        The value passed must be allocated on the stack or on the pile.
 * \return false if there is a problem or if SerializeToString failed.
 *         true instead.
 *
 *
 * \fn const M & llgc::pattern::visitor::InterfaceVisitable::Message() const
 * \brief return a const reference of the data so the visitor may use it.
 * \return The message in read-only format.
 *
 *
 * \fn M & llgc::pattern::visitor::InterfaceVisitable::Message()
 * \brief return a modifiable reference of the data so the visitor may modify
 *        it.
 * \return The message in read-write format.
 *
 *
 * \var llgc::pattern::visitor::InterfaceVisitable::message_
 * \brief The raw data.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
