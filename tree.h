#ifndef __TREE_INCLUDED__
#define __TREE_INCLUDED__

#include <stdlib.h>
#include <stdbool.h>

struct Node {
	struct Node *left, *right;
	int freq;
	bool is_leaf;
	char ch;
};

typedef struct Node Node;

Node*
create_node() {
	Node *node = (Node*)malloc(sizeof(Node));

	node->left = NULL;
	node->right = NULL;
	node->freq = 0;
	node->is_leaf = false;
	node->ch = 0;

	return node;
}

void
destroy_tree(Node *root) {
	if (root!=NULL) {
		destroy_tree(root->left);
		destroy_tree(root->right);
		free(root);
	}
}
#endif//__TREE_INCLUDED__
