#ifndef DFS_H
#define DFS_H
#include "graph.h"

#define MIN_STACK_SIZE 8
typedef struct {
  vertex **stack;
  int top;
  int capacity;
} gstack;

/* api */

gstack * gstack_init(void);
int g_push (gstack *s, vertex * node);
vertex * g_pop (gstack *s);
int gstack_drop (gstack *s);

#endif
