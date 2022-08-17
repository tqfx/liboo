#ifndef __TEST_VEC_H__
#define __TEST_VEC_H__

#include "../src/o_vector_o.h"
#include "test.h"

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

o_static_o void test(void)
{
    {
        o_vector_s *ctx = o_vector_o_new();

        o_vector_o_foreach(it, ctx);
        o_vector_o_foreach_reverse(it, ctx);

        for (int i = 0; i != 10; ++i)
        {
            o_T_o *obj = o_vector_o_push(ctx);
            if (obj)
            {
                obj->i = i;
            }
        }

        {
            o_T_o *end = o_vector_o_end(ctx);
            o_T_o *top = o_vector_o_top(ctx);
            TEST_BUG(end - top == 1);
        }

        o_vector_o_forenum(i, ctx)
        {
            o_T_o *it = o_vector_o_at(ctx, i);
            printf("%i ", it->i);
        }
        putchar('\n');

        o_vector_o_forenum_reverse(i, ctx)
        {
            o_T_o *it = o_vector_o_at(ctx, i);
            printf("%i ", it->i);
        }
        putchar('\n');

        o_vector_s obj[1];
        o_vector_o_copy(obj, ctx);
        o_vector_o_dtor(ctx);
        o_vector_o_move(ctx, obj);

        for (int i = 0; i != 10; ++i)
        {
            o_vector_o_pull(ctx);
        }

        o_vector_o_die(ctx);
    }

    {
        o_vector_s *ctx = o_vector_o_new();

        for (int i = 5; i != 10; ++i)
        {
            o_T_o *obj = o_vector_o_insert(ctx, o_cast_s(size_t, i));
            if (obj)
            {
                obj->i = i;
            }
        }
        for (int i = 0; i != 5; ++i)
        {
            o_T_o *obj = o_vector_o_insert(ctx, o_cast_s(size_t, i));
            if (obj)
            {
                obj->i = i;
            }
        }

        o_vector_o_swap(ctx, 0, 0);
        o_vector_o_swap(ctx, 0, ~0U);
        o_vector_o_swap(ctx, ~0U, 0);
        o_vector_o_swap(ctx, 4, 6);
        o_vector_o_swap(ctx, 6, 4);

        o_vector_o_foreach(it, ctx)
        {
            printf("%i ", it->i);
        }
        putchar('\n');

        o_vector_o_foreach_reverse(it, ctx)
        {
            printf("%i ", it->i);
        }
        putchar('\n');

        for (int i = 0; i != 5; ++i)
        {
            o_T_o *obj = o_vector_o_remove(ctx, o_cast_s(size_t, i));
            if (obj)
            {
                printf("%i ", obj->i);
            }
        }
        for (int i = 0; i != 5; ++i)
        {
            o_T_o *obj = o_vector_o_remove(ctx, 0);
            if (obj)
            {
                printf("%i ", obj->i);
            }
        }
        putchar('\n');

        o_vector_o_die(ctx);
    }

    {
        o_vector_s *ctx = o_vector_o_new();

        for (int i = 5; i != 10; ++i)
        {
            o_T_o *obj = o_vector_o_push_back(ctx);
            if (obj)
            {
                obj->i = i;
            }
        }
        for (int i = 5; i != 10; ++i)
        {
            o_T_o *obj = o_vector_o_pull_back(ctx);
            if (obj)
            {
                printf("%i ", obj->i);
            }
        }
        for (int i = 0; i != 5; ++i)
        {
            o_T_o *obj = o_vector_o_push_fore(ctx);
            if (obj)
            {
                obj->i = i;
            }
        }
        for (int i = 0; i != 5; ++i)
        {
            o_T_o *obj = o_vector_o_pull_fore(ctx);
            if (obj)
            {
                printf("%i ", obj->i);
            }
        }
        putchar('\n');

        o_vector_o_die(ctx);
    }
}

#include <time.h>

o_static_o int cmp(const o_T_o *lhs, const o_T_o *rhs)
{
    return lhs->i - rhs->i;
}

o_static_o int cmpr(const o_T_o *lhs, const o_T_o *rhs)
{
    return rhs->i - lhs->i;
}

o_static_o void test_sort(void)
{
    unsigned int t = o_cast_s(unsigned int, time(o_null_o));
    o_vector_s *ctx = o_vector_o_new();

    o_vector_o_make(ctx, 10);

    srand(t);
    o_vector_o_set(ctx, cmpr);
    o_vector_o_foreach(it, ctx)
    {
        it->i = rand() % 10;
        printf("%i ", it->i);
    }
    printf("-> ");
    o_vector_o_sort(ctx);
    o_vector_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    srand(t);
    o_vector_o_set(ctx, cmp);
    o_vector_o_foreach(it, ctx)
    {
        it->i = rand() % 10;
        printf("%i ", it->i);
    }
    printf("-> ");
    o_vector_o_sort(ctx);
    o_vector_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    srand(t);
    o_vector_o_drop(ctx);
    for (int i = 0; i != 10; ++i)
    {
        o_T_o *obj = o_vector_o_push_fore(ctx);
        if (obj)
        {
            obj->i = rand() % 10;
            printf("%i ", obj->i);
            o_vector_o_sort_fore(ctx);
        }
    }
    printf("-> ");
    o_vector_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    srand(t);
    o_vector_o_drop(ctx);
    for (int i = 0; i != 10; ++i)
    {
        o_T_o *obj = o_vector_o_push_back(ctx);
        if (obj)
        {
            obj->i = rand() % 10;
            printf("%i ", obj->i);
            o_vector_o_sort_back(ctx);
        }
    }
    printf("-> ");
    o_vector_o_foreach(it, ctx)
    {
        printf("%i ", it->i);
    }
    putchar('\n');

    {
        char bufok[32];
        char bufno[32];
        char *ok = bufok;
        char *no = bufno;
        ok += sprintf(bufok, "%s ", "finding");
        no += sprintf(bufno, "%s ", "missing");
        o_T_o key[1];
        for (int i = 0; i != 10; ++i)
        {
            key->i = i;
            o_T_o *obj = o_vector_o_search(ctx, key);
            if (obj)
            {
                ok += sprintf(ok, "%i ", obj->i);
            }
            else
            {
                no += sprintf(no, "%i ", i);
            }
        }
        printf("%s\n%s\n", bufok, bufno);
    }

    o_vector_o_die(ctx);
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

#endif /* __TEST_VEC_H__ */
