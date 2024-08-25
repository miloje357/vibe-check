#include "logger.h"
#include <stdlib.h>
#include <check.h>

START_TEST(test_string) { 
  log_message("pada");
}
END_TEST

START_TEST(test_null) { 
  log_message(NULL);
}
END_TEST

Suite *logger_suite(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("Logger");
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_string);
  tcase_add_test(tc_core, test_null);
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
