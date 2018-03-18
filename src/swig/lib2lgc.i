#define CHK

%module libgdb
%{
// Do not include compatibility.
// %include "../2lgc/compatibility.h"
// %include "../2lgc/compatibility/visual_studio.h"
#include "../2lgc/error/show.h"
#include "../2lgc/math/compare_decimal.h"
#include "../2lgc/pattern/visitor/visitable.h"
#include "../2lgc/pattern/visitor/visitor.h"
#include "../2lgc/poco/number.h"
#include "../2lgc/poco/number_visitor_unit.h"
#include "../2lgc/poco/number_visitor_value.h"
#include "../2lgc/software/gdb/backtrace.h"
#include "../2lgc/software/gdb/function.h"
#include "../2lgc/software/gdb/gdb.h"
#include "../2lgc/software/gdb/set_stack.h"
#include "../2lgc/software/gdb/stack.h"
%}

%include "../2lgc/error/show.h"
%include "../2lgc/math/compare_decimal.h"
%include "../2lgc/pattern/visitor/visitable.h"
%include "../2lgc/pattern/visitor/visitor.h"
%include "../2lgc/poco/number.h"
%include "../2lgc/poco/number_visitor_unit.h"
%include "../2lgc/poco/number_visitor_value.h"
%include "../2lgc/software/gdb/backtrace.h"
%include "../2lgc/software/gdb/function.h"
%include "../2lgc/software/gdb/gdb.h"
%include "../2lgc/software/gdb/set_stack.h"
%include "../2lgc/software/gdb/stack.h"
