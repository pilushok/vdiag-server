#pragma once

#include <stddef.h>

enum test_calloc_status {
    CALLOC_NO_ERROR = 0,
    CALLOC_FAIL     = 1
};


void *__real_calloc(size_t nmemb, size_t size);

void *__wrap_calloc(size_t nmemb, size_t size);
