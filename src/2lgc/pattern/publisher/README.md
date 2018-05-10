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

## Functions

Three functions will be used:
  - AddSubscriber that allow a subscriber to receive message with id.
  - RemoveSubscriber to remove a subscriber from an id.
  - Send to send (protobuf) messages to the publisher.

## Messages

All messages are serialized by protobuf library. You can send multiples messages at same time.

## Limitations

The protobuf message after serialize cannot be bigger than 1500 bytes, the size of a TCP frame.

# Declaration of publisher, subscriber and connector

## Protobuf messages

```
// Protobuf 3 is a need.
syntax = "proto3";

// Use the pattern_publisher.proto to support TCP publisher.
import "pattern_publisher.proto";

// By internal convention, all protobuf are in package llgc.protobuf but it possible to not use it.
package llgc.protobuf.test;

// Atomic message.
message Tcp
{
  // By convention, keep Msg for name
  message Msg
  {
    // All atomic actions of a publisher must be in a sub-message.
    message Test
    {
    }

    // Then all possible atomic actions must be in a union.
    oneof data
    {
      // These two lines must be keeped unchanged to support TCP publisher.
      // You can use the id you want.
      llgc.protobuf.pattern.publisher.AddSubscriber add_subscriber = 1;
      llgc.protobuf.pattern.publisher.net.RemoveSubscriber remove_subscriber = 2;
      // Then add all atomic message here.
      Test test = 3;
    }
  }

  // Message send to publisher. It's a list of atomic messages.
  // The name of a list must be msg.
  repeated Msg msg = 1;
}
```

## In direct mode

### Publisher

Use the protobuf message class for the typename.

Header:

```
#include <2lgc/pattern/publisher/publisher_interface.h>
#include <2lgc/pattern/publisher/publisher_direct.h>

#include <2lgc/pattern/publisher/publisher_interface.cc>

// Here, you need to use std::weak_ptr for internal use.
template class llgc::pattern::publisher::PublisherInterface<
    llgc::protobuf::test::Direct,
    std::weak_ptr<llgc::pattern::publisher::ConnectorInterface<
        llgc::protobuf::test::Direct>>>;
```

Declaration:

```
  auto server = std::make_shared<llgc::pattern::publisher::PublisherDirect<
      llgc::protobuf::test::Direct>>();
```

### Subscriber

Header:

```
#include <2lgc/pattern/publisher/subscriber_direct.h>

#include <2lgc/pattern/publisher/publisher_interface.cc>

template class llgc::pattern::publisher::SubscriberDirect<
    llgc::protobuf::test::Direct>;
```

Declaration with SubscriberBase based on SubscriberDirect:

```
  auto subscriber = std::make_shared<SubscriberBase>(1);
```

You must implement the subscriber with the Listen function.

In this example, a function is create for each action. They are stored in a vector and the good index in found with the function `data_case()`.

```
class SubscriberBase final : public llgc::pattern::publisher::SubscriberDirect<
                                 llgc::protobuf::test::Direct>
{
 public:
  // The id is used to compare subscriber by the publisher.
  explicit SubscriberBase(uint32_t id)
      : SubscriberDirect(id),
        // Here, the kTest has the highest id.
        message_vector(llgc::protobuf::test::Direct_Msg::DataCase::kTest + 1)
  {
    message_vector[0] = nullptr;
    // Attribute a function for each message id.
    message_vector[llgc::protobuf::test::Direct_Msg::DataCase::kTest] =
        &SubscriberBase::TestFct;
  }

  bool Listen(const llgc::protobuf::test::Direct& messages) override
  { 
    for (int i = 0; i < message.action_size(); i++)
    {
      const llgc::protobuf::test::Direct_Msg& message = messages.msg(i);
      
      // Here you supposed that all index of the message_vector are filled and
      // not nullptr.
      message_vector[message.data_case()](*this, message);
    }
    return true;
  }
  
  void TestFct(const msg::Action& action)
  {
    // Do what you want here.
  }

  std::vector<std::function<void(SubscriberBase&,
                                 const llgc::protobuf::test::Direct_Msg&)>>
      message_vector;
};

```

### Connector

Header:

```
#include <2lgc/pattern/publisher/connector_direct.h>
#include <2lgc/pattern/publisher/connector_interface.h>

#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>

template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Direct>;
template class llgc::pattern::publisher::ConnectorDirect<
    llgc::protobuf::test::Direct>;
```

Declaration:

```
  auto connector = std::make_shared<
      llgc::pattern::publisher::ConnectorDirect<llgc::protobuf::test::Direct>>(
      subscriber, server);
```


## In TCP mode

### Publisher

Header:

```
#include <2lgc/pattern/publisher/publisher_tcp.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux.h>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h>

#include <2lgc/pattern/publisher/publisher_interface.cc>
#include <2lgc/pattern/publisher/publisher_tcp.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux.cc>
#include <2lgc/pattern/publisher/publisher_tcp_linux_ipv4.cc>

template class llgc::pattern::publisher::PublisherTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcpLinux<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::PublisherTcpLinuxIpv4<
    llgc::protobuf::test::Tcp>;
```

Declaration:

```
  auto server =
      std::make_shared<llgc::pattern::publisher::PublisherTcpLinuxIpv6<
          llgc::protobuf::test::Tcp>>(8889);
```

### Subscriber

Header:

```
#include <2lgc/pattern/publisher/subscriber_direct.h>
#include <2lgc/pattern/publisher/subscriber_server_tcp.h>

#include <2lgc/pattern/publisher/subscriber_direct.cc>
#include <2lgc/pattern/publisher/subscriber_server_tcp.cc>

template class llgc::pattern::publisher::SubscriberDirect<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::SubscriberServerTcp<
    llgc::protobuf::test::Tcp>;
```

Declaration:

```
  auto subscriber = std::make_shared<SubscriberBase>(1);
```

You must implement the subscriber with the Listen function. See Direct mode for an example.

### Connector

Header:

```
#include <2lgc/pattern/publisher/connector_subscriber_tcp.h>
#include <2lgc/pattern/publisher/connector_interface.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp.h>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h>

#include <2lgc/pattern/publisher/connector_subscriber_tcp.cc>
#include <2lgc/pattern/publisher/connector_direct.cc>
#include <2lgc/pattern/publisher/connector_interface.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp.cc>
#include <2lgc/pattern/publisher/connector_publisher_tcp_ipv4.cc>

template class llgc::pattern::publisher::ConnectorInterface<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcp<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorPublisherTcpIpv6<
    llgc::protobuf::test::Tcp>;
template class llgc::pattern::publisher::ConnectorSubscriberTcp<
    llgc::protobuf::test::Tcp>;
```

Declaration:

```
  auto connector =
      std::make_shared<llgc::pattern::publisher::ConnectorPublisherTcpIpv6<
          llgc::protobuf::test::Tcp>>(subscriber, "::1", 8889);
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
  connector->AddSubscriber(llgc::protobuf::test::Tcp_Msg::DataCase::kTest);
```

Then send protobuf message.

```
  llgc::protobuf::test::Tcp messages;
  auto message = messages.add_msg();
  auto message_test = std::make_unique<llgc::protobuf::test::Tcp_Msg_Test>();
  message->set_allocated_test(message_test.release());
  std::string messages_in_string;
  messages.SerializeToString(&messages_in_string);
  connector->Send(messages_in_string);
```
