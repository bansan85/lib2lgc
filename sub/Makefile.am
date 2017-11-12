lib_LTLIBRARIES	=	lib2lgcVisitor.la


lib2lgcVisitor_la_CXXFLAGS	= $(AM_CXXFLAGS)

lib2lgcVisitor_la_LDFLAGS		= $(AM_LDFLAGS) -release $(VERSION)

lib2lgcVisitor_la_SOURCES		=

lib2lgcVisitor_ladir				= $(includedir)/2lgc

lib2lgcVisitor_la_HEADERS		= visitable.h \
															visitor.h


if HAVE_INTERNAL_DISTCHECK
CPPLINT_PRE_ROOT=$(PACKAGE)-$(PACKAGE_VERSION)/
else
CPPLINT_PRE_ROOT=
endif

check-local :
	cpplint.py --filter=-build/include --root=$(CPPLINT_PRE_ROOT)$(subdir) $(patsubst %,$(srcdir)/%,$(lib2lgcVisitor_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcVisitor_la_HEADERS))
#	cppcheck --inconclusive --enable=all $(DEFAULT_INCLUDES) $(patsubst %,$(srcdir)/%,$(lib2lgcVisitor_la_SOURCES))
	$(foreach file, $(patsubst %,$(srcdir)/%,$(lib2lgcVisitor_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcVisitor_la_HEADERS)), \
		(clang-format -style=Google $(file) | diff -pu $(file) -) || exit 1;\
	)


if HAVE_DOXYGEN

all-local: doxygen.stamp

doxygen.stamp: $(lib2lgcVisitor_la_SOURCES) $(lib2lgcVisitor_la_HEADERS)
	mkdir -p ../../../doc/pattern/visitor
	echo Timestamp > doxygen.stamp
	$(DOXYGEN) ./Doxyfile

CLEANFILES	=	doxygen.stamp

EXTRA_DIST	= Doxyfile

clean-local:
	rm -Rf ../../../doc/pattern/visitor

endif
