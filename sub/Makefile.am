CC_FROM_PROTO	=	cpp/area.pb.cc \
								cpp/load.pb.cc \
								cpp/number.pb.cc \
								cpp/project.pb.cc \
								cpp/raw.pb.cc

H_FROM_PROTO	=	$(CC_FROM_PROTO:.pb.cc=.pb.h)

EXTRA_DIST		=	$(CC_FROM_PROTO) \
								$(H_FROM_PROTO)

CLEANFILES		=	$(EXTRA_DIST)

if HAVE_DOXYGEN
all-local: cpp doxygen.stamp
else
all-local: cpp
endif

cpp:	$(CC_FROM_PROTO) $(H_FROM_PROTO)

cpp/%.pb.cc : %.proto
	protoc --cpp_out=cpp $^

cpp/%.pb.h : %.proto
	protoc --cpp_out=cpp $^

lib_LTLIBRARIES	=	lib2lgcPoco.la \
									lib2lgcPocoGetSet.la

lib2lgcPoco_la_CXXFLAGS	= $(AM_CXXFLAGS) \
													-Wno-switch-default \
													-Wno-suggest-final-methods \
													-Wno-zero-as-null-pointer-constant \
													-Wno-unused-parameter \
													-Wno-strict-aliasing \
													-Wno-effc++ \
													-Wno-suggest-override \
													-Wno-float-equal \
													-Wno-suggest-final-types \
													-Wno-sign-conversion \
													-Wno-suggest-attribute=const

lib2lgcPoco_la_LDFLAGS	= $(AM_LDFLAGS) -release $(VERSION)

lib2lgcPoco_la_SOURCES	= $(CC_FROM_PROTO)

lib2lgcPoco_ladir				= $(includedir)/2lgc

lib2lgcPoco_la_HEADERS	= $(H_FROM_PROTO)


lib2lgcPocoGetSet_la_CXXFLAGS	=	-isystem $(srcdir)/cpp \
																-isystem $(srcdir)/../pattern/visitor \
																-isystem $(srcdir)/../macro \
																-isystem $(srcdir)/../math \
																$(AM_CXXFLAGS)

lib2lgcPocoGetSet_la_LDFLAGS	= $(AM_LDFLAGS) -release $(VERSION)

lib2lgcPocoGetSet_la_SOURCES	= number.cc \
																number_visitor_unit.cc \
																number_visitor_value.cc

lib2lgcPocoGetSet_ladir				= $(includedir)/2lgc

lib2lgcPocoGetSet_la_HEADERS	= number.h \
																number_visitor_unit.h \
																number_visitor_value.h

if HAVE_INTERNAL_DISTCHECK
CPPLINT_PRE_ROOT=$(PACKAGE)-$(PACKAGE_VERSION)/
else
CPPLINT_PRE_ROOT=
endif

check-local :
	cpplint.py --filter=-build/include --root=$(CPPLINT_PRE_ROOT)$(subdir) $(patsubst %,$(srcdir)/%,$(lib2lgcPocoGetSet_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcPocoGetSet_la_HEADERS))
	cppcheck --inconclusive --enable=all $(DEFAULT_INCLUDES) $(patsubst %,$(srcdir)/%,$(lib2lgcPocoGetSet_la_SOURCES))
	$(foreach file, $(patsubst %,$(srcdir)/%,$(lib2lgcPocoGetSet_la_SOURCES)) $(patsubst %,$(srcdir)/%,$(lib2lgcPocoGetSet_la_HEADERS)), \
		(clang-format -style=Google $(file) | diff -pu $(file) -) || exit 1;\
	)

if HAVE_DOXYGEN

doxygen.stamp: $(CC_FROM_PROTO) $(H_FROM_PROTO) $(lib2lgcPocoGetSet_la_SOURCES) $(lib2lgcPocoGetSet_la_HEADERS)
	mkdir -p ../../doc/poco
	echo Timestamp > doxygen.stamp
	$(DOXYGEN) ./Doxyfile

CLEANFILES  += doxygen.stamp

clean-local:
	rm -Rf ../../doc/poco

endif
