#include "files.h"
#include "logger.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

logger_id logger;
#define MAX_ITER 1000

void setup() { logger = init_logger(stderr, LOG_LEVEL_DEBUG, true); }

void teardown() { close_logger(logger); }

START_TEST(test_set_rootpath_cwd_with_slash) {
  bool s = set_rootpath("./");
  ck_assert_int_eq(s, 1);
  char *log_filepath = NULL;
  set_log_filepath(&log_filepath);
  ck_assert_str_eq(log_filepath, "./" LOG_FILENAME);
  free(log_filepath);
  free_rootpath();
}
END_TEST

START_TEST(test_set_rootpath_cwd_without_slash) {
  bool s = set_rootpath(".");
  ck_assert_int_eq(s, 1);
  char *log_filepath = NULL;
  set_log_filepath(&log_filepath);
  ck_assert_str_eq(log_filepath, "./" LOG_FILENAME);
  free(log_filepath);
  free_rootpath();
}
END_TEST

START_TEST(test_set_rootpath_standard_rootpath) {
  bool s = set_rootpath(NULL);
  ck_assert_int_eq(s, 1);
  char *log_filepath = NULL;
  set_log_filepath(&log_filepath);

  char home[6];
  for (int i = 0; i < 5; i++) {
    home[i] = log_filepath[i];
  }
  home[5] = '\0';
  ck_assert_str_eq(home, "/home");

  char *rest = log_filepath;
  int num_perent = 0;
  for (int i = 0; *rest != '\0' && num_perent < 3; i++) {
    if (i >= MAX_ITER) {
      fail("log_filepath is not null-terminated");
    }
    if (*rest == '/') {
      num_perent++;
    }
    rest++;
  }
  ck_assert_str_eq(rest, ".cache/" PACKAGE_NAME "/" PACKAGE_NAME ".log");

  free(log_filepath);
  free_rootpath();
}
END_TEST

Suite *files_suite(void) {
  Suite *suite = suite_create("Files");
  TCase *setting_root = tcase_create("Setting root");
  tcase_add_checked_fixture(setting_root, setup, teardown);
  tcase_add_test(setting_root, test_set_rootpath_cwd_with_slash);
  tcase_add_test(setting_root, test_set_rootpath_cwd_without_slash);
  tcase_add_test(setting_root, test_set_rootpath_standard_rootpath);
  suite_add_tcase(suite, setting_root);
  return suite;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = files_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  return 0;
}
