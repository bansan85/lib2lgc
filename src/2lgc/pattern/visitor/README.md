# Introduction

The goal of this library is to implement the visitor pattern.

The visitable classes are classes that may be extended.

The visitor classes are classes that extend visitable classes.

The two classes were designed to be very easy to used by other class.

# Design

## Raw data

All data are stored in `ProtoBuf`'s Google messages.

If different data represents the same object (e.g. cartesien / polar
coordonate), they will be in the same `ProtoBuf` message with a `oneof` data.

```
syntax = "proto3";

package msg;

message Coordonate {
  // Cartesien coordonate
  message Cartesien {
    float x = 1;
    float y = 2;
  }

  // Polar coordonate
  message Polar {
    float r = 1;
    float angle = 2;
  }

  oneof data {
    Cartesien  cartesien = 1;
    Polar      number_op_number = 2;
  }
}
```

## Container / Interface

If different data represents the same object, an interface with pure virtual
method may be used.

The interface must inheritate the `InterfaceVisitable` class.

This class must have a public constructor without parameter
a virtual destructor and for each method that will visit the object :
  - a pure const virtual fonction `GetXxx`,
  - a static class that will visit all raw data,
  - eventually a mutable variable that will be used as cache.

```
class Coordonate : public InterfaceVisitable<msg::Coordonate> {
 public:
  Coordonate()
#ifndef DISABLE_VISITABLE_CACHE
      : cache_x_(std::nan("")),
        cache_y_(std::nan(""))
#endif  // DISABLE_VISITABLE_CACHE
  {
  }
  virtual ~Coordonate() {}

  virtual double GetX() const = 0;
  virtual double GetY() const = 0;

 protected:
  static CoordonateVisitorX visitor_x;
  static CoordonateVisitorY visitor_y;

#ifndef DISABLE_VISITABLE_CACHE
  mutable double cache_x_;
  mutable double cache_y_;
#endif  // DISABLE_VISITABLE_CACHE
}
```

## Visitors

All visitors must have a virtual destructor and inheritate :
  - the empty class `BaseVisitor ` so all classes will be able to be cast to
this common class.
  - the class `Visitor<XXX>` for each class in the same `ProtoBuf` message.

The `Visit` method takes two arguments, at first a reference to the data to
visit, then a pointer to a `string` (the string must be allocated on the stack
or on the pile).

If `Visit` success, the `return_value` will be decoded with `ParseFromString`.

Inside the `Visit` method, the first argument of `Accept` method is `*this`.

```
class Coordonate_Coordonate;
class Coordonate_Polar;

class CoordonateVisitorX : public BaseVisitor,
                           public Visitor<Coordonate_Coordonate>,
                           public Visitor<Coordonate_Polar> {
 public:
  virtual ~CoordonateVisitorX() {}
  bool Visit(const Coordonate_Coordonate &data,
             std::string *return_value) const override CHK;
  bool Visit(const Coordonate_Polar &data,
             std::string *return_value) const override CHK;
};
```
