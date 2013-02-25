# file      : odb/Makefile.am
# copyright : Copyright (c) 2009-2012 Code Synthesis Tools CC
# license   : GNU GPL v2; see accompanying LICENSE file

lib_LTLIBRARIES = libodb.la
libodb_la_SOURCES = callback.cxx exceptions.cxx database.cxx connection.cxx lazy-ptr-impl.cxx schema-catalog.cxx session.cxx statement.cxx tracer.cxx transaction.cxx details/buffer.cxx details/condition.cxx details/lock.cxx details/mutex.cxx details/shared-ptr/base.cxx

odbincludedir = $(includedir)/odb
nobase_odbinclude_HEADERS = std-map-traits.hxx std-set-traits.hxx no-id-object-result.txx lazy-ptr-impl.txx post.hxx session.txx no-op-cache-traits.hxx transaction.ixx view-result.hxx cache-traits.hxx std-array-traits.hxx std-unordered-set-traits.hxx lazy-ptr.txx database.txx traits.hxx tracer.hxx result.hxx session.hxx object-result.hxx lazy-ptr.hxx exception.hxx transaction.hxx tr1/lazy-ptr.txx tr1/memory.hxx tr1/lazy-ptr.hxx tr1/pointer-traits.hxx tr1/lazy-ptr.ixx tr1/wrapper-traits.hxx tr1/lazy-pointer-traits.hxx connection.hxx polymorphic-map.hxx connection.ixx session.ixx core.hxx simple-object-result.hxx pointer-traits.hxx pre.hxx statement.hxx database.hxx database.ixx simple-object-result.txx view-result.txx lazy-ptr-impl.ixx lazy-ptr.ixx forward.hxx schema-catalog.hxx std-vector-traits.hxx lazy-ptr-impl.hxx polymorphic-object-result.hxx exceptions.hxx compilers/vc/post.hxx compilers/vc/pre.hxx wrapper-traits.hxx polymorphic-info.hxx polymorphic-map.ixx container-traits.hxx query.hxx details/type-info.hxx details/shared-ptr.hxx details/meta/remove-const.hxx details/meta/polymorphic-p.hxx details/meta/remove-volatile.hxx details/meta/answer.hxx details/meta/remove-pointer.hxx details/meta/remove-const-volatile.hxx details/meta/class-p.hxx details/exception.hxx details/mutex.hxx details/shared-ptr-fwd.hxx details/condition.hxx details/thread.hxx details/tls.hxx details/wrapper-p.hxx details/unused.hxx details/export.hxx details/lock.hxx details/transfer-ptr.hxx details/buffer.hxx details/unique-ptr.hxx details/shared-ptr/base.txx details/shared-ptr/counter-type.hxx details/shared-ptr/base.ixx details/shared-ptr/base.hxx details/config.hxx no-id-object-result.hxx polymorphic-object-result.txx std-unordered-map-traits.hxx lazy-pointer-traits.hxx schema-catalog-impl.hxx callback.hxx nullable.hxx std-forward-list-traits.hxx version.hxx std-list-traits.hxx polymorphic-map.txx
nobase_nodist_odbinclude_HEADERS = details/config.h

EXTRA_DIST = details/config-vc.h libodb-vc9.vcproj libodb-vc10.vcxproj libodb-vc10.vcxproj.filters libodb-vc11.vcxproj libodb-vc11.vcxproj.filters

if ODB_THREADS_POSIX
libodb_la_SOURCES += details/posix/exceptions.cxx details/posix/thread.cxx
nobase_odbinclude_HEADERS += details/posix/mutex.hxx details/posix/thread.ixx details/posix/condition.hxx details/posix/thread.hxx details/posix/tls.ixx details/posix/mutex.ixx details/posix/tls.txx details/posix/tls.hxx details/posix/exceptions.hxx details/posix/condition.ixx
endif

if ODB_THREADS_WIN32
libodb_la_SOURCES += details/win32/init.cxx details/win32/once.cxx details/win32/tls.cxx details/win32/condition.cxx details/win32/thread.cxx details/win32/exceptions.cxx details/win32/dll.cxx
nobase_odbinclude_HEADERS += details/win32/once.hxx details/win32/once-init.hxx details/win32/tls-init.hxx details/win32/mutex.hxx details/win32/condition.hxx details/win32/thread.hxx details/win32/once.ixx details/win32/tls.ixx details/win32/mutex.ixx details/win32/tls.txx details/win32/tls.hxx details/win32/exceptions.hxx details/win32/condition.ixx details/win32/lock.hxx details/win32/init.hxx
endif

AM_CPPFLAGS= -I'$(top_builddir)' -I'$(top_srcdir)' -DLIBODB_DYNAMIC_LIB
AM_LDFLAGS = -release 2.1 -no-undefined
