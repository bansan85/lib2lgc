# Publisher pattern

Here is implemented the subscriber / publisher pattern.

The subscriber and the publisher can be in the same application, in direct connection mode.

But you can use it with TCP (IPv4 and IPv6).

# Design

## Classes

Three classes are needed:
  - the subscriber. It can only listen messages from the publisher by the intermediaire of a connector. It can not send messages.
  - the publisher. The server that receive messages and forward them to subscribers.
  - the connector. It's an intermediaire class that can send messages to publisehr and listen messages from publisher to subscriber. A connector can handle only one subscriber.

The subscriber could have been merged but in this way, the connector is a layer to handle communication and the subscriber, which is just an interface, is a layer to execute functions based on messages received from publisher.

## Messages

All messages are serialized by protobuf library. You can send multiples messages at same time.

## Limitations

The protobuf message after serialize cannot be bigger than 1500 bytes, the size of a TCP frame.

# Declaration of publisher, subscriber and connector

## Protobuf messages

```
// Protobuf 3 is a need.
syntax = "proto3";

// Use the net.proto to support TCP publisher.
import "net.proto";

// By internal convention, all protobuf are in package msg but it possible to not use it.
package msg;

// Atomic ation.
message ActionTcp {
  // All atomic actions of a publisher must be in a sub-message.
  message Test {
  }

  // Then all possible atomic actions must be in a union.
  oneof data {
    // These two lines must be keeped unchanged to support TCP publisher.
    // You can use the id you want.
    msg.net.AddSubscriber add_subscriber = 1;
    msg.net.RemoveSubscriber remove_subscriber = 2;
    // Then add all atomic action here.
    Test test = 3;
  }
}

// Message send to publisher. It's a list of atomic actions.
// The name of a list must be action.
message ActionsTcp {
  repeated ActionTcp action = 1;
}
```

## In direct mode

### Publisher

Use the protobuf message class for the typename.

Header:

```
#include <2lgc/pattern/publisher/publisher.h>
#include <2lgc/pattern/publisher/publisher_direct.h>

#include <2lgc/pattern/publisher/publisher.cc>

// Here, you need to use std::weak_ptr for internal use.
template class llgc::pattern::publisher::Publisher<
    msg::Actions,
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<msg::Actions>>>;
```

Declaration:

```
  std::shared_ptr<llgc::pattern::publisher::PublisherDirect<msg::Actions>>
      server = std::make_shared<
          llgc::pattern::publisher::PublisherDirect<msg::Actions>>();
```

### Subscriber

Header:

```
#include <2lgc/pattern/publisher/subscriber_direct.h>

#include <2lgc/pattern/publisher/publisher.cc>

template class llgc::pattern::publisher::SubscriberDirect<msg::Actions>;
```

Declaration:

```
  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);
```

You must implement the subscriber with the Listen function.

In this example, a function is create for each action. They are stored in a vector and the good index in found with the function `data_case()`.

```
class SubscriberBase final
    : public llgc::pattern::publisher::SubscriberDirect<msg::Actions>
{
 public:
  // The id is used to compare subscriber by the publisher.
  explicit SubscriberBase(uint32_t id)
      : SubscriberDirect(id),
        // Here, the kTest have the highest id.
        action_vector(msg::Action::DataCase::kTest + 1)
  {
    action_vector[0] = nullptr;
    // Attribute a function for each message id.
    action_vector[msg::Action::DataCase::kTest] = &SubscriberBase::TestFct;
  }

  bool Listen(const msg::Actions& message) override
  { 
    for (int i = 0; i < message.action_size(); i++)
    {
      const msg::Action& action = message.action(i);
      
      // Here you supposed that all index of the action_vector are filled and
      // not nullptr.
      action_vector[action.data_case()](*this, action);
    }
    return true;
  }
  
  void TestFct(const msg::Action& action)
  {
    // Do what you want here.
  }
  
  std::vector<std::function<void(SubscriberBase&, const msg::Action&)>>
      action_vector;
};

```

### Connector

Header:

```
#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>

#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>

template class llgc::pattern::publisher::ConnectorInterface<msg::Actions>;
template class llgc::pattern::publisher::ConnectorDirect<msg::Actions>;
```

Declaration:

```
  std::shared_ptr<llgc::pattern::publisher::ConnectorDirect<msg::Actions>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorDirect<msg::Actions>>(subscriber,
                                                                   server);
```


## In TCP mode

### Publisher

Header:

```
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>

#include <2lgc/pattern/publisher/publisher.cc>
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc>

template class llgc::pattern::publisher::PublisherTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::PublisherTcpLinux<msg::ActionsTcp>;
template class llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>;
```

Declaration:

```
  std::shared_ptr<
      llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>>
      server = std::make_shared<
          llgc::pattern::publisher::PublisherTcpLinuxIpv4<msg::ActionsTcp>>(
          8888);
```

### Subscriber

Header:

```
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>

#include <2lgc/pattern/publisher/subscriber_direct.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>

template class llgc::pattern::publisher::SubscriberDirect<msg::ActionsTcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<msg::ActionsTcp>;
```

Declaration:

```
  std::shared_ptr<SubscriberBase> subscriber =
      std::make_shared<SubscriberBase>(1);
```

You must implement the subscriber with the Listen function. See Direct mode for an example.

### Connector

Header:

```
#include <2lgc/pattern/publisher/connector_client_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_server_tcp.h>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.h>

#include <2lgc/pattern/publisher/connector_client_tcp.cc>
#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_server_tcp.cc>
#include <2lgc/pattern/publisher/connector_server_tcp_ipv4.cc>

template class llgc::pattern::publisher::ConnectorInterface<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcp<msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorServerTcpIpv4<
    msg::ActionsTcp>;
template class llgc::pattern::publisher::ConnectorClientTcp<msg::ActionsTcp>;
```

Declaration:

```
  std::shared_ptr<
      llgc::pattern::publisher::ConnectorServerTcpIpv4<msg::ActionsTcp>>
      connector = std::make_shared<
          llgc::pattern::publisher::ConnectorServerTcpIpv4<msg::ActionsTcp>>(
          subscriber, "127.0.0.1", 8888);
```

# Usage

## Publisher

In direct mode, you don't have anything to do.

In TCP mode, you need to open the port with Listen and to wait for message.

```
server->Listen();
server->Wait();
```

and before closing the application or to stop the server.

```
server->Stop();
server->JoinWait();
```

## Subscriber

You don't need to do something.

## Connector

The usage is the same for direct mode and TCP mode.

First add subscriber with id of the message.

```
connector->AddSubscriber(msg::Action::DataCase::kTest)
```

Then send protobuf message.

```
msg::ActionsTcp actions = msg::ActionsTcp();
msg::ActionTcp* action = actions.add_action();
std::unique_ptr<msg::ActionTcp_Test> action_test =
    std::make_unique<msg::ActionTcp_Test>();
action->set_allocated_test(action_test.release());
std::string action_in_string;
actions.SerializeToString(&action_in_string);
connector->Send(action_in_string);
```
