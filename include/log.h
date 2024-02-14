// alt.cpp/include/log.h

#ifndef ALT_LOGGER
#define ALT_LOGGER

#include <stdarg.h> // For variadic function support
#include <stdio.h>
#include <stdlib.h> // For memory allocation support

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

struct Logger {
    LogLevel level;
    FILE*    file;
};

struct Logger create_logger(const char* log_file_path, LogLevel log_level) {
    struct Logger logger;

    logger.level = log_level;

    if (log_file_path != NULL) {
        logger.file = fopen(log_file_path, "w");

        if (logger.file == NULL) {
            fprintf(stderr, "Failed to open log file: %s\n", log_file_path);
            logger.file = stderr; // Fallback to stderr if file opening fails
        }
    } else {
        logger.file = stderr;
    }

    return logger;
}

void close_logger(struct Logger logger) {
    if (logger.file != NULL && logger.file != stderr) {
        fclose(logger.file);
    }
    logger.file = NULL;
}

void log_message(
    struct Logger logger, LogLevel log_level, const char* format, ...
) {
    if (logger.level < log_level) {
        return; // Do not log messages below the current log level
    }

    // Prefix log messages based on the level
    switch (logger.level) {
        case LOG_LEVEL_DEBUG:
            fprintf(logger.file, "[DEBUG] ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(logger.file, "[INFO] ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(logger.file, "[WARN] ");
            break;
        case LOG_LEVEL_ERROR:
            fprintf(logger.file, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(logger.file, format, args);
    va_end(args);

    fflush(logger.file); // Ensure the message is written immediately
}

#define LOG(logger, level, format, ...)                                        \
    log_message((logger), (level), (format), ##__VA_ARGS__)
#endif // ALT_LOGGER
