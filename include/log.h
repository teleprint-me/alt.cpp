/*
 * alt.cpp/include/log.h
 *
 * Copyright © 2024 Austin Berrio
 */

#ifndef ALT_LOGGER
#define ALT_LOGGER

#include <pthread.h> // For including mutex functions
#include <stdarg.h>  // For variadic function support
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
 * @brief Enumeration representing different types of logging.
 */
typedef enum {
    LOG_TYPE_UNKNOWN, /**< Unknown log type. */
    LOG_TYPE_STREAM,  /**< Log to a stream (e.g., stdout or stderr). */
    LOG_TYPE_FILE     /**< Log to a file. */
} LogType;

char* LogTypeName[] = {"unknown", "stream", "file"};

/**
 * @brief Structure representing a logger object.
 */
struct Logger {
    LogLevel        log_level;     /**< The logging level of the logger. */
    LogType         log_type;      /**< The type of logger. */
    const char*     log_type_name; /**< The name associated with the logger type. */
    FILE*           file_stream;   /**< The file stream for writing log messages. */
    const char*     file_path;     /**< The path to the log file. */
    pthread_mutex_t thread_lock;   /**< Mutex to ensure thread-safe logging. */
};

/**
 * @brief Sets the logger type and name.
 *
 * This function validates and sets the logger type and name
 * based on the provided logger type.
 *
 * @param logger The logger structure to update.
 * @param log_type The desired logger type.
 *
 * @return True if the type and name were set successfully, false otherwise.
 */
bool set_logger_type_and_name(struct Logger* logger, LogType log_type) {
    // Set logger type based on provided logger type
    switch (log_type) {
        case LOG_TYPE_UNKNOWN:
        case LOG_TYPE_STREAM:
            logger->log_type      = LOG_TYPE_STREAM;
            logger->log_type_name = LogTypeName[LOG_TYPE_STREAM];
            return true;
        case LOG_TYPE_FILE:
            logger->log_type      = LOG_TYPE_FILE;
            logger->log_type_name = LogTypeName[LOG_TYPE_FILE];
            return true;
        default:
            fprintf(stderr, "Invalid logger type\n");
            return false;
    }
}

/**
 * @brief Sets the file path and stream for the logger.
 *
 * This function sets the file path and stream for the logger based on the
 * provided file path. If the file path is NULL, the logger stream is set to
 * stderr, and false is returned to indicate a user error. If the file path
 * is not NULL, an attempt is made to open the file for writing. If the file
 * opening fails, the logger stream is set to stderr, and false is returned
 * to indicate an unexpected condition. If the file path is set successfully,
 * true is returned.
 *
 * @param logger The logger structure to update.
 * @param file_path The path to the log file. Pass NULL to log messages to
 * stderr.
 *
 * @return True if the file path was set successfully, false otherwise.
 */
bool set_logger_file_path_and_stream(struct Logger* logger, const char* file_path) {
    if (file_path == NULL) {
        // set the logger type to stream upon failure.
        set_logger_type_and_name(logger, LOG_TYPE_STREAM);
        logger->file_stream = stderr;
        return false; // File path is NULL (user error)
    }

    logger->file_stream = fopen(file_path, "w");
    if (logger->file_stream == NULL) {
        // set the logger type to stream upon failure.
        set_logger_type_and_name(logger, LOG_TYPE_STREAM);
        logger->file_stream = stderr;
        return false; // Failed to open file (unexpected condition)
    }

    // File path set successfully
    logger->file_path = file_path;
    return true;
}

/**
 * @brief Creates a new logger instance.
 *
 * This function dynamically allocates memory for a new logger instance
 * and initializes it with sane default values.
 *
 * @param log_type The desired logger type.
 *
 * @return A pointer to the newly created logger instance, or NULL if memory
 * allocation fails or if the logger type is invalid.
 */
struct Logger* new_logger(LogType log_type) {
    // Allocate memory for the logger instance
    struct Logger* logger = (struct Logger*)malloc(sizeof(struct Logger));

    // Check if memory allocation was successful
    if (NULL == logger) {
        fprintf(stderr, "Failed to allocate memory for logger\n");
        return NULL;
    }

    // Set default values for the logger
    logger->log_level = LOG_LEVEL_DEBUG;

    // Set logger type and name
    if (!set_logger_type_and_name(logger, log_type)) {
        fprintf(stderr, "Failed to initialize logger with type: %d\n", log_type);
        free(logger); // Clean up allocated memory to avoid memory leaks
        return NULL;
    }

    logger->file_path   = NULL;
    logger->file_stream = NULL;

    // Initialize the mutex for thread safety
    int error_code = pthread_mutex_init(&logger->thread_lock, NULL);
    if (0 != error_code) {
        fprintf(stderr, "Failed to initialize mutex with error: %d\n", error_code);
        free(logger); // Clean up allocated memory to avoid memory leaks
        return NULL;
    }

    return logger; // Return the created logger instance
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
 * @param log_level The desired log level for the logger.
 * @param log_type The desired log type for the logger.
 * @param file_path The path to the log file. Pass NULL to log messages to
 * stderr.
 *
 * @return A pointer to the newly created logger instance, or NULL if memory
 * allocation fails or if the specified log file cannot be opened.
 */
struct Logger* create_logger(LogLevel log_level, LogType log_type, const char* file_path) {
    // Create a new logger instance
    struct Logger* logger = new_logger(log_type);
    if (logger == NULL) {
        return NULL;
    }

    // Set the log level
    logger->log_level = log_level;

    // Set the file path and stream based on the logger type
    switch (log_type) {
        case LOG_TYPE_UNKNOWN:
        case LOG_TYPE_STREAM:
            // Use stdout or stderr directly
            logger->file_stream = stderr;
            break;
        case LOG_TYPE_FILE:
            if (!set_logger_file_path_and_stream(logger, file_path)) {
                fprintf(stderr, "Failed to set log file path. Fallback to stderr.\n");
            }
            break;
        default:
            // Unknown logger type; fallback to stderr
            fprintf(stderr, "Unknown logger type. Fallback to stderr.\n");
            logger->file_stream = stderr;
            break;
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
 * @return True if the logger was successfully destroyed, false otherwise.
 */
bool destroy_logger(struct Logger* logger) {
    if (NULL == logger) {
        return false;
    }

    // Close the log file if it's a file logger
    if (LOG_TYPE_FILE == logger->log_type && NULL != logger->file_stream) {
        if (fclose(logger->file_stream) != 0) {
            fprintf(stderr, "Failed to close log file: %s\n", logger->file_path);
            return false; // return false to prevent dangling pointers
        }
    }

    // Destroy the mutex
    int mutex_error = pthread_mutex_destroy(&logger->thread_lock);
    if (0 != mutex_error) {
        fprintf(stderr, "Failed to destroy mutex with error: %d\n", mutex_error);
        return false; // return false to prevent dangling pointers
    }

    // Free memory for the logger instance
    free(logger);
    return true;
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
bool log_message(struct Logger* logger, LogLevel log_level, const char* format, ...) {
    // block if and only if the logger->log_level is greater than the specified log_level
    if (logger->log_level > log_level) {
        return false; // Do not log messages below the current log level
    }

    // Only lock the thread if log_level is valid!
    pthread_mutex_lock(&logger->thread_lock);

    // Prefix log messages based on the level
    switch (log_level) {
        case LOG_LEVEL_DEBUG:
            fprintf(logger->file_stream, "[DEBUG] ");
            break;
        case LOG_LEVEL_INFO:
            fprintf(logger->file_stream, "[INFO] ");
            break;
        case LOG_LEVEL_WARN:
            fprintf(logger->file_stream, "[WARN] ");
            break;
        case LOG_LEVEL_ERROR:
            fprintf(logger->file_stream, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(logger->file_stream, format, args);
    va_end(args);

    fflush(logger->file_stream); // Ensure the message is written immediately

    pthread_mutex_unlock(&logger->thread_lock);

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
#define LOG(logger, log_level, format, ...) log_message(logger, log_level, format, ##__VA_ARGS__)

#endif // ALT_LOGGER
