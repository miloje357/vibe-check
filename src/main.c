#include <stdio.h>
#include <config.h>
#include <stdlib.h>
#include "logger.h"
#include "files.h"

logger_id stdout_logger;
logger_id file_logger;
FILE *log_file;

void setup() {
  bool s = set_rootpath(NULL);
  if (!s) {
    exit(EXIT_FAILURE);
  }

  char *log_filepath;
  set_log_filepath(&log_filepath);
  if (!log_filepath) {
    exit(EXIT_FAILURE);
  }

  log_file = fopen(log_filepath, "w");
  if (!log_file) {
    exit(EXIT_FAILURE);
  }

  file_logger = init_logger(log_file, LOG_LEVEL_INFO, false);
  stdout_logger = init_logger(stdout, LOG_LEVEL_WARNING, true);

  free(log_filepath);
}

void teardown() {
  fclose(log_file);
  close_logger(file_logger);
  close_logger(stdout_logger);
}

int main() {
  setup();
  LOG_INFO("%s", PACKAGE_STRING);
  printf("Hello, World!\n");
  teardown();
  return 0;
}
