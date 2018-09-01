# Iterator pattern

The goal of this library is to implement the iterator pattern for the base-ranged for loop (`C++11`).

# Design
Two classes are needed: the class that will be looped and the class that contain objects that will be returned.

## Iterator
To implement this class, the template need to know the class that will be call from the loop and the class that the loop will return.

The custom iterator that will use this class will have access to two variables: `data_` (the object in the loop) and `pos_` (the current position in the container).

## Loop-class
The iterator's class will be stored in Object class.

### Container
The container can be what you want. You just need two informations:
the size (number of data) of the container and a function returning the nth element under reference.

For a `st::map` or a `std::set` that don't have a direct access, you can use a function like this:
```
  auto it = object_.begin();
  std::advance(it, i);
  return **it;
```

### Functions
In this loop-class you will need a container and two functions:

```
 public:
  Object::Iter begin() const
  {
    return Object::Iter(*this, 0);
  }

  Object::Iter end() const
  {
    return Object::Iter(*this, object_.size());
  }

 private:
  std::multiset<Object> object_;
```

## Object
Add the iterator in the class's object.

```
class Object
{
 public:
  class Iter : public llgc::pattern::iterator::Iterator<ClassLoop, Object>
  {
   public:
    Iter(const ClassLoop& class_loop, size_t pos)
        : llgc::pattern::iterator::Iterator<ClassLoop, Object>(class_loop, pos) { }
    
    const Object& operator*() const;
  }
}
```

And implement `operator*` in source file.

```
const Object& Object::Iter::operator*() const
{
  return data_.Get(pos_);
}
```
