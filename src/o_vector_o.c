#include "o_vector_o.h"

#include <string.h>

o_always_o o_T_o *o_vector_o_inc_(o_vector_s *ctx)
{
    return (void)(++ctx->num), ctx->tail++;
}

o_always_o o_T_o *o_vector_o_dec_(o_vector_s *ctx)
{
    return (void)(--ctx->num), --ctx->tail;
}

o_static_o void o_vector_o_drop_(o_vector_s *ctx, size_t bot)
{
    while (ctx->num > bot)
    {
        o_dtor_o(o_vector_o_dec_(ctx));
    }
    ctx->tail = ctx->head + bot;
    ctx->num = bot;
}

#undef align
#define align(x) ((sizeof(o_T_o) * (x) + sizeof(void *) - 1) & ~(sizeof(void *) - 1))

o_static_o int o_vector_o_alloc(o_vector_s *ctx, size_t num)
{
    if (ctx->mem <= num)
    {
        size_t mem = ctx->mem;
        do
        {
            mem += (mem >> 1) + 1;
        } while (mem < num);
        o_T_o *head = (o_T_o *)realloc(ctx->head, align(mem));
        if (head == 0)
        {
            return o_failure_o;
        }
        ctx->mem = mem;
        ctx->head = head;
        ctx->tail = head + ctx->num;
        ctx->last = head + ctx->mem;
    }
    return o_success_o;
}

#undef align

o_vector_s *o_vector_o_new(void)
{
    o_vector_s *ctx = (o_vector_s *)malloc(sizeof(o_vector_s));
    if (ctx)
    {
        o_vector_o_ctor(ctx);
    }
    return ctx;
}

void o_vector_o_die(o_vector_s *ctx)
{
    if (ctx)
    {
        o_vector_o_dtor(ctx);
        free(ctx);
    }
}

void o_vector_o_ctor(o_vector_s *ctx)
{
    ctx->head = 0;
    ctx->tail = 0;
    ctx->last = 0;
    ctx->num = 0;
    ctx->mem = 0;
    ctx->cmp = 0;
}

void o_vector_o_dtor(o_vector_s *ctx)
{
    if (ctx->head)
    {
        o_vector_o_drop_(ctx, 0);
        free(ctx->head);
        ctx->head = 0;
    }
    ctx->tail = 0;
    ctx->last = 0;
    ctx->num = 0;
    ctx->mem = 0;
}

int o_vector_o_copy(o_vector_s *ctx, const o_vector_s *obj)
{
    ctx->head = (o_T_o *)malloc(sizeof(o_T_o) * obj->mem);
    if (ctx->head == 0)
    {
        return o_failure_o;
    }
    ctx->num = obj->num;
    ctx->mem = obj->mem;
    ctx->tail = ctx->head + ctx->num;
    ctx->last = ctx->head + ctx->mem;
#if defined(o_dup_o)
    for (o_T_o *src = obj->head, *dst = ctx->head;
         src != obj->last; ++src, ++dst)
    {
        o_dup_o(dst, src);
    }
#else /* !o_dup_o */
    memcpy(ctx->head, obj->head, sizeof(o_T_o) * obj->num);
#endif /* o_dup_o */
    return o_success_o;
}

o_vector_s *o_vector_o_move(o_vector_s *ctx, o_vector_s *obj)
{
    memcpy(ctx, obj, sizeof(*obj));
    memset(obj, 0, sizeof(*obj));
    return ctx;
}

void o_vector_o_drop(o_vector_s *ctx) { o_vector_o_drop_(ctx, 0); }

void o_vector_o_set(o_vector_s *ctx, int (*cmp)(const o_T_o *, const o_T_o *))
{
    if (ctx->cmp != cmp)
    {
        ctx->cmp = cmp;
    }
}

int o_vector_o_make(o_vector_s *ctx, size_t num)
{
    if (o_vector_o_alloc(ctx, num))
    {
        return o_failure_o;
    }
    o_vector_o_drop_(ctx, num);
    return o_success_o;
}

void o_vector_o_swap(const o_vector_s *ctx, size_t lhs, size_t rhs)
{
    size_t num = ctx->num - 1;
    lhs = lhs < num ? lhs : num;
    rhs = rhs < num ? rhs : num;
    if (lhs != rhs)
    {
        o_T_o buf = ctx->head[lhs];
        ctx->head[lhs] = ctx->head[rhs];
        ctx->head[rhs] = buf;
    }
}

void o_vector_o_sort(const o_vector_s *ctx)
{
    qsort(ctx->head, ctx->num, sizeof(o_T_o), (int (*)(const void *, const void *))ctx->cmp);
}

void o_vector_o_sort_fore(const o_vector_s *ctx)
{
    if (ctx->num > 1)
    {
        o_T_o *ptr = ctx->head;
        o_T_o *end = ctx->tail - 1;
        do
        {
            o_T_o *cur = ptr + 1;
            if (ctx->cmp(ptr, cur) > 0)
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

void o_vector_o_sort_back(const o_vector_s *ctx)
{
    if (ctx->num > 1)
    {
        o_T_o *ptr = ctx->tail - 1;
        do
        {
            o_T_o *cur = ptr - 1;
            if (ctx->cmp(cur, ptr) > 0)
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
        } while (ptr != ctx->head);
    }
}

o_T_o *o_vector_o_search(const o_vector_s *ctx, const o_T_o *obj)
{
    return bsearch(obj, ctx->head, ctx->num, sizeof(o_T_o), (int (*)(const void *, const void *))ctx->cmp);
}

o_T_o *o_vector_o_insert(o_vector_s *ctx, size_t idx)
{
    if (o_vector_o_alloc(ctx, ctx->num))
    {
        return 0;
    }
    if (idx < ctx->num)
    {
        o_T_o *ptr = ctx->tail;
        o_T_o *src = ctx->head + idx + 0;
        o_T_o *dst = ctx->head + idx + 1;
        memmove(dst, src, sizeof(o_T_o) * (size_t)(ptr - src));
        o_vector_o_inc_(ctx);
        o_ctor_o(src);
        return src;
    }
    o_T_o *obj = o_vector_o_inc_(ctx);
    o_ctor_o(obj);
    return obj;
}

o_T_o *o_vector_o_push_fore(o_vector_s *ctx)
{
    return o_vector_o_insert(ctx, 0);
}

o_T_o *o_vector_o_push_back(o_vector_s *ctx)
{
    if (o_vector_o_alloc(ctx, ctx->num))
    {
        return 0;
    }
    o_T_o *obj = o_vector_o_inc_(ctx);
    o_ctor_o(obj);
    return obj;
}

o_T_o *o_vector_o_remove(o_vector_s *ctx, size_t idx)
{
    if (ctx->num && idx < ctx->num - 1)
    {
        if (o_vector_o_alloc(ctx, ctx->num))
        {
            return 0;
        }
        o_T_o *ptr = ctx->tail;
        o_T_o *dst = ctx->head + idx + 0;
        o_T_o *src = ctx->head + idx + 1;
        memcpy(ptr, dst, sizeof(o_T_o));
        memmove(dst, src, sizeof(o_T_o) * (size_t)(ptr - src));
        o_vector_o_dec_(ctx);
        return ptr;
    }
    return ctx->head != ctx->tail ? o_vector_o_inc_(ctx) : 0;
}

o_T_o *o_vector_o_pull_fore(o_vector_s *ctx)
{
    return o_vector_o_remove(ctx, 0);
}

o_T_o *o_vector_o_pull_back(o_vector_s *ctx)
{
    return ctx->head != ctx->tail ? o_vector_o_dec_(ctx) : 0;
}
