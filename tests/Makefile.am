
TESTS = check_str check_log

check_PROGRAMS = check_str check_log

check_str_SOURCES = check_str.c $(top_builddir)/lib/luinternal.h
check_str_CFLAGS = @CHECK_CFLAGS@
check_str_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_log_SOURCES = check_log.c $(top_builddir)/lib/luinternal.h
check_log_CFLAGS = @CHECK_CFLAGS@
check_log_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@
