// CHK is useless.
#define CHK 

%module libgdb
%{
#include "../inc/backtrace.h"
#include "../inc/function.h"
#include "../inc/gdb.h"
#include "../inc/set_stack.h"
#include "../inc/stack.h"
%}

%include "../inc/backtrace.h"
%include "../inc/function.h"
%include "../inc/gdb.h"
%include "../inc/set_stack.h"
%include "../inc/stack.h"
