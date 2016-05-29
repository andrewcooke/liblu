
TESTS = check_strings check_log check_simplex check_random check_tiles \
	check_arrays check_vectors

check_PROGRAMS = check_strings check_log check_simplex check_random \
	check_tiles check_arrays check_vectors

check_arrays_SOURCES = check_arrays.c $(top_builddir)/lib/lu/arrays.h
check_arrays_CFLAGS = @CHECK_CFLAGS@
check_arrays_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_log_SOURCES = check_log.c $(top_builddir)/lib/lulog.h
check_log_CFLAGS = @CHECK_CFLAGS@
check_log_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_random_SOURCES = check_random.c $(top_builddir)/lib/lu/random.h
check_random_CFLAGS = @CHECK_CFLAGS@
check_random_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_simplex_SOURCES = check_simplex.c $(top_builddir)/lib/lu/simplex.h
check_simplex_CFLAGS = @CHECK_CFLAGS@
check_simplex_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_strings_SOURCES = check_strings.c $(top_builddir)/lib/lu/strings.h
check_strings_CFLAGS = @CHECK_CFLAGS@
check_strings_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_tiles_SOURCES = check_tiles.c $(top_builddir)/lib/lu/tiles.h
check_tiles_CFLAGS = @CHECK_CFLAGS@
check_tiles_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

check_vectors_SOURCES = check_vectors.c $(top_builddir)/lib/lu/vectors.h
check_vectors_CFLAGS = @CHECK_CFLAGS@
check_vectors_LDADD = $(top_builddir)/lib/liblu.la @CHECK_LIBS@

