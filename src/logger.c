#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

LogLevel used_log_level = LOG_LEVEL_INFO;

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

  if (strlen(message) >= MAX_MESSAGE_LEN) {
    DEV_ERROR("Log message too long");
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

void log_fmessage(LogLevel lvl, const char *format_string, ...) {
  if (!log_lvl_to_str(lvl)) {
    DEV_ERROR("Non-standard LogLevel")
    return;
  }

  if (used_log_level > lvl) {
    return;
  }

  if (!format_string) {
    DEV_ERROR("Can't log NULL")
    return;
  }

  char *message;
  va_list args;
  va_start(args, format_string);
  int status = vasprintf(&message, format_string, args);
  va_end(args);

  if (status <= 0) {
    DEV_ERROR("vasprintf errored");
    return;
  }

  log_message(lvl, message);
}

void set_log_level(LogLevel lvl) { used_log_level = lvl; }
