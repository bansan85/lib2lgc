#include <iostream2

lib_LTLIBRARIES	=	lib2lgcPublisher.la


lib2lgcPublisher_la_CXXFLAGS	=	-isystem $(srcdir)/../../macro \
																$(AM_CXXFLAGS)

lib2lgcPublisher_la_LDFLAGS		= $(AM_LDFLAGS) -release $(VERSION)

lib2lgcPublisher_la_SOURCES		= connector_direct.cc \
																connector_interface.cc \
																publisher_base.cc \
																publisher_remote.cc \
																subscriber_direct.cc \
																subscriber_interface.cc

lib2lgcPublisher_ladir				= $(includedir)/2lgc

lib2lgcPublisher_la_HEADERS		= connector_direct.h \
																connector_interface.h \
																publisher_base.h \
																publisher_remote.h \
																subscriber_direct.h \
																subscriber_interface.h


if HAVE_INTERNAL_DISTCHECK
CPPLINT_PRE_ROOT=$(PACKAGE)-$(PACKAGE_VERSION)/
else
CPPLINT_PRE_ROOT=
endif

check-local :
	cpplint.py --filter=-build/include --root=$(CPPLINT_PRE_ROOT)$(subdir) $(patsubst %,$(srcdir)/%,$(lib2lgcPublisher_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcPublisher_la_HEADERS))
	cppcheck --inconclusive --enable=all $(DEFAULT_INCLUDES) $(patsubst %,$(srcdir)/%,$(lib2lgcPublisher_la_SOURCES))
	$(foreach file, $(patsubst %,$(srcdir)/%,$(lib2lgcPublisher_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcPublisher_la_HEADERS)), \
		(clang-format -style=Google $(file) | diff -pu $(file) -) || exit 1;\
	)


if HAVE_DOXYGEN

all-local: doxygen.stamp

doxygen.stamp: $(lib2lgcPublisher_la_SOURCES) $(lib2lgcPublisher_la_HEADERS)
	mkdir -p ../../../doc/pattern/publisher
	echo Timestamp > doxygen.stamp
	$(DOXYGEN) ./Doxyfile

CLEANFILES	=	doxygen.stamp

EXTRA_DIST	= Doxyfile

clean-local:
	rm -Rf ../../../doc/pattern/publisher

endif
