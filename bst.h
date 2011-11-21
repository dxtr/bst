#ifndef _BST_H
#define _BST_H

#include <stdint.h>
#include <list/list.h>

typedef struct bst_node {
	uint64_t key;
	list_node *list;
	struct bst_node *left;
	struct bst_node *right;
} bst_node;

bst_node* bst_new_node(uint64_t key, void *data);
bst_node* bst_insert(bst_node **root, uint64_t key, int (*compare)(list_node*,void*), void *data);
bst_node* bst_insert_node(bst_node **root, bst_node *node);
bst_node** bst_search(bst_node **root, uint64_t key);
void bst_delete(bst_node **node);
void bst_destroy(bst_node **node);
void bst_free_node(bst_node *node); 

// hashing function
uint64_t hash(const char *str);

#endif

