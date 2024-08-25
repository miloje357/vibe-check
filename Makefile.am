bin_PROGRAMS = vibe_check
vibe_check_SOURCES = \
	src/main.c
vibe_check_CFLAGS = -g -Wall -Werror -O3\
		    -I$(top_srcdir)/include

TESTS = check_logger

check_PROGRAMS = check_logger

check_logger_SOURCES = src/logger.c\
			include/logger.h\
			tests/check_logger.c
check_logger_CFLAGS = ${CHECK_CFLAGS} -I$(top_srcdir)/include
check_logger_LDADD= ${CHECK_LIBS}
