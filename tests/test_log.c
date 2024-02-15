/*
 * alt.cpp/tests/test_log.c
 */
#include "../include/log.h"

int main(void) {
    const char*    file_path = NULL;
    struct Logger* logger    = create_logger(LOG_LEVEL_DEBUG, LOG_TYPE_STREAM, file_path);

    LOG(logger, LOG_LEVEL_DEBUG, "This is just a test: 1, 2, %d... Done!\n", 3);

    puts("Finished tests!");

    return 0;
}
