#ifndef MY_ASSERT_H
#define MY_ASSERT_H

#include <stdlib.h>

#define MY_ASSERT(condition, message) \
    if (!(condition)) \
    { \
        fprintf(stderr, "%s file: %s, function: %s, line: %d.\n", \
               message, __FILE__, __FUNCTION__, __LINE__); \
        abort(); \
    }

#endif
