/*!
 @file o_vector_o.h
 @brief o_brief_o
 @copyright Copyright (C) 2020-present tqfx, All rights reserved.
*/

#ifndef __OO_VECTOR_H__
#define __OO_VECTOR_H__

#include "oo.h"

#include <stdlib.h>
#include <string.h>

#define o_vector_o_forenum(i, ctx) for (size_t i = 0; i != o_cast_s(size_t, (ctx)->tail - (ctx)->head); ++i)

#define o_vector_o_forenum_reverse(i, ctx) for (size_t i = o_cast_s(size_t, (ctx)->tail - (ctx)->head); i--;)

#define o_vector_o_foreach(it, ctx) for (o_T_o *it = (ctx)->head, *it##_ = (ctx)->tail; it != it##_; ++it)

#define o_vector_o_foreach_reverse(it, ctx)                       \
    for (o_T_o *it##_ = (ctx)->head ? (ctx)->head - 1 : o_null_o, \
               *it = (ctx)->tail ? (ctx)->tail - 1 : o_null_o;    \
         it != it##_; --it)

typedef struct
{
    o_T_o *head;
    o_T_o *tail;
    o_T_o *last;
    size_t num;
    size_t mem;
    int (*cmp)(const o_T_o *, const o_T_o *);
} o_vector_s;

o_inline_o o_T_o *o_vector_o_ptr(const o_vector_s *ctx) { return ctx->head; }

o_inline_o o_T_o *o_vector_o_end(const o_vector_s *ctx) { return ctx->tail; }

o_inline_o size_t o_vector_o_num(const o_vector_s *ctx) { return ctx->num; }

o_inline_o size_t o_vector_o_mem(const o_vector_s *ctx) { return ctx->mem; }

o_inline_o o_T_o *o_vector_o_top(const o_vector_s *ctx)
{
    return ctx->head != ctx->tail ? ctx->tail - 1 : o_null_o;
}

o_inline_o o_T_o *o_vector_o_at(const o_vector_s *ctx, size_t idx)
{
    return ctx->head + idx;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

o_public_o o_vector_s *o_vector_o_new(void);

o_public_o void o_vector_o_die(o_vector_s *ctx);

o_public_o void o_vector_o_ctor(o_vector_s *ctx);

o_public_o void o_vector_o_dtor(o_vector_s *ctx);

o_public_o int o_vector_o_copy(o_vector_s *ctx, const o_vector_s *obj);

o_public_o o_vector_s *o_vector_o_move(o_vector_s *ctx, o_vector_s *obj);

o_public_o void o_vector_o_set(o_vector_s *ctx, int (*cmp)(const o_T_o *, const o_T_o *));

o_public_o int o_vector_o_make(o_vector_s *ctx, size_t num);

o_public_o void o_vector_o_drop(o_vector_s *ctx);

o_public_o void o_vector_o_swap(const o_vector_s *ctx, size_t lhs, size_t rhs);

o_public_o void o_vector_o_sort(const o_vector_s *ctx);

o_public_o void o_vector_o_sort_fore(const o_vector_s *ctx);

o_public_o void o_vector_o_sort_back(const o_vector_s *ctx);

o_public_o o_T_o *o_vector_o_search(const o_vector_s *ctx, const o_T_o *obj);

o_public_o o_T_o *o_vector_o_insert(o_vector_s *ctx, size_t idx);

o_public_o o_T_o *o_vector_o_remove(o_vector_s *ctx, size_t idx);

o_public_o o_T_o *o_vector_o_push_fore(o_vector_s *ctx);

o_public_o o_T_o *o_vector_o_push_back(o_vector_s *ctx);

o_public_o o_T_o *o_vector_o_pull_fore(o_vector_s *ctx);

o_public_o o_T_o *o_vector_o_pull_back(o_vector_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

o_inline_o o_T_o *o_vector_o_push(o_vector_s *ctx)
{
    return o_vector_o_push_back(ctx);
}

o_inline_o o_T_o *o_vector_o_pull(o_vector_s *ctx)
{
    return o_vector_o_pull_back(ctx);
}

#endif /* __OO_VECTOR_H__ */
