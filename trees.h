#ifndef trees
#define trees


typedef struct node{
  int value;
  struct node *left;
  struct node *right;
}node;


node * mkbst(int val);
int bst_insert(node *parent, int value);
#endif
