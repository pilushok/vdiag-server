#include "mock_if_nametoindex.h"

extern enum test_if_nametoindex_status G_IF_NAMETOINDEX_STATUS;
extern int                             G_MOCK_IFINDEX;

int __wrap_if_nametoindex(const char *ifname)
{
    switch (G_IF_NAMETOINDEX_STATUS) {
    case IF_NAMETOINDEX_NO_ERROR:
        return __real_if_nametoindex(ifname);
    case IF_NAMETOINDEX_FAIL:
        return -1;
    case IF_NAMETOINDEX_SPEC:
        return G_MOCK_IFINDEX;
    }
}
