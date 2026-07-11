/*
 * implementazione strutture di dati ad albero
 */

/*
 * errori policy: 
 * [] -1: errore di allocazione
 * */

#include <stdlib.h>
#include <stdio.h>
#include "trees.h"
//#include <stddef.h
/*
basic BST implemented with strct
*/
/* alloc first node and return it, might return null if allocations problems*/
node * mkbst(int val){
  node *root = malloc(sizeof(node));
  if (!root){
    perror("error: malloc failed in the BST root allocation");
    return NULL;
  }
  root->value = val;
  root->left = NULL;
  root->right = NULL;
  root->parent = NULL;
  return root;
}


/*operationo of insertion in a bst 
 * if insert a already saved integer than return 0 and does nothing
 * */
int bst_insert(node *parent, int value){
  if (parent == NULL){
    fprintf(stderr, "error: node passed is null\n");
    return -2;
  }
  if (parent->value == value)
    return 0;
  if (parent->left == NULL && parent->value > value ){
    node *leaf = malloc(sizeof(node));
    if (!leaf){
      perror ("error: malloc failed to allocate BTS leaf allocation in insertion");
      return -1;
    }
    leaf->value = value;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = parent;
    parent->left = leaf;
    return 0;
  }
  if (parent->right == NULL && parent->value < value  ){
    node *leaf = malloc(sizeof(node));
    if (!leaf){
      perror ("error: malloc failed to allocate BTS leaf allocation in insertion");
      return -1;
    }
    leaf->value = value;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = parent;
    parent->right = leaf;
    return 0;
  }
  if(parent->value > value ){
    return bst_insert(parent->left, value);
  }
  else{
    return bst_insert(parent->right, value);
  }
}

int rm_leaf(node * leaf){
  if (leaf == NULL)
    return 0;
  if (leaf->parent == NULL){
    free(leaf);
    return 0;
  }
  if (leaf->left == NULL && leaf->right == NULL){
    if(leaf->parent->value > leaf->value){
      leaf->parent->left = NULL; 
      free(leaf);
      return 0;
    }
    else{
      leaf->parent->right = NULL;
      free(leaf);
      return 0;
    }
  }
  if (leaf->left == NULL){
    if(leaf->parent->value > leaf->value){
      leaf->parent->left = leaf->right;
      leaf->right->parent = leaf->parent;
      free(leaf);
      return 0;
    }
    else{
      leaf->parent->right = leaf->right;
      leaf->right->parent = leaf->parent;
      free(leaf);
      return 0;
    }
  }
  if (leaf->right == NULL){
    if(leaf->parent->value > leaf->value){
      leaf->parent->left = leaf->left; 
      leaf->left->parent = leaf->parent; 
      free(leaf);
      return 0;
    }
    else{
      leaf->parent->right = leaf->left; 
      leaf->left->parent = leaf->parent; 
      free(leaf);
      return 0;
    }
  }
  node *succ = find_successor(leaf); 
  if(succ == NULL)
    return -1;
  succ->left = leaf->left;
  if (succ !=leaf->right){
    succ->parent->left = succ->right;
    if (succ->right != NULL)
      succ->right->parent = succ->parent;
    succ->right = leaf->right;
    succ->right->parent = succ;
  }
  succ->parent = leaf->parent;
  leaf->left->parent = succ;
  if (leaf->parent->value > leaf->value){
    leaf->parent->left = succ;
    free(leaf);
    return 0;
  }
  else{
    leaf->parent->right = succ;
    free(leaf);
    return 0;
  }

}
/*trova il sucessore overo il minimo figlio destro, 
 * nel caso in cui non ci siano figli destri prende 
 * il primo padre che ha il nodo come sinistro */
node * find_successor(node * leaf){
  if (leaf == NULL)
    return NULL;
  node *succ;
  if (leaf->right != NULL){
    succ = leaf->right;
    while(True){
      if (succ->left != NULL)
        succ = succ->left;
      else
        break;
    }
  return succ;
  }
  else{
    succ = leaf->parent;
    int buf = leaf->value;
    while (True){
      if (succ == NULL)
        return NULL;
      if (succ->value > buf)
        break;
      succ = succ->parent;
    }
    return succ;
  }
}

/* helper privati */
static node *find_root(node *n);
static void post_order_free(node *root);
static void post_order(node *root);
static void pre_order(node *root);
static void in_order(node *root);

/* risale al root dell'albero a cui appartiene n (NULL se n e' NULL) */
static node *find_root(node *n){
  if (n == NULL)
    return NULL;
  while (n->parent != NULL)
    n = n->parent;
  return n;
}

/* libera l'intero albero in post-order (foglie -> root) */
int rm_bst_tree(node * leaf){
  post_order_free(find_root(leaf));
  return 0;
}

static void post_order_free(node *root){
  if (!root)
    return;
  post_order_free(root->left);
  post_order_free(root->right);
  free(root);
}

int read_post_order(node * leaf){
  post_order(find_root(leaf));
  return 0;
}

static void post_order(node *root){
  if (!root)
    return;
  post_order(root->left);
  post_order(root->right);
  printf("%d ",root->value);
}

int read_pre_order(node * leaf){
  pre_order(find_root(leaf));
  return 0;
}

static void pre_order(node *root){
  if (!root)
    return;
  printf("%d ",root->value);
  pre_order(root->left);
  pre_order(root->right);
}

int read_in_order(node * leaf){
  in_order(find_root(leaf));
  return 0;
}

static void in_order(node *root){
  if (!root)
    return;
  in_order(root->left);
  printf("%d ",root->value);
  in_order(root->right);
}
