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

#ifndef UTILS_UNDOMANAGER_UNDOMANAGER_CC_
#define UTILS_UNDOMANAGER_UNDOMANAGER_CC_

#include <2lgc/compat.h>
#include <2lgc/error/show.h>
// node_parent->GetData needs it.
#include <2lgc/utils/tree.h>  // IWYU pragma: keep
#include <2lgc/utils/undomanager/undomanager.h>
#include <deque>
#include <iostream>

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::undomanager::Undomanager<T, U>::Undomanager(
    std::unique_ptr<llgc::pattern::AbstractFactory<T, U>> abstract_factory)
    : memory_(), abstract_factory_(std::move(abstract_factory))
{
}

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::undomanager::Undomanager<T, U>::Undomanager(
    std::unique_ptr<llgc::pattern::AbstractFactory<T, U>> abstract_factory,
    const std::string &file)
    : Undomanager(std::move(abstract_factory))
{
}

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::Tree<U>
    *llgc::utils::undomanager::Undomanager<T, U>::AddCommand(
        const std::string &command)
{
  std::unique_ptr<U> new_command = abstract_factory_->Create(command);

  BUGCRIT(std::cout, new_command != nullptr, nullptr,
          "Failed to decode command " + command + ".\n");

  Tree<U> *retval = Add(std::move(new_command));
  BUGCONT(std::cout, retval != nullptr, nullptr);

  return retval;
}

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::Tree<U>
    *llgc::utils::undomanager::Undomanager<T, U>::AddCommand(
        size_t id, const std::string &command)
{
  auto new_command = abstract_factory_->Create(command);

  BUGCRIT(std::cout, new_command != nullptr, nullptr,
          "Failed to decode command " + command + ".\n");

  Tree<U> *retval = Add(id, std::move(new_command));
  BUGCONT(std::cout, retval != nullptr, nullptr);

  return retval;
}

template <typename T, typename U>
INLINE_TEMPLATE std::vector<std::string>
llgc::utils::undomanager::Undomanager<T, U>::DuplicateCommand(size_t start,
                                                              size_t end,
                                                              size_t insert)
{
  return std::vector<std::string>();
}

template <typename T, typename U>
INLINE_TEMPLATE bool llgc::utils::undomanager::Undomanager<T, U>::DoCommand(
    size_t id)
{
  BUGCRIT(std::cout, memory_ != nullptr, false, "Undomanager empty.\n");

  Tree<U> *node_parent = memory_->FindNode(id);

  BUGCRIT(std::cout, node_parent != nullptr, false,
          "Failed to find node " + std::to_string(id) + ".\n");
  BUGCONT(std::cout, node_parent->GetData()->Do(), false);

  return true;
}

template <typename T, typename U>
INLINE_TEMPLATE bool llgc::utils::undomanager::Undomanager<T, U>::DoCommands(
    size_t start, size_t end)
{
  BUGCRIT(std::cout, memory_ != nullptr, false, "Undomanager empty.\n");

  std::deque<U *> path;

  BUGCRIT(std::cout, memory_->FindPath(start, end, &path), false,
          "Fails to find path between " + std::to_string(start) + " and " +
              std::to_string(end) + ".\n");

  size_t i;
  bool retval = true;
  for (i = 0; i < path.size(); i++)
  {
    if (!path[i]->Do())
    {
      retval = false;
      break;
    }
  }

  if (!retval)
  {
    for (size_t j = i; i > 0; i--)
    {
      BUGCRIT(std::cout, path[j - 1]->Undo(), false,
              "Fails to rollback " + path[j - 1]->GetDescription() +
                  ". First fails with " + path[i]->GetDescription() +
                  " to do.\n");
    }
    BUGCRIT(std::cout, false, false,
            "Fails to do from " + path.front()->GetDescription() + " to " +
                path.back()->GetDescription() +
                " but rollback with success.\n");
  }

  return true;
}

template <typename T, typename U>
INLINE_TEMPLATE bool llgc::utils::undomanager::Undomanager<T, U>::UndoCommand(
    size_t id)
{
  BUGCRIT(std::cout, memory_ != nullptr, false, "Undomanager empty.\n");

  Tree<U> *node_parent = memory_->FindNode(id);

  BUGCRIT(std::cout, node_parent != nullptr, false,
          "Failed to find node " + std::to_string(id) + ".\n");
  BUGCONT(std::cout, node_parent->GetData()->Undo(), false);

  return true;
}

template <typename T, typename U>
INLINE_TEMPLATE bool llgc::utils::undomanager::Undomanager<T, U>::UndoCommands(
    size_t start, size_t end)
{
  BUGCRIT(std::cout, memory_ != nullptr, false, "Undomanager empty.\n");

  std::deque<U *> path;

  BUGCRIT(std::cout, memory_->FindPath(start, end, &path), false,
          "Fails to find path between " + std::to_string(start) + " and " +
              std::to_string(end) + ".\n");

  size_t i;
  bool retval = true;
  for (i = 0; i < path.size(); i++)
  {
    if (!path[i]->Undo())
    {
      retval = false;
      break;
    }
  }

  if (!retval)
  {
    for (size_t j = i; i > 0; i--)
    {
      BUGCRIT(std::cout, path[j - 1]->Do(), false,
              "Fails to rollback " + path[j - 1]->GetDescription() +
                  ". First fails with " + path[i]->GetDescription() +
                  " to undo.\n");
    }
    BUGCRIT(std::cout, false, false,
            "Fails to undo from " + path.front()->GetDescription() + " to " +
                path.back()->GetDescription() +
                " but rollback with success.\n");
  }

  return true;
}

template <typename T, typename U>
INLINE_TEMPLATE void llgc::utils::undomanager::Undomanager<T, U>::DrawHistory(
    void *before, void *after)
{
}

template <typename T, typename U>
INLINE_TEMPLATE std::vector<size_t>
llgc::utils::undomanager::Undomanager<T, U>::GetType() const
{
  return std::vector<size_t>();
}

template <typename T, typename U>
INLINE_TEMPLATE std::vector<T>
llgc::utils::undomanager::Undomanager<T, U>::FindByZone(int i) const
{
  return std::vector<T>();
}

template <typename T, typename U>
INLINE_TEMPLATE bool
llgc::utils::undomanager::Undomanager<T, U>::BeginNewCommand(size_t id)
{
  return false;
}

template <typename T, typename U>
INLINE_TEMPLATE bool
llgc::utils::undomanager::Undomanager<T, U>::EndNewCommand()
{
  return false;
}

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::Tree<U>
    *llgc::utils::undomanager::Undomanager<T, U>::Add(std::unique_ptr<U> child)
{
  if (memory_ == nullptr)
  {
    memory_ =
        std::make_unique<llgc::utils::Tree<U>>(0, std::move(child), nullptr);
    return memory_.get();
  }

  Tree<U> *retval = memory_->AddChild(std::move(child));
  BUGCONT(std::cout, retval != nullptr, nullptr);

  return retval;
}

template <typename T, typename U>
INLINE_TEMPLATE llgc::utils::Tree<U>
    *llgc::utils::undomanager::Undomanager<T, U>::Add(size_t id,
                                                      std::unique_ptr<U> child)
{
  BUGCRIT(std::cout, memory_ != nullptr, nullptr, "Undomanager empty.\n");

  Tree<U> *node_parent = memory_->FindNode(id);

  BUGCRIT(std::cout, node_parent != nullptr, nullptr,
          "Failed to find node " + std::to_string(id) + ".\n");

  Tree<U> *retval = node_parent->AddChild(std::move(child));
  BUGCONT(std::cout, retval != nullptr, nullptr);

  return retval;
}

#endif  // UTILS_UNDOMANAGER_UNDOMANAGER_CC_

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
