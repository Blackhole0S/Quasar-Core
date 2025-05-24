// include/logging.h

#ifndef LOGGING_H
#define LOGGING_H

typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

void logging_init(const char *ident);
void log_message(LogLevel level, const char *fmt, ...);

// Convenience macros
#define log_info(...) log_message(LOG_LEVEL_INFO, __VA_ARGS__)
#define log_warning(...) log_message(LOG_LEVEL_WARNING, __VA_ARGS__)
#define log_error(...) log_message(LOG_LEVEL_ERROR, __VA_ARGS__)

#endif // LOGGING_H
