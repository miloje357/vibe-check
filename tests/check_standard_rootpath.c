#include "files.h"
#include "logger.h"

int main() {
  logger_id logger = init_logger(stderr, LOG_LEVEL_DEBUG, true);
  set_rootpath(NULL);
  free_rootpath();
  close_logger(logger);
}
