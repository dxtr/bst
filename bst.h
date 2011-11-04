#ifndef _BST_H
#define _BST_H

typedef struct bst_node {
	void *data;
	struct bst_node *left;
	struct bst_node *right;
} bst_node;

bst_node* bst_new_node(void *data);
bst_node* bst_insert(bst_node *root, int (*compare)(void*,void*), void *data);
bst_node** bst_search(bst_node **root, int (*compare)(void*,void*), void *data);
void bst_delete(bst_node **node);

void bst_free_node(bst_node *node); 

#endif

