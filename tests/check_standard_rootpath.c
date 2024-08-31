#include "files.h"
#include "logger.h"

int main() {
  logger_id logger = init_logger(stderr, LOG_LEVEL_DEBUG, true);
  set_rootpath(NULL);
  char *token_filepath;
  set_log_filepath(&token_filepath);
  printf("%s\n", token_filepath);
  free_rootpath();
  close_logger(logger);
}
