lib_LTLIBRARIES	=	lib2lgcMath.la


lib2lgcMath_la_CXXFLAGS	= $(AM_CXXFLAGS)

lib2lgcMath_la_LDFLAGS	= $(AM_LDFLAGS) -release $(VERSION)

lib2lgcMath_la_SOURCES	= compare_decimal.cc

lib2lgcMath_ladir				= $(includedir)/2lgc

lib2lgcMath_la_HEADERS	= compare_decimal.h


if HAVE_INTERNAL_DISTCHECK
CPPLINT_PRE_ROOT=$(PACKAGE)-$(PACKAGE_VERSION)/
else
CPPLINT_PRE_ROOT=
endif

check-local :
	cpplint.py --filter=-build/include --root=$(CPPLINT_PRE_ROOT)$(subdir) $(patsubst %,$(srcdir)/%,$(lib2lgcMath_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcMath_la_HEADERS))
	cppcheck --inconclusive --enable=all $(DEFAULT_INCLUDES) $(patsubst %,$(srcdir)/%,$(lib2lgcMath_la_SOURCES))
	$(foreach file, $(patsubst %,$(srcdir)/%,$(lib2lgcMath_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcMath_la_HEADERS)), \
		(clang-format -style=Google $(file) | diff -pu $(file) -) || exit 1;\
	)


if HAVE_DOXYGEN

all-local: doxygen.stamp

doxygen.stamp: $(lib2lgcMath_la_SOURCES) $(lib2lgcMath_la_HEADERS)
	mkdir -p ../../doc/math
	echo Timestamp > doxygen.stamp
	$(DOXYGEN) ./Doxyfile

CLEANFILES  = doxygen.stamp

EXTRA_DIST  = Doxyfile

clean-local:
	rm -Rf ../../doc/math

endif
