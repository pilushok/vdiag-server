#ifndef __MOCK_WRITE_H__
#define __MOCK_WRITE_H__

#include <sys/types.h>

// Mock write status
enum test_write_status {
    WRITE_NO_ERROR,
    WRITE_ERROR,
    WRITE_PARTIAL
};

// Global mock variables
extern enum test_write_status G_WRITE_STATUS;
extern ssize_t G_MOCK_WRITE_RESULT;

#endif // __MOCK_WRITE_H__
