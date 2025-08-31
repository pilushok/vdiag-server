#ifndef __MOCK_OPEN_H__
#define __MOCK_OPEN_H__

enum test_open_status {
    OPEN_NO_ERROR,
    OPEN_ERROR,
    OPEN_ACCESS_DENIED,
    OPEN_SPEC,
};

extern enum test_open_status G_OPEN_STATUS;
extern int G_MOCK_FD;

#endif // __MOCK_OPEN_H__
