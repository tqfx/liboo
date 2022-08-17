#ifndef __TEST_TEST_H__
#define __TEST_TEST_H__

#include "../src/oo.h"
#include <stdio.h>

#define TEST_IS_TRUE(expression, message)             \
    do                                                \
    {                                                 \
        if (!(expression))                            \
        {                                             \
            fprintf(stderr, "%s: file %s, line %i\n", \
                    message, __FILE__, __LINE__);     \
        }                                             \
    } while (0)

#define TEST_IS_FASLE(expression, message)            \
    do                                                \
    {                                                 \
        if (expression)                               \
        {                                             \
            fprintf(stderr, "%s: file %s, line %i\n", \
                    message, __FILE__, __LINE__);     \
        }                                             \
    } while (0)

#define TEST_BUG(expression) TEST_IS_TRUE(expression, "bug")

#endif /* __TEST_TEST_H__ */
