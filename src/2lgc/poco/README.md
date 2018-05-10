# Poco

`POCO` means `Plain Old Class Object` and stores only raw data.

They are creating with protobuf. I decided to choose Protobuf because it supports serialization / deserialization and versioning.

# Example

For this example, have a look in number.proto.

## Protobuf

First, create the protobuf message. Remember that a protobuf class doesn't support inheritance. You have to do it with a `oneof`.

## Class interface

You are not allowed to use the protobuf message without an adapter.

```
class Number : public InterfaceVisitable<llgc::protobuf::Number>
{
 public:
  virtual double GetVal() const = 0;
  virtual llgc::protobuf::math::Number_Unit GetUnit() const = 0;
}
```

The class in template `InterfaceVisitable` must be a protobuf message.

You can use cache to reduce usage of (de/)serialization. But be careful so the cache is faster than without.

## Implementation of the class

```
class Number_Constant : virtual public Number,
                        virtual public BaseVisitable<Number_Constant, Number>
{
}
```

Here, you have to implement all pure abstract method from the interface.

You can you a visitor or a simple method.

