<h1>Introduction</h1>

The goal of the singleton pattern is to be able to have only one instance of a class that will be shared by multiple threads.

<h1>Design</h1>

There's two interfaces, one for static class, one for instantiated object.

<h2>Common</h2>
Both have two functions and one public member.

`bool IsInstance();` is to know if the instance is already instanciated.

It's usefull if you need to call a function only if the object is instanciated.
For example, if the instance is a server, there's no need to allocate it if you want send message to all subscribers.

`std::shared_ptr<T> GetInstance();` is to get the shared instance and instanciate it if needed.

`std::recursive_mutex mutex_;` is to freeze usage of singleton.

<h2>Local</h2>

If you class is allocated and not static, use a `Local` class member.
The template needs to know the class of the instance.

<h2>Static</h2>

If you class is static and not allocated, use a `Static` class member.
The template needs to know the class that hold this static class and the class of the instance.

The typename static class is needed only to be able to distinguish variables if two differents static classes need to implement the same singleton.
