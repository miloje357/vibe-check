#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <check.h>

START_TEST(test_string) { 
  LOG_ERROR("TEST");
}
END_TEST

START_TEST(test_null) { 
  LOG_ERROR(NULL);
}
END_TEST

START_TEST(test_non_standard_log_level) {
  log_message(124, "TEST");
}
END_TEST

START_TEST(test_heap_string) {
  char *msg;
  asprintf(&msg, "TEST");
  LOG_ERROR(msg);
  free(msg);
}
END_TEST

START_TEST(test_long_message) {
  int len = 6000;
  char buffer[len];
  for (int i = 0; i < len; i++) {
    buffer[i] = 'a';
  }
  LOG_ERROR(buffer);
}
END_TEST

Suite *logger_suite(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("Logger");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_string);
  tcase_add_test(tc_core, test_null);
  tcase_add_test(tc_core, test_non_standard_log_level);
  tcase_add_test(tc_core, test_long_message);
  tcase_add_test(tc_core, test_heap_string);
  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;
  s = logger_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  return 0;
}
