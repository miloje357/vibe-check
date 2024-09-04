#include "logger.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_FILENAME "test.log"

// @return number of characters the strinf was shifted
int shift_to_second_space(char **dest, int max_len) {
  bool seen_space = false;
  for (int i = 0; i < max_len; i++) {
    if ((*dest)[i] == ' ' && !seen_space) {
      seen_space = true;
      continue;
    }
    if ((*dest)[i] == ' ') {
      *dest += i + 1;
      return i + 1;
    }
  }
  return 0;
}

FILE *test_file;
char *test_str = NULL;
int line_len;

void setup() {
  test_file = fopen(TEST_FILENAME, "w");
  init_logger(test_file, LOG_LEVEL_INFO, false);
  test_str = malloc(sizeof(char) * MAX_MESSAGE_LEN);
}

void teardown() {
  fclose(test_file);
  remove(TEST_FILENAME);
  free(test_str);
  test_str = NULL;
  close_all_loggers();
}

void reopen_for_reading() {
  fclose(test_file);
  test_file = fopen(TEST_FILENAME, "r");
}

// @return number of characters the strinf was shifted
int set_line_from_testfile() {
  size_t len = MAX_MESSAGE_LEN;
  line_len = getline(&test_str, &len, test_file);
  if (line_len > 0) {
    int shift_by = shift_to_second_space(&test_str, line_len);
    return shift_by;
  }
  return 0;
}

START_TEST(test_string) {
  LOG_ERROR("TEST");
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_str_eq(test_str, "ERROR] TEST\n");
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_null) {
  LOG_ERROR(NULL);
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_int_le(line_len, 0);
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_non_standard_log_level) {
  log_fmessage(124, "TEST");
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_int_le(line_len, 0);
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_heap_string) {
  char *msg;
  asprintf(&msg, "TEST");
  LOG_ERROR("%s", msg);
  free(msg);
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_str_eq(test_str, "ERROR] TEST\n");
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_long_message) {
  int len = 6000;
  char buffer[len];
  for (int i = 0; i < len; i++) {
    buffer[i] = 'a';
  }
  LOG_ERROR("%s", buffer);
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_int_le(line_len, 0);
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_formating_const_string) {
  LOG_ERROR("TEST %s %s", "TEST", "TEST");
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_str_eq(test_str, "ERROR] TEST TEST TEST\n");
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_formating_heap_string) {
  char *test;
  asprintf(&test, "HEAP");
  LOG_ERROR("TEST %s %s", test, test);
  free(test);
  reopen_for_reading();
  int shift_by = set_line_from_testfile();
  ck_assert_str_eq(test_str, "ERROR] TEST HEAP HEAP\n");
  free(test_str - shift_by); // - shift_by to delete the original string
  test_str = NULL;
}
END_TEST

START_TEST(test_init_logger) {
  logger_id logger = init_logger(stdout, LOG_LEVEL_INFO, true);
  ck_assert_int_ne(logger, -1);
}
END_TEST

START_TEST(test_init_max_loggers) {
  int max_loggers = 9;
  logger_id last_logger;
  for (int i = 0; i <= max_loggers; i++) {
    last_logger = init_logger(stdout, LOG_LEVEL_INFO, true);
  }
  ck_assert_int_eq(last_logger, -1);
}
END_TEST

START_TEST(test_close_no_loggers) {
  bool is_closed = close_logger(0);
  ck_assert_int_eq(is_closed, 0);
}
END_TEST

START_TEST(test_close_logger) {
  logger_id logger = init_logger(stdout, LOG_LEVEL_INFO, true);
  bool is_closed = close_logger(logger);
  ck_assert_int_eq(is_closed, 1);
}
END_TEST

Suite *logger_suite(void) {
  Suite *suite = suite_create("Logger");

  TCase *logging = tcase_create("Logging");
  tcase_add_checked_fixture(logging, setup, teardown);
  tcase_add_test(logging, test_string);
  tcase_add_test(logging, test_null);
  tcase_add_test(logging, test_non_standard_log_level);
  tcase_add_test(logging, test_long_message);
  tcase_add_test(logging, test_heap_string);
  tcase_add_test(logging, test_formating_const_string);
  tcase_add_test(logging, test_formating_heap_string);

  TCase *loggers = tcase_create("Loggers");
  tcase_add_test(loggers, test_init_logger);
  tcase_add_test(loggers, test_init_max_loggers);
  tcase_add_test(loggers, test_close_no_loggers);
  tcase_add_test(loggers, test_close_logger);

  suite_add_tcase(suite, logging);
  suite_add_tcase(suite, loggers);
  return suite;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = logger_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  return 0;
}
