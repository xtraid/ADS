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

/*
 * Capacity always starts at MIN_STACK_SIZE and changes only
 * by powers of two (x2 on grow, /2 on shrink).
 * Therefore, if capacity > MIN_STACK_SIZE, then capacity / 2
 * is guaranteed to be >= MIN_STACK_SIZE.
 */

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
// fast linked list


typedef struct vlist{
  vertex node;
  struct vlist * next;
}vlist;


static vlist* vlist_init (vertex * v);
static int v_insert (vlist *parent, vertex *v);
static int vl_search (vlist *root, vertex * target);
static vertex vl_pop (vlist *v);
static int vl_drop (vlist *root);

/*who calls it has the duty of saving root address*/
static vlist *vlist_init (vertex * v){
  if (!v)
    return NULL;
  vlist * root = malloc(sizeof(vlist));
  if (!root){
    perror("malloc");
    return NULL;
  }
  root->node = *v;
  root->next = NULL;
  return root;
}

static int v_insert (vlist *parent, vertex *v){
   if (!v || !parent)
    return -1;
  vlist *tmp = parent->next;
  vlist *new_node = malloc(sizeof(vlist));
  if (!new_node){
    perror("malloc");
    return -2;
  }
  new_node->node = *v;
  new_node->next = tmp;
  parent->next = new_node;
  return 0;
}

static int vl_search (vlist *root, vertex * target){
  if(!root || !target)
    return -1;
  int i = 0;
  vlist *v = root;
  while (v){
    if (v->node.id == target->id)
      return i;
    i++;
    v = v->next;
  }
  return -3;
}

static vertex vl_pop (vlist *v){
  if (!v || !v->next)
    return (vertex){0};
  vertex pop = v->next->node;
  vlist *tmp = v->next;
  v->next = v->next->next;
  free(tmp);
  return pop;
}

static int vl_drop (vlist *root){
  if (!root)
    return -1;
  while (root){
    vlist *tmp = root;
    root = root->next;
    free(tmp);
  }
  return 0;
}


/*  dfs functions  */




