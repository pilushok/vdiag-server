#include "mock_calloc.h"

void *__wrap_calloc(size_t nmemb, size_t size)
{
    switch (G_CALLOC_STATUS) {
    case CALLOC_NO_ERROR:
        return __real_calloc(nmemb, size);
        break;
    case CALLOC_FAIL:
        return NULL;
    }
}
