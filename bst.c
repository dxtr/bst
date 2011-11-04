#include <stdlib.h>
#include "bst.h"

bst_node *bst_new_node(void *data)
{
	struct bst_node *new_node = malloc(sizeof(struct bst_node));
	if (data == NULL || new_node == NULL) goto fail;

	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;

ret:
	return new_node;
fail:
	if (new_node != NULL) {
		free(new_node);
		new_node = NULL;
	}
	goto ret;
}

bst_node* bst_insert(bst_node *root, int (*compare)(void*,void*), void *data)
{
	if (root == NULL || compare == NULL || data == NULL) return NULL;

	bst_node** node = bst_search(&root, compare, data);
	if (*node == NULL) *node = bst_new_node(data);
	return *node;
}

bst_node** bst_search(bst_node **root, int (*compare)(void*,void*), void *data)
{
	if (root == NULL || compare == NULL || data == NULL) return NULL;
	struct bst_node** node = root;

	while (*node != NULL) {
		int result = compare(data, (*node)->data);
		if (result < 0) node = &(*node)->left;
		else if (result > 0) node = &(*node)->right;
		else break;
	}

	return node;
}

void bst_delete(bst_node **node)
{
	if (node == NULL || *node == NULL) return;
	bst_node *old_node = *node;
	if ((*node)->left == NULL && ((*node)->right != NULL)) {
		*node = (*node)->right;
		bst_free_node(old_node);
	} else if ((*node)->right == NULL && ((*node)->left != NULL)) {
		*node = (*node)->left;
		bst_free_node(old_node);
	} else if ((*node)->right == NULL && ((*node)->left == NULL)) {
		bst_free_node(*node);
		*node = NULL;
	} else {
		bst_node **pred = &(*node)->left;
		while ((*pred)->right != NULL) pred = &(*pred)->right;

		void *temp = (*pred)->data;
		(*pred)->data = (*node)->data;
		(*node)->data = temp;
		bst_free_node(*pred);
		*pred = NULL;
	}
}

void bst_free_node(bst_node *node)
{
	free(node);
}
