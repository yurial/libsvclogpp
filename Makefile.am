ACLOCAL_AMFLAGS = -I m4
lib_LIBRARIES = @BUILD_LIBS@
EXTRA_LIBRARIES = libsvclogpp.a libsvclogpp_r.a
libsvclogpp_a_SOURCES = src/svclog.cpp
libsvclogpp_r_a_SOURCES = src/svclog_r.cpp
libsvclogpp_a_CPPFLAGS = -fPIC
libsvclogpp_r_a_CPPFLAGS = -fPIC
library_includedir=$(includedir)/
library_include_HEADERS = src/svclog.hpp
man_MANS = man/libsvclogpp.7
EXTRA_DIST = ebuild $(man_MANS) examples

