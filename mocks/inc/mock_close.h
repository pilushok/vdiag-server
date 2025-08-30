#ifndef __MOCK_CLOSE_H__
#define __MOCK_CLOSE_H__

enum test_close_status {
    CLOSE_NO_ERROR,
    CLOSE_ERROR,
    CLOSE_INVALID_FD
};

extern enum test_close_status G_CLOSE_STATUS;

#endif // __MOCK_CLOSE_H__
