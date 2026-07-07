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
//#include <stddef.h>
/*
basic BST implemented with strct
*/

node * mkbst(int val){
  node *root = malloc(sizeof(node));
  if (!root){
    perror("error: malloc failed in the BST root allocation");
    return NULL;
  }
  root->value = val;
  root->left = NULL;
  root->right = NULL;
  return root;
}

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
