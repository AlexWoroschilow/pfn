#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "bintree.h"
#include "custom-macro.h"

#define validate_binary_tree(tree)\
  assert_with_message(((tree) != NULL), "Binary tree can not be empty");\
  assert_with_message(((tree->cmp_node_value) != NULL),\
      "Compare function can not be empty");\
  assert_with_message(((tree->combine_node_value) != NULL), \
      "Combine function can not be empty");\
  assert_with_message(((tree->free_node_value) != NULL),\
      "Free function can not be empty");\

#define validate_binary_tree_node(node)\
  assert_with_message(((node) != NULL), "Binary tree node can not be empty");\
  assert_with_message(((node->value) != NULL), \
      "Binary tree node value can not be empty");

typedef struct BinTreeNode BinTreeNode;

struct BinTreeNode {
  void *value;
  BinTreeNode *left, *right;
};

struct GenBinTree {
  BinTreeNode *t_root;
  Cmpfunction cmp_node_value; /* pointer to compare function, returns
   < 0 if value1 < value2,
   0 if value1==value2 * and
   > 0 if value1 > value2 */
  Combinefunction combine_node_value; /* pointer to function that handles
   duplicate values */
  Freefunction free_node_value; /* pointer to Funktion that frees the contents
   of a node pointed to by value */
};

/* Return a pointer to a new <GenBinTree> object. <cmp_node_value> will be used
 to compare and sort the values to be stored in the <GenBinTree>,
 <combine_node_value> is called if <cmp_node_value> indicates a duplicate
 value during a gbt_add()-call, <free_value> will be called for every value in
 the <GenBinTree> if it is deleted. */
GenBinTree *gbt_new(Cmpfunction cmp_node_value,
    Combinefunction combine_node_value, Freefunction free_node_value) {

  GenBinTree * bintree = NULL;

  realloc_or_exit(bintree, sizeof(*bintree),
      "Can not allocate memory for binary tree");

  bintree->t_root = NULL;
  bintree->cmp_node_value = cmp_node_value;
  bintree->combine_node_value = combine_node_value;
  bintree->free_node_value = free_node_value;

  return bintree;
}

BinTreeNode * gbt_create_node(void *new_value) {

  BinTreeNode * node = NULL;

  realloc_or_exit(node, sizeof(*node),
      "Can not allocate memory for binary tree");

  node->left = NULL;
  node->right = NULL;
  node->value = new_value;

  return node;
}

bool gbt_place_node(GenBinTree *bintree, BinTreeNode * node, void *new_value) {
  validate_binary_tree_node(node);

  int compare = (*bintree->cmp_node_value)(node->value, new_value);
  if (compare < 0) {
    if (node->left == NULL) {
      node->left = gbt_create_node(new_value);
      validate_binary_tree_node(node->left);
      return true;
    }
    return gbt_place_node(bintree, node->left, new_value);

  } else if (compare > 0) {
    if (node->right == NULL) {
      node->right = gbt_create_node(new_value);
      validate_binary_tree_node(node->right);
      return true;
    }
    return gbt_place_node(bintree, node->right, new_value);
  }
  (*bintree->combine_node_value)(node->value, new_value);

  return false;
}

/* add a new element to binary tree, uses cmp_node_value. Return 
 true if node corresponding to value was added. Otherwise
 combine_node_value function was called and the gtb_add returns false */
bool gbt_add(GenBinTree *bintree, void *new_value) {
  validate_binary_tree(bintree);

  if (bintree->t_root == NULL) {
    bintree->t_root = gbt_create_node(new_value);
    validate_binary_tree_node(bintree->t_root);
    return true;
  }

  return gbt_place_node(bintree, bintree->t_root, new_value);
}

void gbt_delete_node(GenBinTree *bintree, BinTreeNode * node) {
  validate_binary_tree_node(node);

  if (node->left != NULL) {
    gbt_delete_node(bintree, node->left);
    free(node->left);
  }

  bintree->free_node_value(node->value);

  if (node->right != NULL) {
    gbt_delete_node(bintree, node->right);
    free(node->right);
  }
}

/* free the memory of the whole tree, also of the values inside the nodes,
 * uses free_node_value */
void gbt_delete(GenBinTree *bintree) {
  validate_binary_tree(bintree);
  gbt_delete_node(bintree, bintree->t_root);
  free(bintree->t_root);
  free(bintree);
}

/* enumerate all nodes recursive */
void gbt_enumvalues_node(const GenBinTree *bintree, BinTreeNode * node,
    Applyfunction apply_node_value, void *data) {
  validate_binary_tree_node(node);

  if (node->left != NULL) {
    gbt_enumvalues_node(bintree, node->left, apply_node_value, data);
  }

  (*apply_node_value)(node->value, data);

  if (node->right != NULL) {
    gbt_enumvalues_node(bintree, node->right, apply_node_value, data);
  }
}

/* iterate in sorted order over the values stored in the binary tree.
 Apply the function apply_node_value to the values. data is supplied
 as second argument to this function. */
void gbt_enumvalues(const GenBinTree *bintree, Applyfunction apply_node_value,
    void *data) {
  validate_binary_tree(bintree);
  gbt_enumvalues_node(bintree, bintree->t_root, apply_node_value, data);
}

