#include "DFS.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* private functioons */
static int gstack_resize(gstack * s);


/*
 * 0 <= top <= capacity
 * stack[0 ... top-1]     = valid elements
 * stack[top ... capacity-1] = free space
 */

gstack * gstack_init(void){
  gstack *s = malloc(sizeof(gstack));
  if (!s){
    perror("malloc");
    return NULL;
  }
  vertex ** stack = malloc(sizeof(vertex *) * MIN_STACK_SIZE);
  if (!stack){
    perror("malloc");
    free(s);
    return NULL;
  }
  s->stack = stack;
  s->top = 0;
  s->capacity = MIN_STACK_SIZE;
  return s;
}

static int gstack_resize(gstack * s){
  if (!s)
    return -1;
  int new_capacity;
  if (s->top == s->capacity)
    new_capacity = s->capacity * 2;
  else if ( s->capacity >= 4 * s->top && s->capacity > MIN_STACK_SIZE)
    new_capacity = s->capacity /2;
  else
    return 0;
  vertex ** new_stack = malloc(sizeof(vertex *) * new_capacity);
  if (!new_stack){
    perror("malloc");
    return -2;
  }
  memcpy(new_stack, s->stack, (size_t)s->top * sizeof(vertex*));
  free(s->stack);
  s->stack = new_stack;
  s->capacity = new_capacity;
  return 0;
}

int g_push (gstack *s, vertex * node){
  if (!s)
    return -1;
  if (!node)
    return -3;
  if (s->top == s->capacity){
    int err = gstack_resize(s);
    if (err != 0)
      return err;
  }

  s->stack[s->top] = node;
  s->top ++;
    return 0;
}

vertex * g_pop (gstack *s){
  if (!s)
    return NULL;
  if (s->top == 0)
    return NULL;
  s->top --;
  vertex * node  = s->stack[s->top];

  if (s->capacity >= 4 * s->top && s->capacity > MIN_STACK_SIZE){
    int err = gstack_resize(s);
    if(err != 0)
      fprintf(stderr, "Warning: resize failed with code: %d, pop succeded", err);
  }
  return node;
}

int gstack_drop (gstack *s){
  if (!s)
    return -1;
  free(s->stack);
  free(s);
  return 0;
}




