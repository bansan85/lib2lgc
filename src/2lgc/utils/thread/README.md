# Thread

Various functions to ease manipulation of threads.

## CountLock

It's a extension of std::recursive_mutex. When the mutex count ref is zero, a function will be called on destructor.

You will need a recursive_mutex and a ref counter for the constructor.
