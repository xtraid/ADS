# ADS

A C library of algorithms and data structures, built to make writing C a little more comfortable through a few useful abstractions.

This is a long term project. It grows slowly, one data structure at a time, and it is didactic by design: every implementation is written by hand, no AI model is used at the writing level. AI tooling is involved only as an external reviewer, never as the author of the code.

## Build

The library builds as a static archive with make:

```sh
make          # produces libads.a
make clean
```

To use it in your program:

```c
#include "trees.h"
```

```sh
gcc main.c -L. -lads -o main
```

## API

### Binary Search Tree (`trees.h`)

A BST with parent pointers that behaves like an integer map: each node holds a `key`, which orders the tree, and a `value`, the payload associated with the key.

#### Lifecycle

| Function | Description |
|---|---|
| `node * mkbst(int key, int value)` | Allocates the root node and returns it. Returns `NULL` on allocation failure. |
| `int bst_insert(node *parent, int key, int value)` | Inserts a pair in the tree. If the key already exists the value is overwritten. Returns `0` on a new node, `1` when an existing value was overwritten, negative on error. |
| `int rm_leaf(node *leaf)` | Removes a single node, handling all three cases: no children, one child, two children (via successor replacement). |
| `int rm_bst_tree(node *leaf)` | Frees the whole tree the node belongs to, in post order. |

#### Queries

| Function | Description |
|---|---|
| `node * bst_search(node *leaf, int key)` | Searches the whole tree for a key. Returns the node, or `NULL` when the key is not present. |
| `node * bst_min(node *leaf)` | Returns the node with the smallest key in the tree. |
| `node * bst_max(node *leaf)` | Returns the node with the largest key in the tree. |
| `node * find_successor(node *leaf)` | Returns the in order successor of a node, or `NULL` if there is none. |
| `int bst_size(node *leaf)` | Returns the number of nodes in the tree, `0` for an empty tree. |

All query functions accept any node of the tree: they climb to the root on their own before descending.

#### Traversals

| Function | Description |
|---|---|
| `int read_in_order(node *leaf)` | Prints every `key:value` pair in order. |
| `int read_pre_order(node *leaf)` | Prints the pairs in pre order. |
| `int read_post_order(node *leaf)` | Prints the pairs in post order. |

#### Rotations

| Function | Description |
|---|---|
| `int L_rotation(node *leaf)` | Left rotation: the right child of the node is promoted and takes its place. |
| `int R_rotation(node *leaf)` | Right rotation: the left child of the node is promoted and takes its place. |

The names follow the textbook convention, the letter is the direction the rotated node moves. Both rotations preserve the BST ordering and keep every parent pointer consistent, including when the rotation happens at the root.

## Roadmap

The project moves without deadlines, on purpose. What is planned:

* Traversals with an optional output buffer, so the pairs can be collected into arrays instead of printed to stdout.
* `tree_height`.
* A CI/CD section, with the test suite running on every push.
* A calm pass of review, refactoring where needed, and optimization over all the existing code. No rush.
