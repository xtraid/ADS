/*
 * implementazione strutture di dati ad albero
 */

/*
 * errori policy:
 * WIP
 *  */

#include <stdlib.h>
#include <stdio.h>
#include "trees.h"
//#include <stddef.h


/*
 * BST implemented with struct:
 *
 * typedef struct node{
 * int key;
 * int value;
 * struct node *left;
 * struct node *right;
 * struct node *parent;
 * }node;
 *
 * Has the propriety of mantaining all minor keys on left and all magior keys on right
 * for each node; value is the payload associated to the key
 *
 * n is the number of nodes that the tree has
 * H is the height of the tree and is calculated as the furthest branch from root
 *  -> Log n <= H <= n
 *
 * Costs:
 * - research: O(H)
 * - insert: O(H) i find the leaf and than insert in O(1) ~ O(H)
 * - find min or max: O(H) cos in worst case the tree is ordered so ive to go through the whole
 * - find the successor: O(H)
 * - remoove a node: 3 cases
 *   1) no child -> O(1)
 *   2) single child -> O(1)
 *   3) has 2 child -> we need to find the successor O(H) and do a replacment O(1) ~ O(H)
 * - visits (O(n)):
 *   1) pre order  ~ node -> left -> right
 *   2) in order   ~ left -> node -> right
 *   3) post order ~ left -> right -> node
 * rotation XP wip
*/

/* funzioni helper*/
static node *find_root(node *n);
static void post_order_free(node *root);
static void post_order(node *root);
static void pre_order(node *root);
static void in_order(node *root);
static node *bst_recursive_search (node *leaf, int key);

/* alloc first node and return it, might return null if allocations problems*/
node * mkbst(int key, int value){
  node *root = malloc(sizeof(node));
  if (!root){
    perror("error: malloc failed in the BST root allocation");
    return NULL;
  }
  root->key = key;
  root->value = value;
  root->left = NULL;
  root->right = NULL;
  root->parent = NULL;
  return root;
}


/*operationo of insertion in a bst
 * if insert a already saved key than overwrite the value and return 1
 * */
int bst_insert(node *parent, int key, int value){
  if (parent == NULL){
    fprintf(stderr, "error: node passed is null\n");
    return -2;
  }
  if (parent->key == key){
    parent->value = value;
    return 1;
  }
  if (parent->left == NULL && parent->key > key ){
    node *leaf = malloc(sizeof(node));
    if (!leaf){
      perror ("error: malloc failed to allocate BTS leaf allocation in insertion");
      return -1;
    }
    leaf->key = key;
    leaf->value = value;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = parent;
    parent->left = leaf;
    return 0;
  }
  if (parent->right == NULL && parent->key < key  ){
    node *leaf = malloc(sizeof(node));
    if (!leaf){
      perror ("error: malloc failed to allocate BTS leaf allocation in insertion");
      return -1;
    }
    leaf->key = key;
    leaf->value = value;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = parent;
    parent->right = leaf;
    return 0;
  }
  if(parent->key > key ){
    return bst_insert(parent->left, key, value);
  }
  else{
    return bst_insert(parent->right, key, value);
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
    if(leaf->parent->key > leaf->key){
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
    if(leaf->parent->key > leaf->key){
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
    if(leaf->parent->key > leaf->key){
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
  if (leaf->parent->key > leaf->key){
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
    int buf = leaf->key;
    while (True){
      if (succ == NULL)
        return NULL;
      if (succ->key > buf)
        break;
      succ = succ->parent;
    }
    return succ;
  }
}

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
  printf("%d:%d ",root->key,root->value);
}

int read_pre_order(node * leaf){
  pre_order(find_root(leaf));
  return 0;
}

static void pre_order(node *root){
  if (!root)
    return;
  printf("%d:%d ",root->key,root->value);
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
  printf("%d:%d ",root->key,root->value);
  in_order(root->right);
}


// ---------------------------------------------------

/*
 * min of a BST is naturally the last left chilf from root
 */
node *bst_min(node *leaf){
  if (!leaf){
    fprintf(stderr, "errore: nodo invalido");
    return NULL;
  }
  node * root = find_root(leaf);
  while (root->left != NULL){
    root = root->left;
  }
  return root;
}

/*
 *  max of a BST is naturally the last right child from root
 */
node *bst_max(node *leaf){
  if (!leaf){
    fprintf(stderr, "errore: nodo invalido");
    return NULL;
  }
  node * root = find_root(leaf);
  while (root->right != NULL){
    root = root->right;
  }
  return root;
}



//-------------------------------------------------------


node *bst_search (node *leaf, int key){
  if (!leaf){
    fprintf(stderr, "errore: nodo invalido");
    return NULL;
  }
  node *root = find_root(leaf);
  return bst_recursive_search(root, key);
}

static node * bst_recursive_search (node *leaf, int key){
  if(!leaf){
    return NULL;
  }
  if(leaf->key == key){
    return leaf;
  }
  if(leaf->key > key)
    return bst_recursive_search(leaf->left, key);
  else
    return bst_recursive_search(leaf->right, key);
}


//----------------------------------------------------------

/*
 * Rotations
 *
 * shitty code this make me feel so stupd this is 3 hours of work and 2 refactor :(
 */

int R_rotation (node *leaf){
  if (!leaf){
    fprintf(stderr, "errore: nodo invalido");
    return -1;
  }
  if (!leaf->right){
    fprintf(stderr, "error: node dont has a right son");
    return -1;
  }
  node *y = leaf->right;
  if(!leaf->parent){
    y->parent = NULL;
  }
  else{
    y->parent = leaf->parent;
    if (leaf->parent->key > leaf->key){
      leaf->parent->left = y;
    }
    else{
      leaf->parent->right = y;
    }
  }
  leaf->right = y->left;
  if (y->left){
    y->left->parent = leaf;
  }
  y->left = leaf;
  leaf->parent = y;
  return 0;
}

int L_rotation (node *leaf){
  if (!leaf){
    fprintf(stderr, "errore: nodo invalido");
    return -1;
  }
  if (!leaf->left){
    fprintf(stderr, "error: node dont has a left son");
    return -1;
  }
  node *y = leaf->left;
  if(!leaf->parent){
    y->parent = NULL;
  }
  else{
    y->parent = leaf->parent;
    if (leaf->parent->key > leaf->key){
      leaf->parent->left = y;
    }
    else{
      leaf->parent->right = y;
    }
  }
  leaf->left= y->right;
  if (y->right){
    y->right->parent = leaf;
  }
  y->right = leaf;
  leaf->parent = y;
  return 0;
}



