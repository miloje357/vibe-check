#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdbool.h>
#include <stdio.h>
typedef enum {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
} LogLevel;

typedef int logger_id;

#define MAX_MESSAGE_LEN 4096

/**
 * Logs a message that can be formated (like printf) to the loggers (if any are
 * initialized)
 * @param lvl - the log level (debug, info, warning, error) (if the logger's
 *              level is greater than the message's, nothing will be logged)
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
  fprintf(stderr, "\x1b[31m" message "\x1b[0m\n", ##__VA_ARGS__)

/**
 * Initializes a logger
 * @param output - the file to which to log to
 * @param lvl - the logger's log level (any messages that have a lesser log
 *              level won't be logged)
 * @param should_color - set to true if outputing to the terminal (stdout,
 *                       stderr)
 * @return a logger id that is used for closing the logger or -1 if no logger
 *         could be initialized
 * @see tests/check_logger.c
 * @since 0.1.0
 */
logger_id init_logger(FILE *output, LogLevel lvl, bool should_color);

/**
 * Closes a logger (if it's initialized)
 * @param id - id of the logger to close
 * @return true if the logger was closed, false otherwise
 * @see tests/check_logger.c
 * @since 0.1.0
 */
bool close_logger(logger_id id);

#endif
