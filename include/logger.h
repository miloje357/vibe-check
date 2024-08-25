#ifndef LOGGER_H_
#define LOGGER_H_

typedef enum {
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
} LogLevel;

#define MAX_MESSAGE_LEN 4096

void log_message(LogLevel lvl, const char *message);

#define LOG_DEBUG(message) log_message(LOG_LEVEL_DEBUG, message)
#define LOG_INFO(message) log_message(LOG_LEVEL_INFO, message)
#define LOG_WARNING(message) log_message(LOG_LEVEL_WARNING, message)
#define LOG_ERROR(message) log_message(LOG_LEVEL_ERROR, message)

#endif
