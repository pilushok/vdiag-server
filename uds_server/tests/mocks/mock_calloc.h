#include <stddef.h>

enum calloc_status {
    CALLOC_NO_ERROR = 0,
    CALLOC_FAIL     = 1
} G_CALLOC_STATUS = CALLOC_NO_ERROR;

void *__real_calloc(size_t nmemb, size_t size);

void *__wrap_calloc(size_t nmemb, size_t size);
