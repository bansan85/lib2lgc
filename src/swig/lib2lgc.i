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

%template(interface_visitable_number) pattern::visitor::InterfaceVisitable<msg::Number>;

%include "../2lgc/pattern/visitor/visitor.h"

%template(visitor_number_constant) pattern::visitor::Visitor<pattern::visitor::Number_Constant>;
%template(visitor_number_num_op_num) pattern::visitor::Visitor<pattern::visitor::Number_NumOpNum>;

%include "../2lgc/poco/number.h"

%template(base_visitable_number_constant) pattern::visitor::BaseVisitable<pattern::visitor::Number_Constant, pattern::visitor::Number>;
%template(base_visitable_number_num_po_num) pattern::visitor::BaseVisitable<pattern::visitor::Number_NumOpNum, pattern::visitor::Number>;

%include "../2lgc/poco/number_impl.h"

%include "../2lgc/poco/number_visitor_unit.h"
%include "../2lgc/poco/number_visitor_value.h"
%include "../2lgc/software/gdb/backtrace.h"
%include "../2lgc/software/gdb/function.h"
%include "../2lgc/software/gdb/gdb.h"
%include "../2lgc/software/gdb/set_stack.h"
%include "../2lgc/software/gdb/stack.h"
