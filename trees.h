#ifndef TREES_H
#define TREES_H

enum{False, True};

typedef struct node{
  int key;
  int value;
  struct node *left;
  struct node *right;
  struct node *parent;
}node;


node * mkbst(int key, int value);
int bst_insert(node *parent, int key, int value);
int rm_leaf(node *leaf);
node * find_successor(node * leaf);
int rm_bst_tree(node * leaf);

int read_post_order(node * leaf);
int read_pre_order(node * leaf);
int read_in_order(node * leaf);

node *bst_min(node *leaf);
node *bst_max(node *leaf);

node * bst_search(node *leaf, int key);

int R_rotation (node *leaf);
int L_rotation (node *leaf);


#endif
