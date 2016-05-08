
TESTS = check_str check_log check_simplex

check_PROGRAMS = check_str check_log check_simplex

check_str_SOURCES = check_str.c $(top_builddir)/lib/lustr.h
check_str_CFLAGS = @CHECK_CFLAGS@
check_str_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_log_SOURCES = check_log.c $(top_builddir)/lib/lulog.h
check_log_CFLAGS = @CHECK_CFLAGS@
check_log_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_simplex_SOURCES = check_simplex.c $(top_builddir)/lib/lusimplex.h
check_simplex_CFLAGS = @CHECK_CFLAGS@
check_simplex_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@
