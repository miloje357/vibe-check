#include <stdio.h>
#include <config.h>
#include "logger.h"

int main() {
  logger_id logger = init_logger(stdout, LOG_LEVEL_WARNING, true);
  LOG_INFO("%s", PACKAGE_STRING);
  printf("Hello, World!\n");
  close_logger(logger);
  return 0;
}
