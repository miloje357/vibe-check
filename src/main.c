#include <stdio.h>
#include <config.h>
#include <stdlib.h>
#include "logger.h"
#include "files.h"

void setup() {
  bool s = set_rootpath(NULL);
  if (!s) {
    DEV_ERROR("Exiting...");
    exit(EXIT_FAILURE);
  }

  char *log_filepath;
  set_log_filepath(&log_filepath);

  int status = init_logger("stdout", LOG_LEVEL_WARNING);
  if (status == -1) {
    DEV_ERROR("Coldn't initialize any logger. Exiting...");
    exit(EXIT_FAILURE);
  }
  if (log_filepath) {
    init_logger(log_filepath, LOG_LEVEL_INFO);
  }
  LOG_INFO("Started logging");

  free(log_filepath);
}

void teardown() {
  close_all_loggers();
  free_rootpath();
}

int main() {
  setup();
  LOG_INFO("%s", PACKAGE_STRING);
  printf("Hello, World!\n");
  teardown();
  return 0;
}
