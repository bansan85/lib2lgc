#define CHK

%module libgdb

%include <swiginterface.i>

//%interface_impl(llgc::poco::Number);
//%interface_impl(llgc::pattern::visitor::InterfaceVisitable);
//%interface_impl(llgc::pattern::visitor::BaseVisitable);

%{
// Do not include compatibility.
// #include "../2lgc/compatibility.h"
// #include "../2lgc/compatibility/visual_studio.h"
#include "../2lgc/error/show.h"
#include "../2lgc/filesystem/files.h"
#include "../2lgc/math/compare_decimal.h"
#include "../2lgc/math/hash.h"
#include "../2lgc/net/linux.h"
#include "../2lgc/pattern/abstract_factory/abstract_factory_interface.h"
#include "../2lgc/pattern/iterator/iterator.h"
#include "../2lgc/pattern/publisher/connector_direct.h"
#include "../2lgc/pattern/publisher/connector_interface.h"
#include "../2lgc/pattern/publisher/connector_publisher_tcp.h"
#include "../2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h"
#include "../2lgc/pattern/publisher/connector_publisher_tcp_ipv6.h"
#include "../2lgc/pattern/publisher/connector_subscriber_tcp.h"
#include "../2lgc/pattern/publisher/publisher_direct.h"
#include "../2lgc/pattern/publisher/publisher_interface.h"
#include "../2lgc/pattern/publisher/publisher_tcp.h"
#include "../2lgc/pattern/publisher/publisher_tcp_linux.h"
#include "../2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h"
#include "../2lgc/pattern/publisher/publisher_tcp_linux_ipv6.h"
#include "../2lgc/pattern/publisher/subscriber_direct.h"
#include "../2lgc/pattern/publisher/subscriber_interface.h"
#include "../2lgc/pattern/publisher/subscriber_server_tcp.h"
#include "../2lgc/pattern/singleton/singleton.h"
#include "../2lgc/pattern/visitor/visitable.h"
#include "../2lgc/pattern/visitor/visitor.h"
#include "../2lgc/poco/number.h"
#include "../2lgc/poco/number_impl.h"
#include "../2lgc/poco/number_visitor_unit.h"
#include "../2lgc/poco/number_visitor_value.h"
#include "../2lgc/software/gdb/backtrace.h"
#include "../2lgc/software/gdb/function.h"
#include "../2lgc/software/gdb/gdb.h"
#include "../2lgc/software/gdb/gdb_server.h"
#include "../2lgc/software/gdb/set_stack.h"
#include "../2lgc/software/gdb/stack.h"
#include "../2lgc/text/printf.h"
#include "../2lgc/text/string_ext.h"
#include "../2lgc/utils/thread/count_lock.h"
#include "../2lgc/utils/tree/node.h"
%}

%rename(operator_not_equal) operator!=;
%rename(operator_increment) operator++;
%rename(operator_parentheses) operator();

%include "../2lgc/error/show.h"
%include "../2lgc/filesystem/files.h"
%include "../2lgc/math/compare_decimal.h"
%include "../2lgc/math/hash.h"
%include "../2lgc/net/linux.h"
%include "../2lgc/pattern/abstract_factory/abstract_factory_interface.h"
%include "../2lgc/pattern/iterator/iterator.h"
%include "../2lgc/pattern/publisher/connector_direct.h"
%include "../2lgc/pattern/publisher/connector_interface.h"
%include "../2lgc/pattern/publisher/connector_publisher_tcp.h"
%include "../2lgc/pattern/publisher/connector_publisher_tcp_ipv4.h"
%include "../2lgc/pattern/publisher/connector_publisher_tcp_ipv6.h"
%include "../2lgc/pattern/publisher/connector_subscriber_tcp.h"
%include "../2lgc/pattern/publisher/publisher_direct.h"
%include "../2lgc/pattern/publisher/publisher_interface.h"
%include "../2lgc/pattern/publisher/publisher_tcp.h"
%include "../2lgc/pattern/publisher/publisher_tcp_linux.h"
%include "../2lgc/pattern/publisher/publisher_tcp_linux_ipv4.h"
%include "../2lgc/pattern/publisher/publisher_tcp_linux_ipv6.h"
%include "../2lgc/pattern/publisher/subscriber_direct.h"
%include "../2lgc/pattern/publisher/subscriber_interface.h"
%include "../2lgc/pattern/publisher/subscriber_server_tcp.h"
%include "../2lgc/pattern/singleton/singleton.h"
%include "../2lgc/pattern/visitor/visitable.h"


%include "../2lgc/pattern/visitor/visitor.h"

%template(visitor_number_constant) llgc::pattern::visitor::Visitor<llgc::poco::Number_Constant>;
%template(visitor_number_num_op_num) llgc::pattern::visitor::Visitor<llgc::poco::Number_NumOpNum>;
%template(interface_visitable_number) llgc::pattern::visitor::InterfaceVisitable<llgc::protobuf::math::Number>;

%include "../2lgc/poco/number.h"

%template(base_visitable_number_constant) llgc::pattern::visitor::BaseVisitable<llgc::poco::Number_Constant, llgc::poco::Number>;
%template(base_visitable_number_num_po_num) llgc::pattern::visitor::BaseVisitable<llgc::poco::Number_NumOpNum, llgc::poco::Number>;

%include "../2lgc/poco/number_impl.h"
%include "../2lgc/poco/number_visitor_unit.h"
%include "../2lgc/poco/number_visitor_value.h"

%template(iterator_stack_backtrace) llgc::pattern::iterator::Iterator<llgc::software::gdb::Stack, llgc::software::gdb::Backtrace>;

%include "../2lgc/software/gdb/backtrace.h"
%include "../2lgc/software/gdb/function.h"
%include "../2lgc/software/gdb/gdb.h"

%template(local_publisher_direct_gdb) llgc::pattern::singleton::Local<llgc::pattern::publisher::PublisherDirect<llgc::protobuf::software::Gdb>>;

%include "../2lgc/software/gdb/gdb_server.h"
%include "../2lgc/software/gdb/set_stack.h"

%template(iterator_set_stack_stack) llgc::pattern::iterator::Iterator<llgc::software::gdb::SetStack, llgc::software::gdb::Stack>;

%include "../2lgc/software/gdb/stack.h"
%include "../2lgc/text/printf.h"
%include "../2lgc/text/string_ext.h"
%include "../2lgc/utils/thread/count_lock.h"
%include "../2lgc/utils/tree/node.h"
