#ifndef __MOCK_READ_H__
#define __MOCK_READ_H__

#include <sys/types.h>

// Mock read status enums
enum test_read_status {
    READ_NO_ERROR,
    READ_ERROR,
    READ_PARTIAL,
    READ_EOF
};

// Global mock variables
extern enum test_read_status G_READ_STATUS;
extern ssize_t G_MOCK_READ_RESULT;

#endif // __MOCK_READ_H__
