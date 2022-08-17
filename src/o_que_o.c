#include "o_que_o.h"

#include <string.h>

o_static_o o_node_s *o_que_o_alloc(o_que_s *ctx)
{
    o_node_s *node = 0;
    if (ctx->cur)
    {
        node = ctx->ptr[--ctx->cur];
    }
    else
    {
        node = (o_node_s *)malloc(sizeof(o_node_s));
        if (node == 0)
        {
            return node;
        }
        node->vptr = 0;
    }
    if (node->vptr == 0)
    {
        node->vptr = (o_T_o *)malloc(sizeof(o_T_o));
        if (node->vptr == 0)
        {
            free(node);
            return 0;
        }
        o_ctor_o(node->vptr);
    }
    ++ctx->num;
    return node;
}

o_static_o int o_que_o_free(o_que_s *ctx, o_node_s *obj)
{
    if (obj == 0)
    {
        return o_invalid_o;
    }
    if (ctx->mem <= ctx->cur)
    {
        size_t mem = ctx->mem + (ctx->mem >> 1) + 1;
        o_node_s **ptr = (o_node_s **)realloc(ctx->ptr, sizeof(void *) * mem);
        if (ptr == 0)
        {
            return o_failure_o;
        }
        ctx->ptr = ptr;
        ctx->mem = mem;
    }
    ctx->ptr[ctx->cur++] = obj;
    --ctx->num;
    return o_success_o;
}

o_static_o void o_que_o_drop_(o_que_s *ctx)
{
    while (ctx->head->next != ctx->head)
    {
        o_node_s *node = o_node_o_from(ctx->head->prev);
        if (o_que_o_free(ctx, node))
        {
            break;
        }
        o_list_o_del_(node->node);
        o_list_o_init(node->node);
    }
}

o_que_s *o_que_o_new(void)
{
    o_que_s *ctx = (o_que_s *)malloc(sizeof(o_que_s));
    if (ctx)
    {
        o_que_o_ctor(ctx);
    }
    return ctx;
}

void o_que_o_die(o_que_s *ctx)
{
    if (ctx)
    {
        o_que_o_dtor(ctx);
        free(ctx);
    }
}

void o_que_o_ctor(o_que_s *ctx)
{
    o_list_o_init(ctx->head);
    ctx->ptr = 0;
    ctx->num = 0;
    ctx->cur = 0;
    ctx->mem = 0;
}

void o_que_o_dtor(o_que_s *ctx)
{
    o_que_o_drop_(ctx);
    while (ctx->cur)
    {
        --ctx->cur;
        o_dtor_o(ctx->ptr[ctx->cur]->vptr);
        free(ctx->ptr[ctx->cur]->vptr);
        free(ctx->ptr[ctx->cur]);
    }
    free(ctx->ptr);
    ctx->ptr = 0;
    ctx->mem = 0;
}

o_que_s *o_que_o_move(o_que_s *ctx, o_que_s *obj)
{
    memcpy(ctx, obj, sizeof(*obj));
    memset(obj, 0, sizeof(*obj));
    return ctx;
}

o_T_o *o_que_o_at(const o_que_s *ctx, intmax_t idx)
{
    intmax_t cur = 0;
    o_T_o *ptr = 0;
    if (idx < 0)
    {
        for (o_list_s *it = ctx->head->prev; it != ctx->head; it = it->prev)
        {
            if (--cur == idx)
            {
                ptr = o_node_o_from(it)->vptr;
                break;
            }
        }
    }
    else
    {
        for (o_list_s *it = ctx->head->next; it != ctx->head; it = it->next)
        {
            if (cur++ == idx)
            {
                ptr = o_node_o_from(it)->vptr;
                break;
            }
        }
    }
    return ptr;
}

void o_que_o_drop(o_que_s *ctx)
{
    o_que_o_drop_(ctx);
    for (size_t cur = ctx->cur; cur--; ctx->ptr[cur]->vptr = 0)
    {
        o_dtor_o(ctx->ptr[cur]->vptr);
        free(ctx->ptr[cur]->vptr);
    }
}

void o_que_o_swap(const o_que_s *ctx, size_t lhs, size_t rhs)
{
    size_t cur = 0;
    size_t num = ctx->num - 1;
    lhs = lhs < ctx->num ? lhs : num;
    rhs = rhs < ctx->num ? rhs : num;
    if (lhs != rhs)
    {
        o_node_s *l = 0;
        o_node_s *r = 0;
        for (o_list_s *it = ctx->head->next; it != ctx->head; it = it->next)
        {
            if (cur == lhs)
            {
                l = o_node_o_from(it);
            }
            else if (cur == rhs)
            {
                r = o_node_o_from(it);
            }
            if (l && r)
            {
                break;
            }
            ++cur;
        }
        o_list_o_swap(l->node, r->node);
    }
}

void o_que_o_sort_fore(const o_que_s *ctx, int (*cmp)(const void *, const void *))
{
    if (ctx->num > 1)
    {
        o_list_s *pt = 0;
        o_list_s *it = ctx->head->next;
        for (o_list_s *at = it->next; at != ctx->head; at = at->next)
        {
            if (cmp(o_node_o_from(it)->vptr, o_node_o_from(at)->vptr) > 0)
            {
                pt = at;
            }
            else
            {
                break;
            }
        }
        if (pt)
        {
            o_list_o_del_(it);
            o_list_o_add_(it, pt, pt->next);
        }
    }
}

void o_que_o_sort_back(const o_que_s *ctx, int (*cmp)(const void *, const void *))
{
    if (ctx->num > 1)
    {
        o_list_s *pt = 0;
        o_list_s *it = ctx->head->prev;
        for (o_list_s *at = it->prev; at != ctx->head; at = at->prev)
        {
            if (cmp(o_node_o_from(at)->vptr, o_node_o_from(it)->vptr) > 0)
            {
                pt = at;
            }
            else
            {
                break;
            }
        }
        if (pt)
        {
            o_list_o_del_(it);
            o_list_o_add_(it, pt->prev, pt);
        }
    }
}

o_T_o *o_que_o_push_fore(o_que_s *ctx)
{
    o_node_s *node = o_que_o_alloc(ctx);
    if (node == 0)
    {
        return 0;
    }
    o_list_o_add_(node->node, ctx->head, ctx->head->next);
    return node->vptr;
}

o_T_o *o_que_o_push_back(o_que_s *ctx)
{
    o_node_s *node = o_que_o_alloc(ctx);
    if (node == 0)
    {
        return 0;
    }
    o_list_o_add_(node->node, ctx->head->prev, ctx->head);
    return node->vptr;
}

o_T_o *o_que_o_pull_fore(o_que_s *ctx)
{
    o_T_o *ptr = 0;
    if (ctx->head->next != ctx->head)
    {
        o_node_s *node = o_node_o_from(ctx->head->next);
        if (o_que_o_free(ctx, node))
        {
            return ptr;
        }
        o_list_o_del_(node->node);
        o_list_o_init(node->node);
        ptr = node->vptr;
    }
    return ptr;
}

o_T_o *o_que_o_pull_back(o_que_s *ctx)
{
    o_T_o *ptr = 0;
    if (ctx->head->next != ctx->head)
    {
        o_node_s *node = o_node_o_from(ctx->head->prev);
        if (o_que_o_free(ctx, node))
        {
            return ptr;
        }
        o_list_o_del_(node->node);
        o_list_o_init(node->node);
        ptr = node->vptr;
    }
    return ptr;
}

o_T_o *o_que_o_insert(o_que_s *ctx, size_t idx)
{
    if (idx < ctx->num)
    {
        size_t cur = 0;
        o_node_s *node = o_que_o_alloc(ctx);
        if (node == 0)
        {
            return 0;
        }
        for (o_list_s *it = ctx->head->next; it != ctx->head; it = it->next)
        {
            if (cur++ == idx)
            {
                o_list_o_add_(node->node, it->prev, it);
                break;
            }
        }
        return node->vptr;
    }
    return o_que_o_push_back(ctx);
}

o_T_o *o_que_o_remove(o_que_s *ctx, size_t idx)
{
    if (idx < ctx->num)
    {
        size_t cur = 0;
        o_node_s *node = 0;
        for (o_list_s *it = ctx->head->next; it != ctx->head; it = it->next)
        {
            if (cur++ == idx)
            {
                node = o_node_o_from(it);
                break;
            }
        }
        if (o_que_o_free(ctx, node))
        {
            return 0;
        }
        o_list_o_del_(node->node);
        o_list_o_init(node->node);
        return node->vptr;
    }
    return o_que_o_pull_back(ctx);
}
