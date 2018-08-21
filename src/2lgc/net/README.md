# Net

# linux.h

Enhancement various functions to communicate throw networ :
  - Bind: add timeout.
  - Send: use std::string.
  - RepeteOnEintr: repete function if it's launch has been stopped by the kernel. To be use with select and accept4.

And add simples usefull functions for network:
  - DisableSigPipe: disable interrupt if using invalid pipe (file descriptor). Be sure to always check return value of function using file description (socket).
  - Class AutoCloseSocket: auto close a socket on destructor. Equivalent of std::unique_ptr for socket file descriptor.
