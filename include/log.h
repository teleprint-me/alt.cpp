// alt.cpp/include/log.h

#ifndef ALT_LOGGER
#define ALT_LOGGER

#include <stdarg.h> // For variadic function support
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // For memory allocation support

/**
 * @brief Enumeration representing different levels of logging.
 */
typedef enum {
    LOG_LEVEL_DEBUG, /**< Debug level logging. */
    LOG_LEVEL_INFO,  /**< Information level logging. */
    LOG_LEVEL_WARN,  /**< Warning level logging. */
    LOG_LEVEL_ERROR  /**< Error level logging. */
} LogLevel;

/**
 * @brief Structure representing a logger object.
 */
struct Logger {
    LogLevel level; /**< The logging level of the logger. */
    FILE*    file;  /**< The file to which log messages are written. */
};

/**
 * @brief Creates a new logger instance.
 *
 * This function dynamically allocates memory for a new logger instance
 * and initializes it with sane default values.
 *
 * @return A pointer to the newly created logger instance, or NULL if memory
 * allocation fails.
 */
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

/**
 * @brief Creates a new logger instance with the specified log file path and log
 * level.
 *
 * This function creates a new logger instance and initializes it with the
 * specified log level. If a log file path is provided, the logger will attempt
 * to open the file for writing. If the file opening fails, the logger will fall
 * back to writing log messages to stderr.
 *
 * @param file_path The path to the log file. Pass NULL to log messages to
 * stderr.
 * @param log_level The desired log level for the logger.
 *
 * @return A pointer to the newly created logger instance, or NULL if memory
 * allocation fails or if the specified log file cannot be opened.
 */
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

/**
 * @brief Destroys a logger instance and releases associated resources.
 *
 * This function closes the log file associated with the logger, if any,
 * and frees the memory allocated for the logger instance.
 *
 * @param logger A pointer to the logger instance to be destroyed.
 */
void close_logger(struct Logger* logger) {
    if (logger != NULL) {
        if (logger->file != NULL && logger->file != stderr) {
            fclose(logger->file);
        }
        free(logger);
    }
}

/**
 * @brief Logs a message with the specified log level to the logger's file.
 *
 * This function logs a message with the specified log level to the logger's
 * file. If the logger's log level is lower than the specified log level, the
 * message will not be logged.
 *
 * @param logger A pointer to the logger instance to use for logging.
 * @param log_level The log level of the message to be logged.
 * @param format The format string of the message to be logged.
 * @param ... Additional arguments for formatting the message (optional).
 *
 * @return true if the message was successfully logged, false otherwise.
 */
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

/**
 * @brief Macro for logging messages using a logger instance.
 *
 * This macro provides a convenient shorthand for logging messages using a
 * logger instance. It calls the log_message function with the specified logger,
 * log level, and message format.
 *
 * @param logger A pointer to the logger instance to use for logging.
 * @param level The log level of the message to be logged.
 * @param format The format string of the message to be logged.
 * @param ... Additional arguments for formatting the message (optional).
 *
 * Example usage:
 * @code{.cpp}
 * LOG(my_logger, LOG_LEVEL_DEBUG, "Debug message: %s\n", "Hello, world!");
 * @endcode
 */
#define LOG(logger, level, format, ...)                                        \
    log_message((logger), (level), (format), ##__VA_ARGS__)

#endif // ALT_LOGGER
