#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <list/list.h>
#include "bst.h"

/* Creates a new node
 * Arguments: the data the node will contain or NULL for an empty node
 */
bst_node *bst_new_node(uint64_t key, void *data)
{
	struct bst_node *new_node = malloc(sizeof(struct bst_node));
	if (new_node == NULL) return NULL;
	new_node->list = list_create(data);
	if (new_node->list == NULL) {
		free(new_node);
		new_node = NULL;
		return NULL;
	}

	new_node->key = key;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

/* Creates a new node and inserts it in the tree specified by root
 * Arguments: the tree, a comparison function and the data the node will contain
 * If NULL is passed instead of the comparison function it will blindly insert
 * the data (Not taking duplicates into consideration, etc)
 */
bst_node* bst_insert(bst_node** root, uint64_t key, int (*compare)(list_node*,void*), void *data)
{
	if (root == NULL || data == NULL) return NULL; // We return NULL because we won't insert any data anyway

	bst_node** node = bst_search(root, key);
	if (*node == NULL) {
		*node = bst_new_node(key, data);
	} else if (list_find((*node)->list, compare, data) == NULL) {
		list_insert((*node)->list, data);
	}

	return *node;
}

/* Inserts an already created node in a tree
 * Arguments: The tree to insert to and the node to insert
 */
bst_node *bst_insert_node(bst_node** root, bst_node* node)
{
	if (root == NULL || *root == NULL || node == NULL) return NULL;

	bst_node** s_node = bst_search(root, node->key);
	if (*s_node == NULL) {
		*s_node = node;
		return *s_node;
	} /*else if (list_merge((*s_node)->list, compare, node->list) > 0) {
		return *s_node;
	}*/

	return NULL;
}

/* Searches for the data in the tree by using the comparison function
 * Arguments: The tree, comparison function and the data to search for
 */
bst_node** bst_search(bst_node **root, uint64_t key)
{
	if (root == NULL || *root == NULL) return NULL;
	struct bst_node** node = root;

	while (*node != NULL) {
		if (((*node)->key) < key) node = &((*node)->left);
		else if (((*node)->key) > key) node = &((*node)->right);
		else break;
	}

	return node;
}

/* Delete a node
 * Arguments: The node. Duh...
 */
void bst_delete(bst_node **node)
{
	if (node == NULL || *node == NULL) return;
	bst_node *old_node = *node;
	if ((*node)->left == NULL && (*node)->right != NULL) {
		*node = (*node)->right;
		bst_free_node(old_node);
		old_node = NULL;
	} else if ((*node)->right == NULL && (*node)->left != NULL) {
		*node = (*node)->left;
		bst_free_node(old_node);
		old_node = NULL;
	} else if ((*node)->right == NULL && (*node)->left == NULL) {
		bst_free_node(*node);
		*node = NULL;
	} else {
		bst_node **pred = &((*node)->left);
		bst_node *tmp_node = NULL;

		while ((*pred)->right != NULL) pred = &((*pred)->right);
		if ((*pred)->left != NULL) { tmp_node = (*pred)->left; }

		list_node *temp = (*pred)->list;
		(*pred)->list = (*node)->list;
		(*node)->list = temp;
		(*node)->key = (*pred)->key;
		bst_free_node(*pred);
		*pred = NULL;

		if (tmp_node != NULL) { bst_insert_node(node, tmp_node); }
	}
}

/* DELETE ALL THE THINGS!
 * Arguments: Quite obvious. The tree to destroy
 */
void bst_destroy(bst_node **node)
{
	if (node == NULL || *node == NULL) return;
	while (*node) {
		bst_delete(node);
	}
}

/* Free a node.
 * Arguments: ...
 */
void bst_free_node(bst_node *node)
{
	if (node == NULL) return; 
	list_destroy(&(node->list));
	memset(node, 0, sizeof(bst_node));
	free(node);
	node = NULL;
}

/* Function for creating a hash key from a string.
 * Using djb found at http://www.cse.yorku.ca/~oz/hash.html
 * Simply returns the hash
 */ 
uint64_t hash(const char *str)
{
	if (str == NULL) return 0;
	uint64_t h = 5381;
	for(; *str; str++) h = 33*h+(*str);
	return h;
}

