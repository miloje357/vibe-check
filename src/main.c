#include <stdio.h>
#include <config.h>
#include <stdlib.h>
#include "logger.h"
#include "files.h"

FILE *log_file;

void setup() {
  bool s = set_rootpath(NULL);
  if (!s) {
    DEV_ERROR("Exiting...");
    exit(EXIT_FAILURE);
  }

  char *log_filepath;
  set_log_filepath(&log_filepath);
  if (!log_filepath) {
    DEV_ERROR("Exiting...");
    exit(EXIT_FAILURE);
  }

  log_file = fopen(log_filepath, "w");
  if (!log_file) {
    DEV_ERROR("Couldn't open %s. Exiting...", log_filepath);
    exit(EXIT_FAILURE);
  }

  int status = init_logger(stdout, LOG_LEVEL_WARNING, true);
  if (status == -1) {
    DEV_ERROR("Coldn't initialize any logger. Exiting...");
    exit(EXIT_FAILURE);
  }
  init_logger(log_file, LOG_LEVEL_INFO, false);
  LOG_INFO("Started two loggers: stdout and %s", log_filepath);

  free(log_filepath);
}

void teardown() {
  fclose(log_file);
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
