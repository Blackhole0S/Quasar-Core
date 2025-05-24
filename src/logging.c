// src/logging.c

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <syslog.h>
#include <pthread.h>
#include "logging.h"

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
static int log_initialized = 0;

void logging_init(const char *ident) {
    openlog(ident, LOG_PID | LOG_CONS, LOG_USER);
    log_initialized = 1;
}

static const char *level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_INFO: return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void log_message(LogLevel level, const char *fmt, ...) {
    pthread_mutex_lock(&log_mutex);

    // Get current time for timestamp
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_r(&now, &tm_info);
    char time_str[20];  // YYYY-MM-DD HH:MM:SS
    strftime(time_str, sizeof(time_str), "%F %T", &tm_info);

    // Format message
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    // Output to stderr with timestamp
    fprintf(stderr, "%s [%s] %s\n", time_str, level_to_string(level), msg);

    // Also send to syslog if initialized
    if (log_initialized) {
        int syslog_level;
        switch (level) {
            case LOG_LEVEL_INFO: syslog_level = LOG_INFO; break;
            case LOG_LEVEL_WARNING: syslog_level = LOG_WARNING; break;
            case LOG_LEVEL_ERROR: syslog_level = LOG_ERR; break;
            default: syslog_level = LOG_INFO; break;
        }
        syslog(syslog_level, "%s", msg);
    }

    pthread_mutex_unlock(&log_mutex);
}
