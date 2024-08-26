#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

const char *log_lvl_to_str(LogLevel lvl) {
  switch (lvl) {
  case LOG_LEVEL_DEBUG:
    return ANSI_COLOR_CYAN "DEBUG" ANSI_COLOR_RESET;
  case LOG_LEVEL_INFO:
    return "INFO";
  case LOG_LEVEL_WARNING:
    return ANSI_COLOR_YELLOW "WARNING" ANSI_COLOR_RESET;
  case LOG_LEVEL_ERROR:
    return ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET;
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
    fprintf(stderr, ANSI_COLOR_RED "Log message too long\n" ANSI_COLOR_RESET);
    return;
  }

  size_t timestamp_len = 20;
  char timestamp[timestamp_len];
  set_timestamp(timestamp, timestamp_len);

  char *formated_message;
  asprintf(&formated_message, "[%s %s] %s", timestamp, lvl_str, message);
  puts(formated_message);
  free(formated_message);
}
