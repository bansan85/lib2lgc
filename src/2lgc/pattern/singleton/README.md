<h1>Introduction</h1>

The goal of the singleton pattern is to be able to have only one instance of a class that will be shared by multiple threads.

<h1>Design</h1>

Singleton has two functions and one public member.

`bool IsInstance();` is to know if the instance is already instanciated.
It's usefull if you need to call a function only if the object is instanciated.
For example, if the instance is a server, there's no need to allocate it if you want send message to all subscribers.

`std::shared_ptr<T> GetInstance();` is to get the shared instance and instanciate it if needed.

`std::recursive_mutex mutex_;` is to freeze usage of singleton.

The template needs to know the class of the instance. The class will be instantiated with the empty constructor.
