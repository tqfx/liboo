#ifndef __TEST_QUE_H__
#define __TEST_QUE_H__

#include "../src/o_que_o.h"
#include "test.h"

#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

o_static_o void test(void)
{
    o_que_s *ctx = o_que_o_new();
    for (int i = 5; i--;)
    {
        o_T_o *obj = o_que_o_push_fore(ctx);
        if (obj)
        {
            obj->i = i;
        }
    }
    for (int i = 5; i != 10; ++i)
    {
        o_T_o *obj = o_que_o_push_back(ctx);
        if (obj)
        {
            obj->i = i;
        }
    }
    {
        o_T_o *obj = o_que_o_insert(ctx, 0);
        if (obj)
        {
            obj->i = INT_MAX;
        }
    }
    {
        o_T_o *obj = o_que_o_insert(ctx, SIZE_MAX);
        if (obj)
        {
            obj->i = INT_MAX;
        }
    }
    (void)(o_que_o_remove(ctx, 0));
    (void)(o_que_o_remove(ctx, SIZE_MAX));
    o_que_o_swap(ctx, 0, SIZE_MAX);
    {
        o_T_o *lhs = o_que_o_fore(ctx);
        o_T_o *rhs = o_que_o_back(ctx);
        printf("%i %i ", lhs->i, rhs->i);
    }
    {
        o_T_o *lhs = o_que_o_at(ctx, 0);
        o_T_o *rhs = o_que_o_at(ctx, ~0);
        printf("%i %i\n", lhs->i, rhs->i);
    }
    o_que_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');
    o_que_o_foreach_reverse(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');
    for (int i = 0; i != 5; ++i)
    {
        (void)(o_que_o_pull_back(ctx));
    }
    for (int i = 0; i != 5; ++i)
    {
        (void)(o_que_o_pull_fore(ctx));
    }
    o_que_o_die(ctx);
}

#include <time.h>

o_static_o int cmp(const void *lhs, const void *rhs)
{
    return o_cast_s(const o_T_o *, lhs)->i - o_cast_s(const o_T_o *, rhs)->i;
}

o_static_o int cmpr(const void *lhs, const void *rhs)
{
    return o_cast_s(const o_T_o *, rhs)->i - o_cast_s(const o_T_o *, lhs)->i;
}

o_static_o void test_sort(void)
{
    unsigned int t = o_cast_s(unsigned int, time(o_null_o));
    o_que_s *ctx = o_que_o_new();

    srand(t);
    // o_que_o_drop(ctx);
    for (int i = 0; i != 10; ++i)
    {
        o_T_o *obj = o_que_o_push_fore(ctx);
        if (obj)
        {
            obj->i = rand() % 10;
            printf("%i ", obj->i);
            o_que_o_sort_fore(ctx, cmp);
        }
    }
    printf("-> ");
    o_que_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    srand(t);
    o_que_o_drop(ctx);
    for (int i = 0; i != 10; ++i)
    {
        o_T_o *obj = o_que_o_push_back(ctx);
        if (obj)
        {
            obj->i = rand() % 10;
            printf("%i ", obj->i);
            o_que_o_sort_back(ctx, cmpr);
        }
    }
    printf("-> ");
    o_que_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    o_que_o_die(ctx);
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
int main_c(void);
int main_cc(void);
#if defined(__cplusplus)
} /* extern "C" */
#define test_main main_cc
#else /* !__cplusplus */
#define test_main main_c
#endif /* __cplusplus */
int test_main(void)
{
    printf("%s\n", __func__);
    test();
    test_sort();
    return o_success_o;
}

#endif /* __TEST_QUE_H__ */
