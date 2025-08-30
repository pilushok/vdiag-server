#ifndef __MOCK_LSEEK_H__
#define __MOCK_LSEEK_H__

#include <sys/types.h>

enum test_lseek_status {
    LSEEK_NO_ERROR,
    LSEEK_ERROR,
    LSEEK_INVALID_OFFSET
};

extern enum test_lseek_status G_LSEEK_STATUS;
extern off_t G_MOCK_LSEEK_RESULT;

#endif // __MOCK_LSEEK_H__
