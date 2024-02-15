/*
 * alt.cpp/tests/test_log.c
 *
 * Build:
 *   gcc -o test_log tests/test_log.c -lpthread
 * Run:
 *   ./test_log
 * Expected output:
 *   [DEBUG] Debug message
 *   [INFO] Info message
 *   [WARN] Warning message
 *   [ERROR] Error message
 *   [WARN] This is a warning message
 *   Finished tests!
 * Run: cat test.log
 * Expected output:
 *   [DEBUG] Logging to a file: 1, 2, 3... Done!
 */
#include "../include/log.h"

int main(void) {
    // Test logging to a file
    const char*    file_path   = "test.log";
    struct Logger* file_logger = create_logger(LOG_LEVEL_DEBUG, LOG_TYPE_FILE, file_path);
    LOG(file_logger, LOG_LEVEL_DEBUG, "Logging to a file: 1, 2, %d... Done!\n", 3);

    // Test logging at different log levels
    struct Logger* stream_logger = create_logger(LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, NULL);
    LOG(stream_logger, LOG_LEVEL_DEBUG, "Debug message\n");
    LOG(stream_logger, LOG_LEVEL_INFO, "Info message\n");
    LOG(stream_logger, LOG_LEVEL_WARN, "Warning message\n");
    LOG(stream_logger, LOG_LEVEL_ERROR, "Error message\n");

    // Test changing log level
    stream_logger->log_level = LOG_LEVEL_WARN;
    LOG(stream_logger, LOG_LEVEL_INFO, "This message should not be logged\n");
    LOG(stream_logger, LOG_LEVEL_WARN, "This is a warning message\n");

    // Clean up
    destroy_logger(file_logger);
    destroy_logger(stream_logger);

    puts("Finished tests!");

    return 0;
}
