#ifndef __OO_QUE_H__
#define __OO_QUE_H__

#include "oo.h"

#include <stdlib.h>
#include <string.h>

#define o_list_o_foreach_next(it, ctx) \
    for (o_list_s *it = (ctx)->next; it != (ctx); it = it->next)

#define o_list_o_foreach_prev(it, ctx) \
    for (o_list_s *it = (ctx)->prev; it != (ctx); it = it->prev)

#define o_list_o_forsafe_next(it, at, ctx) \
    for (o_list_s *it = (ctx)->next, *at = it->next; it != (ctx); it = at, at = it->next)

#define o_list_o_forsafe_prev(it, at, ctx) \
    for (o_list_s *it = (ctx)->prev, *at = it->prev; it != (ctx); it = at, at = it->prev)

typedef struct o_list_s
{
    struct o_list_s *next, *prev;
} o_list_s;

o_inline_o o_list_s *o_list_o_from(void *obj) { return o_cast_s(o_list_s *, obj); }

o_inline_o void o_list_o_init(o_list_s *ctx) { ctx->prev = ctx->next = ctx; }

o_inline_o void o_list_o_link(o_list_s *head, o_list_s *tail)
{
    head->next = tail;
    tail->prev = head;
}

o_inline_o void o_list_o_add_(o_list_s *ctx, o_list_s *head, o_list_s *tail)
{
    o_list_o_link(head, ctx);
    o_list_o_link(ctx, tail);
}

o_inline_o void o_list_o_swap(o_list_s *lhs, o_list_s *rhs)
{
    o_list_s *head = rhs->prev, *tail = rhs->next;
    o_list_o_add_(rhs, lhs->prev, lhs->next);
    o_list_o_add_(lhs, head, tail);
}

o_inline_o void o_list_o_del_(o_list_s *ctx)
{
    o_list_o_link(ctx->prev, ctx->next);
}

typedef struct o_node_s
{
    o_list_s node[1];
    o_T_o *vptr;
} o_node_s;

o_inline_o o_node_s *o_node_o_from(void *obj) { return o_cast_s(o_node_s *, obj); }

typedef struct o_que_s
{
    o_list_s head[1];
    o_node_s **ptr;
    size_t cur;
    size_t num;
    size_t mem;
} o_que_s;

o_inline_o size_t o_que_o_num(const o_que_s *ctx) { return ctx->num; }

o_inline_o o_T_o *o_que_o_fore(const o_que_s *ctx)
{
    return o_node_o_from(ctx->head->next)->vptr;
}

o_inline_o o_T_o *o_que_o_back(const o_que_s *ctx)
{
    return o_node_o_from(ctx->head->prev)->vptr;
}

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

o_public_o o_que_s *o_que_o_new(void);

o_public_o void o_que_o_die(o_que_s *ctx);

o_public_o void o_que_o_ctor(o_que_s *ctx);

o_public_o void o_que_o_dtor(o_que_s *ctx);

o_public_o int o_que_o_copy(o_que_s *ctx, const o_que_s *obj);

o_public_o o_que_s *o_que_o_move(o_que_s *ctx, o_que_s *obj);

o_public_o o_T_o *o_que_o_at(const o_que_s *ctx, intmax_t idx);

o_public_o void o_que_o_drop(o_que_s *ctx);

o_public_o void o_que_o_swap(const o_que_s *ctx, size_t lhs, size_t rhs);

o_public_o void o_que_o_sort(const o_que_s *ctx, int (*cmp)(const void *, const void *));

o_public_o void o_que_o_sort_fore(const o_que_s *ctx, int (*cmp)(const void *, const void *));

o_public_o void o_que_o_sort_back(const o_que_s *ctx, int (*cmp)(const void *, const void *));

o_public_o o_T_o *o_que_o_insert(o_que_s *ctx, size_t idx);

o_public_o o_T_o *o_que_o_remove(o_que_s *ctx, size_t idx);

o_public_o o_T_o *o_que_o_push_fore(o_que_s *ctx);

o_public_o o_T_o *o_que_o_push_back(o_que_s *ctx);

o_public_o o_T_o *o_que_o_pull_fore(o_que_s *ctx);

o_public_o o_T_o *o_que_o_pull_back(o_que_s *ctx);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */

#define o_que_o_foreach(it, ctx)                                            \
    for (o_T_o *it = o_cast_r(o_T_o *, (ctx)->head->next),                  \
               *it##_ = o_cast_r(o_T_o *, o_list_o_from(it)->next);         \
         it != o_cast_r(o_T_o *, (ctx)->head)                               \
             ? ((void)(it = o_cast_s(o_T_o *, o_node_o_from(it)->vptr)), 1) \
             : (0);                                                         \
         it = it##_, it##_ = o_cast_r(o_T_o *, o_list_o_from(it)->next))

#define o_que_o_foreach_reverse(it, ctx)                                    \
    for (o_T_o *it = o_cast_r(o_T_o *, (ctx)->head->prev),                  \
               *it##_ = o_cast_r(o_T_o *, o_list_o_from(it)->prev);         \
         it != o_cast_r(o_T_o *, (ctx)->head)                               \
             ? ((void)(it = o_cast_s(o_T_o *, o_node_o_from(it)->vptr)), 1) \
             : (0);                                                         \
         it = it##_, it##_ = o_cast_r(o_T_o *, o_list_o_from(it)->prev))

#endif /* __OO_QUE_H__ */
