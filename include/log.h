// alt.cpp/include/log.h

#ifndef ALT_LOGGER
#define ALT_LOGGER

#include <stdarg.h> // For variadic function support
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // For memory allocation support

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

// Logger is our "class" definition.
struct Logger {
    LogLevel level;
    FILE*    file;
};

// new_logger is a generic "helper" function for dynamically creating
// loggers on the fly.
struct Logger* new_logger(void) {
    struct Logger* logger = (struct Logger*)malloc(sizeof(struct Logger));

    if (logger == NULL) {
        fprintf(stderr, "Failed to allocate memory for logger\n");
        return NULL;
    }

    // Set sane default values
    logger->level = LOG_LEVEL_DEBUG;
    logger->file  = NULL;

    return logger;
}

// create_logger is our "constructor" function.
struct Logger* create_logger(const char* file_path, LogLevel log_level) {
    struct Logger* logger = new_logger();

    if (logger == NULL) {
        return NULL;
    }

    logger->level = log_level;

    if (file_path == NULL) {
        logger->file = stderr;
    } else {
        logger->file = fopen(file_path, "w");
    }

    if (logger->file == NULL) {
        fprintf(stderr, "[ERROR] Failed to open log file: %s\n", file_path);
        logger->file = stderr; // Fallback to stderr if file opening fails
    }

    return logger;
}

// close_logger is our "destructor" function.
void close_logger(struct Logger* logger) {
    if (logger != NULL) {
        if (logger->file != NULL && logger->file != stderr) {
            fclose(logger->file);
        }
        free(logger);
    }
}

// normally, log_message would simply be a method, but is a function in
// implementation and use to keep up with the theme.
bool log_message(
    struct Logger* logger, LogLevel log_level, const char* format, ...
) {
    if (logger->level < log_level) {
        return false; // Do not log messages below the current log level
    }

    // Prefix log messages based on the level
    switch (log_level) {
        case LOG_LEVEL_DEBUG:
            fprintf(logger->file, "[DEBUG] ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(logger->file, "[INFO] ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(logger->file, "[WARN] ");
            break;
        case LOG_LEVEL_ERROR:
            fprintf(logger->file, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(logger->file, format, args);
    va_end(args);

    fflush(logger->file); // Ensure the message is written immediately

    return true;
}

#define LOG(logger, level, format, ...)                                        \
    log_message((logger), (level), (format), ##__VA_ARGS__)
#endif // ALT_LOGGER
