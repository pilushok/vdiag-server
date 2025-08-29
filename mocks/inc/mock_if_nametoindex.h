#pragma once
#include <stddef.h>

enum test_if_nametoindex_status {
    IF_NAMETOINDEX_NO_ERROR = 0,
    IF_NAMETOINDEX_FAIL     = 1,
    IF_NAMETOINDEX_SPEC     = 2
};
// extern enum test_if_nametoindex_status G_IF_NAMETOINDEX_STATUS;
// extern int                             G_MOCK_IFINDEX;

int __real_if_nametoindex(const char *ifname);

int __wrap_if_nametoindex(const char *ifname);
