#include "o_vec_o.h"

#include <string.h>

o_always_o o_T_o *o_vec_o_inc_(o_vec_s *ctx) { return ctx->ptr + ctx->num++; }
o_always_o o_T_o *o_vec_o_dec_(o_vec_s *ctx) { return ctx->ptr + --ctx->num; }

o_static_o void o_vec_o_drop_(o_vec_s *ctx, size_t bot)
{
    while (ctx->num > bot)
    {
        o_dtor_o(o_vec_o_dec_(ctx));
    }
    ctx->num = bot;
}

#undef align
#define align(x) ((sizeof(o_T_o) * (x) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))

o_static_o int o_vec_o_alloc(o_vec_s *ctx, size_t num)
{
    if (ctx->mem <= num)
    {
        size_t mem = ctx->mem;
        do
        {
            mem += (mem >> 1) + 1;
        } while (mem < num);
        o_T_o *ptr = (o_T_o *)realloc(ctx->ptr, align(mem));
        if (ptr == 0)
        {
            return o_failure_o;
        }
        ctx->ptr = ptr;
        ctx->mem = mem;
    }
    return o_success_o;
}

#undef align

o_vec_s *o_vec_o_new(void)
{
    o_vec_s *ctx = (o_vec_s *)malloc(sizeof(o_vec_s));
    if (ctx)
    {
        o_vec_o_ctor(ctx);
    }
    return ctx;
}

void o_vec_o_die(o_vec_s *ctx)
{
    if (ctx)
    {
        o_vec_o_dtor(ctx);
        free(ctx);
    }
}

void o_vec_o_ctor(o_vec_s *ctx)
{
    ctx->ptr = 0;
    ctx->num = 0;
    ctx->mem = 0;
}

void o_vec_o_dtor(o_vec_s *ctx)
{
    if (ctx->ptr)
    {
        o_vec_o_drop_(ctx, 0);
        free(ctx->ptr);
        ctx->ptr = 0;
    }
    ctx->mem = 0;
}

int o_vec_o_copy(o_vec_s *ctx, const o_vec_s *obj)
{
    ctx->ptr = (o_T_o *)malloc(sizeof(o_T_o) * obj->mem);
    if (ctx->ptr == 0)
    {
        return o_failure_o;
    }
    ctx->num = obj->num;
    ctx->mem = obj->mem;
#if defined(o_dup_o)
    for (o_T_o *src = obj->ptr, *dst = ctx->ptr,
               *end = src + obj->num;
         src != end; ++src, ++dst)
    {
        o_dup_o(dst, src);
    }
#else /* !o_dup_o */
    memcpy(ctx->ptr, obj->ptr, sizeof(o_T_o) * obj->num);
#endif /* o_dup_o */
    return o_success_o;
}

o_vec_s *o_vec_o_move(o_vec_s *ctx, o_vec_s *obj)
{
    memcpy(ctx, obj, sizeof(*obj));
    memset(obj, 0, sizeof(*obj));
    return ctx;
}

int o_vec_o_make(o_vec_s *ctx, size_t num)
{
    if (o_vec_o_alloc(ctx, num))
    {
        return o_failure_o;
    }
    o_vec_o_drop_(ctx, num);
    return o_success_o;
}

void o_vec_o_drop(o_vec_s *ctx) { o_vec_o_drop_(ctx, 0); }

void o_vec_o_swap(const o_vec_s *ctx, size_t lhs, size_t rhs)
{
    size_t num = ctx->num - 1;
    lhs = lhs < ctx->num ? lhs : num;
    rhs = rhs < ctx->num ? rhs : num;
    if (lhs != rhs)
    {
        o_T_o buf = ctx->ptr[lhs];
        ctx->ptr[lhs] = ctx->ptr[rhs];
        ctx->ptr[rhs] = buf;
    }
}

void o_vec_o_sort(const o_vec_s *ctx, int (*cmp)(const void *, const void *))
{
    qsort(ctx->ptr, ctx->num, sizeof(o_T_o), cmp);
}

void o_vec_o_sort_fore(const o_vec_s *ctx, int (*cmp)(const void *, const void *))
{
    if (ctx->num > 1)
    {
        o_T_o *ptr = ctx->ptr;
        o_T_o *end = ctx->ptr + ctx->num - 1;
        do
        {
            o_T_o *cur = ptr + 1;
            if (cmp(ptr, cur) > 0)
            {
                o_T_o buf = *cur;
                *cur = *ptr;
                *ptr = buf;
            }
            else
            {
                break;
            }
            ptr = cur;
        } while (ptr != end);
    }
}

void o_vec_o_sort_back(const o_vec_s *ctx, int (*cmp)(const void *, const void *))
{
    if (ctx->num > 1)
    {
        o_T_o *ptr = ctx->ptr + ctx->num - 1;
        do
        {
            o_T_o *cur = ptr - 1;
            if (cmp(cur, ptr) > 0)
            {
                o_T_o buf = *cur;
                *cur = *ptr;
                *ptr = buf;
            }
            else
            {
                break;
            }
            ptr = cur;
        } while (ptr != ctx->ptr);
    }
}

o_T_o *o_vec_o_search(const o_vec_s *ctx, const o_T_o *obj, int (*cmp)(const void *, const void *))
{
    return bsearch(obj, ctx->ptr, ctx->num, sizeof(o_T_o), cmp);
}

o_T_o *o_vec_o_insert(o_vec_s *ctx, size_t idx)
{
    if (o_vec_o_alloc(ctx, ctx->num))
    {
        return 0;
    }
    if (idx < ctx->num)
    {
        o_T_o *src = ctx->ptr + idx + 0;
        o_T_o *dst = ctx->ptr + idx + 1;
        o_T_o *ptr = ctx->ptr + ctx->num;
        memmove(dst, src, sizeof(o_T_o) * (size_t)(ptr - src));
        o_vec_o_inc_(ctx);
        o_ctor_o(src);
        return src;
    }
    o_T_o *obj = o_vec_o_inc_(ctx);
    o_ctor_o(obj);
    return obj;
}

o_T_o *o_vec_o_push_fore(o_vec_s *ctx)
{
    return o_vec_o_insert(ctx, 0);
}

o_T_o *o_vec_o_push_back(o_vec_s *ctx)
{
    if (o_vec_o_alloc(ctx, ctx->num))
    {
        return 0;
    }
    o_T_o *obj = o_vec_o_inc_(ctx);
    o_ctor_o(obj);
    return obj;
}

o_T_o *o_vec_o_remove(o_vec_s *ctx, size_t idx)
{
    if (ctx->num && idx < ctx->num - 1)
    {
        if (o_vec_o_alloc(ctx, ctx->num))
        {
            return 0;
        }
        o_T_o *dst = ctx->ptr + idx + 0;
        o_T_o *src = ctx->ptr + idx + 1;
        o_T_o *ptr = ctx->ptr + ctx->num;
        memcpy(ptr, dst, sizeof(o_T_o));
        memmove(dst, src, sizeof(o_T_o) * (size_t)(ptr - src));
        o_vec_o_dec_(ctx);
        return ptr;
    }
    return ctx->num ? o_vec_o_dec_(ctx) : 0;
}

o_T_o *o_vec_o_pull_fore(o_vec_s *ctx)
{
    return o_vec_o_remove(ctx, 0);
}

o_T_o *o_vec_o_pull_back(o_vec_s *ctx)
{
    return ctx->num ? o_vec_o_dec_(ctx) : 0;
}
