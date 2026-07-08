#ifndef trees
#define trees

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
#endif
