/* Copyright [2017] LE GARREC Vincent
 *
 * This file is part of 2LGC.
 *
 * 2LGC is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * 2LGC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with 2LGC. If not, see <http://www.gnu.org/licenses/>.
 */

#include "subscriber_direct.h"

bool pattern::publisher::SubscriberDirect::Equals(
    const SubscriberInterface *connector) const {
  const SubscriberDirect *subscriber_direct_cast =
      dynamic_cast<const SubscriberDirect *>(connector);

  // Not the same type.
  if (subscriber_direct_cast == nullptr) {
    return false;
  }

  return subscriber_direct_cast->id_ == id_;
}
