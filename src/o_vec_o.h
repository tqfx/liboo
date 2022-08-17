#ifndef __OO_VEC_H__
#define __OO_VEC_H__

#include "oo.h"

#include <stdlib.h>
#include <string.h>

#define o_vec_o_forenum(i, ctx) for (size_t i = 0; i != (ctx)->num; ++i)

#define o_vec_o_forenum_reverse(i, ctx) for (size_t i = (ctx)->num; i--;)

#define o_vec_o_foreach(it, ctx) \
    for (o_T_o *it = (ctx)->ptr, *it##_ = it ? it + (ctx)->num : o_null_o; it != it##_; ++it)

#define o_vec_o_foreach_reverse(it, ctx)                         \
    for (o_T_o *it##_ = (ctx)->ptr ? (ctx)->ptr - 1 : o_null_o,  \
               *it = (ctx)->ptr ? it##_ + (ctx)->num : o_null_o; \
         it != it##_; --it)

typedef struct
{
    o_T_o *ptr;
    size_t num;
    size_t mem;
} o_vec_s;

o_inline_o o_T_o *o_vec_o_ptr(const o_vec_s *ctx) { return ctx->ptr; }

o_inline_o size_t o_vec_o_num(const o_vec_s *ctx) { return ctx->num; }

o_inline_o size_t o_vec_o_mem(const o_vec_s *ctx) { return ctx->mem; }

o_inline_o o_T_o *o_vec_o_top(const o_vec_s *ctx)
{
    return ctx->num ? ctx->ptr + ctx->num - 1 : o_null_o;
}

o_inline_o o_T_o *o_vec_o_end(const o_vec_s *ctx)
{
    return ctx->ptr + ctx->num;
}

o_inline_o o_T_o *o_vec_o_at(const o_vec_s *ctx, size_t idx)
{
    return ctx->ptr + idx;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

o_public_o o_vec_s *o_vec_o_new(void);

o_public_o void o_vec_o_die(o_vec_s *ctx);

o_public_o void o_vec_o_ctor(o_vec_s *ctx);

o_public_o void o_vec_o_dtor(o_vec_s *ctx);

o_public_o int o_vec_o_copy(o_vec_s *ctx, const o_vec_s *obj);

o_public_o o_vec_s *o_vec_o_move(o_vec_s *ctx, o_vec_s *obj);

o_public_o int o_vec_o_make(o_vec_s *ctx, size_t num);

o_public_o void o_vec_o_drop(o_vec_s *ctx);

o_public_o void o_vec_o_swap(const o_vec_s *ctx, size_t lhs, size_t rhs);

o_public_o void o_vec_o_sort(const o_vec_s *ctx, int (*cmp)(const void *, const void *));

o_public_o void o_vec_o_sort_fore(const o_vec_s *ctx, int (*cmp)(const void *, const void *));

o_public_o void o_vec_o_sort_back(const o_vec_s *ctx, int (*cmp)(const void *, const void *));

o_public_o o_T_o *o_vec_o_search(const o_vec_s *ctx, const o_T_o *obj, int (*cmp)(const void *, const void *));

o_public_o o_T_o *o_vec_o_insert(o_vec_s *ctx, size_t idx);

o_public_o o_T_o *o_vec_o_remove(o_vec_s *ctx, size_t idx);

o_public_o o_T_o *o_vec_o_push_fore(o_vec_s *ctx);

o_public_o o_T_o *o_vec_o_push_back(o_vec_s *ctx);

o_public_o o_T_o *o_vec_o_pull_fore(o_vec_s *ctx);

o_public_o o_T_o *o_vec_o_pull_back(o_vec_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

o_inline_o o_T_o *o_vec_o_push(o_vec_s *ctx)
{
    return o_vec_o_push_back(ctx);
}

o_inline_o o_T_o *o_vec_o_pull(o_vec_s *ctx)
{
    return o_vec_o_pull_back(ctx);
}

#endif /* __OO_VEC_H__ */
