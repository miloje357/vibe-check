bin_PROGRAMS = vibe_check
vibe_check_SOURCES = \
	src/main.c\
	src/logger.c\
	include/logger.h
vibe_check_CFLAGS = -g -Wall -Werror -O3\
		    -I$(top_srcdir)/include

TESTS = check_logger\
	check_files

check_PROGRAMS = check_logger\
		 check_files\
		 check_standard_rootpath

check_logger_SOURCES = src/logger.c\
			include/logger.h\
			tests/check_logger.c
check_logger_CFLAGS = ${CHECK_CFLAGS} -I$(top_srcdir)/include
check_logger_LDADD= ${CHECK_LIBS}

check_files_SOURCES = src/logger.c\
			src/files.c\
			include/logger.h\
			include/files.h\
			tests/check_files.c
check_files_CFLAGS = ${CHECK_CFLAGS} -I$(top_srcdir)/include
check_files_LDADD= ${CHECK_LIBS}

check_standard_rootpath_SOURCES = src/logger.c\
			src/files.c\
			include/logger.h\
			include/files.h\
			tests/check_standard_rootpath.c
check_standard_rootpath_CFLAGS = -I$(top_srcdir)/include
