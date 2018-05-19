# Abstract factory

## Design

The abstract factory convert a serialize message to a command.

So you will need:
  - a protobuf class that will convert serialize message from std::string to a class. The main message must be a union of all different simple message.
  - a common interface will all pure virtual function that all commands must implement,
  - one function for each simple message.

And the factory returns a std::unique_ptr with the command instantiated casted to the common interface.

## Example

### The protobuf message

```
// By convention, all package must be in llgc.protobuf.
package llgc.protobuf.test;

// The global message that hold all simple message.
message AbstractFactoryMsg
{
  // One simple message for each possible command.
  message Test
  {
  }

  // A union that contains all simple message. Name must be data.
  oneof data
  {
    Test test = 1;
  }
}
```

### Command

The interface:

```
class CommonCommand
{
 public:
  virtual void Method1() = 0;
  virtual bool Method2(size_t param) = 0;
};
```

One simple command:

```
class CommandTest : public CommonCommand
{
 public:
  void Method1() override {}
  bool Method2(size_t param) override { return true; }
};
```

### The factory

```
// The factory with as template the protobuf message and interface command.
class Factory
    : public llgc::pattern::abstract_factory::AbstractFactoryInterface<
          llgc::protobuf::test::AbstractFactoryMsg, CommonCommand>
{
 public:
  Factory()
      : AbstractFactoryInterface(
      // The number of the last id message + 1.
            llgc::protobuf::test::AbstractFactoryMsg::kTest + 1)
  {
    // Add to the factory at index "Test" a function that return
    // std::unique_ptr<CommonCommand> and that have one param the global
    // protobuf message.
    map_factory_[llgc::protobuf::test::AbstractFactoryMsg::kTest] =
        std::bind(&Factory::GetTest, this, std::placeholders::_1);
  }

 private:
  // A function that convert a protobuf message with Test data to the
  // command interface class with Test implementation.
  std::unique_ptr<CommonCommand> GetTest(
      const llgc::protobuf::test::AbstractFactoryMsg& message)
  {
    // Be sure that message has test data.
    assert(message.has_test());

    // Create the command test.
    auto retval = std::make_unique<CommandTest>();
  
    // And return it.
    return std::move(retval);
  }
};
```

### Usage

```
// The abstract factory.
Factory factory;

auto command = factory.Create(message_in_string);

// if command == nullptr then unknown command.
command->Method1();
```
