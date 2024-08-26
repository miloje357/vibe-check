#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void set_timestamp(char *str, size_t str_len) {
  time_t rawtime;
  time(&rawtime);
  struct tm *timeinfo = localtime(&rawtime);
  strftime(str, str_len, "%Y-%m-%d %H:%M:%S", timeinfo);
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

  size_t timestamp_len = 20;
  char timestamp[timestamp_len];
  set_timestamp(timestamp, timestamp_len);

  char *formated_message;
  asprintf(&formated_message, "[%s] %s %s", timestamp, lvl_str, message);
  puts(formated_message);
  free(formated_message);
}
