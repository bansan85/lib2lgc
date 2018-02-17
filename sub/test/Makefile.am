CC_FROM_PROTO = cpp/actions.pb.cc

H_FROM_PROTO	= $(CC_FROM_PROTO:.pb.cc=.pb.h)

EXTRA_DIST		= $(CC_FROM_PROTO) \
								$(H_FROM_PROTO)

CLEANFILES		= $(EXTRA_DIST)

check-local: cpp

cpp:	$(CC_FROM_PROTO) $(H_FROM_PROTO)

cpp/%.pb.cc : %.proto
	protoc --cpp_out=cpp $^

cpp/%.pb.h : %.proto
	protoc --cpp_out=cpp $^


check_PROGRAMS = publisher1

TESTS = $(check_PROGRAMS)


publisher1_SOURCES_WITHOUT_PROTO = publisher1.cc
publisher1_SOURCES 	= $(publisher1_SOURCES_WITHOUT_PROTO) $(CC_FROM_PROTO)

publisher1_CXXFLAGS	= `pkg-config --libs protobuf` \
											-isystem $(srcdir)/../../../../lib/pattern/publisher \
											-isystem $(srcdir)/../../../../lib/macro \
											$(AM_CXXFLAGS)

publisher1_LDADD		= ../../../../lib/pattern/publisher/lib2lgcPublisher.la


if HAVE_INTERNAL_DISTCHECK
CPPLINT_PRE_ROOT=$(PACKAGE)-$(PACKAGE_VERSION)/
else
CPPLINT_PRE_ROOT=
endif

check-local :
	cpplint.py --filter=-build/include --root=$(CPPLINT_PRE_ROOT)$(subdir) $(patsubst %,$(srcdir)/%,$(publisher1_SOURCES_WITHOUT_PROTO))
	cppcheck --inconclusive --enable=all $(DEFAULT_INCLUDES) $(patsubst %,$(srcdir)/%,$(publisher1_SOURCES_WITHOUT_PROTO))
	$(foreach file, $(patsubst %,$(srcdir)/%,$(publisher1_SOURCES_WITHOUT_PROTO)), \
		(clang-format -style=Google $(file) | diff -pu $(file) -) || exit 1;\
	)
