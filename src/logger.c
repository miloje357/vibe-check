#include "logger.h"
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define MAX_LOGGERS 8

typedef struct Logger {
  logger_id id;
  FILE *output;
  LogLevel level;
  bool should_color;
} Logger;

Logger *loggers[MAX_LOGGERS] = {NULL};

void free_message_variables(char *formated_message, char *lvl_str) {
  free(formated_message);
  if (strlen(lvl_str) != 0) {
    free(lvl_str);
  }
}

void log_lvl_to_str(char **dest, LogLevel lvl, bool should_color) {
  const char *lvl_str = NULL;
  const char *color = "";
  const char *reset = ANSI_COLOR_RESET;
  switch (lvl) {
  case LOG_LEVEL_DEBUG:
    lvl_str = "DEBUG";
    color = ANSI_COLOR_CYAN;
    break;
  case LOG_LEVEL_INFO:
    lvl_str = "INFO";
    break;
  case LOG_LEVEL_WARNING:
    lvl_str = "WARNING";
    color = ANSI_COLOR_YELLOW;
    break;
  case LOG_LEVEL_ERROR:
    lvl_str = "ERROR";
    color = ANSI_COLOR_RED;
    break;
  }
  if (!should_color) {
    color = "";
    reset = "";
  }
  int s = asprintf(dest, "%s%s%s", color, lvl_str, reset);
  if (s <= 0) {
    DEV_ERROR("Cannot convert LogLevel to string: asprintf errored");
  }
}

void set_timestamp(char *str, size_t str_len) {
  time_t rawtime;
  time(&rawtime);
  struct tm *timeinfo = localtime(&rawtime);
  strftime(str, str_len, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void log_message_to_logger(Logger *logger, LogLevel lvl, const char *message) {
  char *lvl_str = "";
  log_lvl_to_str(&lvl_str, lvl, logger->should_color);
  if (strlen(lvl_str) == 0) {
    DEV_ERROR("Couldn't convert LogLevel to string");
  }

  size_t timestamp_len = 20;
  char timestamp[timestamp_len];
  set_timestamp(timestamp, timestamp_len);

  char *formated_message;
  int s =
      asprintf(&formated_message, "[%s %s] %s\n", timestamp, lvl_str, message);
  if (s <= 0) {
    DEV_ERROR("Coudln't allocate memory for log message: Not enough memory");
    free_message_variables(formated_message, lvl_str);
    return;
  }

  s = fputs(formated_message, logger->output);
  if (s <= 0) {
    free_message_variables(formated_message, lvl_str);
    DEV_ERROR("Coudn't log message: fputs errored");
    return;
  }

  free_message_variables(formated_message, lvl_str);
}

void log_message(LogLevel lvl, const char *message) {
  for (int i = 0; i < MAX_LOGGERS; i++) {
    if (!loggers[i] || loggers[i]->level > lvl) {
      continue;
    }
    log_message_to_logger(loggers[i], lvl, message);
  }
}

void log_fmessage(LogLevel lvl, const char *format_string, ...) {
  if (lvl < 0 || lvl >= 4) {
    DEV_ERROR("Couldn't log message: Non-standard LogLevel");
    return;
  }

  if (!format_string) {
    DEV_ERROR("Couldn't log message: Can't log NULL");
    return;
  }

  char *message;
  va_list args;
  va_start(args, format_string);
  int status = vasprintf(&message, format_string, args);
  va_end(args);

  if (status <= 0) {
    DEV_ERROR("Couldn't log message: vasprintf error");
    free(message);
    return;
  }

  if (strlen(message) >= MAX_MESSAGE_LEN) {
    DEV_ERROR("Couldn't log message: Log message too long");
    free(message);
    return;
  }
  log_message(lvl, message);
  free(message);
}

Logger *get_logger(FILE *output, LogLevel lvl, bool should_color) {
  Logger *logger = malloc(sizeof(Logger));
  logger->level = lvl;
  logger->output = output;
  logger->should_color = should_color;
  return logger;
}

bool open_logfile(const char *log_filepath, FILE **log_file,
                  bool *should_color) {
  if (memcmp(log_filepath, "stdout", 7) == 0) {
    *log_file = stdout;
    *should_color = true;
    return true;
  }
  if (memcmp(log_filepath, "stderr", 7) == 0) {
    *log_file = stderr;
    *should_color = true;
    return true;
  }

  FILE *file = fopen(log_filepath, "w");
  if (!file) {
    DEV_ERROR("Coudln't open %s: %s", log_filepath, strerror(errno));
    return false;
  }
  *log_file = file;
  *should_color = false;
  return true;
}

logger_id init_logger(const char *output, LogLevel lvl) {
  if (!output) {
    DEV_ERROR("init_logger: output cannot be null");
    return -1;
  }
  if (lvl < 0 || lvl >= 4) {
    DEV_ERROR("init_logger: Non-standard LogLevel");
    return -1;
  }

  int next_logger_place = 0;
  while (loggers[next_logger_place]) {
    next_logger_place++;
    if (next_logger_place >= MAX_LOGGERS) {
      DEV_ERROR("Cannot init more loggers");
      return -1;
    }
  }

  FILE *log_file;
  bool should_color;
  bool s = open_logfile(output, &log_file, &should_color);
  if (!s) {
    return -1;
  }

  Logger *logger = get_logger(log_file, lvl, should_color);
  logger->id = next_logger_place;
  loggers[next_logger_place] = logger;
  return logger->id;
}

bool close_logger(logger_id id) {
  for (int i = 0; i < MAX_LOGGERS; i++) {
    if (loggers[i] && loggers[i]->id == id) {
      fclose(loggers[i]->output);
      free(loggers[i]);
      loggers[i] = NULL;
      return true;
    }
  }
  return false;
}

void close_all_loggers() {
  for (int i = 0; i < MAX_LOGGERS; i++) {
    if (!loggers[i]) {
      continue;
    }
    fclose(loggers[i]->output);
    free(loggers[i]);
    loggers[i] = NULL;
  }
}
