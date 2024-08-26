#ifndef LOGGER_H_
#define LOGGER_H_

typedef enum {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
} LogLevel;

#define MAX_MESSAGE_LEN 4096

/**
 * Logs a message that can be formated (like printf) to a logger
 * @param lvl - the log level (debug, info, warning, error) (see the LogLevel
 * enum)
 * @param format_string - the message to be logged
 * @param ... - a list of variables to be put in the format_string
 * @see tests/check_logger.c
 * @since 0.1.0
 */
void log_fmessage(LogLevel lvl, const char *format_string, ...)
    __attribute__((format(printf, 2, 3)));

#define LOG_DEBUG(message, ...)                                                \
  log_fmessage(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define LOG_INFO(message, ...)                                                 \
  log_fmessage(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)                                              \
  log_fmessage(LOG_LEVEL_WARNING, message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)                                                \
  log_fmessage(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define DEV_ERROR(message, ...)                                                \
  fprintf(stderr, "\x1b[31m" message "\x1b[0m\n", ##__VA_ARGS__);

void set_log_level(LogLevel lvl);

#endif
