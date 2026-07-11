#ifndef TREES_H
#define TREES_H

enum{False, True};

typedef struct node{
  int value;
  struct node *left;
  struct node *right;
  struct node *parent;
}node;


node * mkbst(int val);
int bst_insert(node *parent, int value);
int rm_leaf(node *leaf);
node * find_successor(node * leaf);
int rm_bst_tree(node * leaf);

int read_post_order(node * leaf);
int read_pre_order(node * leaf);
int read_in_order(node * leaf);



#endif
