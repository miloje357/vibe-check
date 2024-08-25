#include "logger.h"
#include <stdio.h>

void log_message(const char *message) {
  if (!message) {
    log_message("Can't log NULL");
    return;
  }
  puts(message);
}
