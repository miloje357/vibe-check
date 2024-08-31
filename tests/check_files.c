#include "files.h"
#include "logger.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

logger_id logger;

void setup() { logger = init_logger(stderr, LOG_LEVEL_DEBUG, true); }

void teardown() { close_logger(logger); }

START_TEST(test_set_rootpath_cwd_with_slash) {
  bool s = set_rootpath("./");
  ck_assert_int_eq(s, 1);
  char *token_filepath = NULL;
  set_log_filepath(&token_filepath);
  ck_assert_str_eq(token_filepath, "./" LOG_FILENAME);
  free(token_filepath);
  free_rootpath();
}
END_TEST

START_TEST(test_set_rootpath_cwd_without_slash) {
  bool s = set_rootpath(".");
  ck_assert_int_eq(s, 1);
  char *token_filepath = NULL;
  set_log_filepath(&token_filepath);
  ck_assert_str_eq(token_filepath, "./" LOG_FILENAME);
  free(token_filepath);
  free_rootpath();
}
END_TEST

// NOTE: Doesn't check if set_rootpath(NULL) ran properly (that is if .cache
// exists then mkdir ~/.cache/vibe_check/ else mkdir ~/.vibecheck)
// it just checks if returned values aren't null
START_TEST(test_set_rootpath_standard_rootpath) {
  bool s = set_rootpath(NULL);
  ck_assert_int_eq(s, 1);
  char *token_filepath = NULL;
  set_log_filepath(&token_filepath);
  ck_assert_ptr_nonnull(token_filepath);
  free(token_filepath);
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
