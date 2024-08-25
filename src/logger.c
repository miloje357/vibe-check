#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *log_lvl_to_str(LogLevel lvl) {
  switch (lvl) {
  case LOG_LEVEL_DEBUG:
    return "DEBUG";
  case LOG_LEVEL_INFO:
    return "INFO";
  case LOG_LEVEL_WARNING:
    return "WARNING";
  case LOG_LEVEL_ERROR:
    return "ERROR";
  }
  return NULL;
}

void log_message(LogLevel lvl, const char *message) {
  const char *lvl_str = log_lvl_to_str(lvl);
  if (!lvl_str) {
    fprintf(stderr, "Non-standard LogLevel\n");
    return;
  }

  if (!message) {
    fprintf(stderr, "Can't log NULL\n");
    return;
  }

  if (strlen(message) >= MAX_MESSAGE_LEN) {
    fprintf(stderr, "Log message too long\n");
    return;
  }

  char *formated_message;
  asprintf(&formated_message, "[%s] %s", lvl_str, message);
  puts(formated_message);
  free(formated_message);
}
