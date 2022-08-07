///////////////////////////////////////////////////////////
//
// ADT Set implementation using AVL tree
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTSet.h"


// The struct is basically an AVL tree
struct set {
	SetNode root;				// The root, NULL when the tree is empty
	int size;					// size, so that set_size is O(1)
	CompareFunc compare;		// The ordered is based on this function
	DestroyFunc destroy_value;	// Function that destroy's an element of the set
};

// This struct is the node of an AVL tree
struct set_node {
	SetNode left, right;		// Children
	Pointer value;				// Value of node
	int height;					// The height of the node inside the tree
};


//// Functions used only for AVL and not for normal BST's /////////////////////////////////////

// Returns the max of two integers

static int int_max(int a, int b) {
	return (a > b) ? a : b ;
}

// Returns the height of the node inside the tree

static int node_height(SetNode node) {
	if (!node) return 0;
	return node->height;
}

// Updates the height of a node

static void node_update_height(SetNode node) {
	node->height = 1 + int_max(node_height(node->left), node_height(node->right));
}

// Returns the difference in height of the left and the right sub-trees

static int node_balance(SetNode node) {
	return node_height(node->left) - node_height(node->right);
}

// Rotations are the basic functions used to preserve the AVL property*
// There are 4 differrent cases for rotations
// Each function takes the node that needs to be rototed and returns the new root of the sub-tree

// * AVL property: For each node of the BST the difference between the heights of it's left and right sub-trees must be <= 1

// Single left rotation

static SetNode node_rotate_left(SetNode node) {
	SetNode right_node = node->right;
	SetNode left_subtree = right_node->left;

	right_node->left = node;
	node->right = left_subtree;

	node_update_height(node);
	node_update_height(right_node);
	
	return right_node;
}

// Single right rotation

static SetNode node_rotate_right(SetNode node) {
	SetNode left_node = node->left;
	SetNode left_right = left_node->right;

	left_node->right = node;
	node->left = left_right;

	node_update_height(node);
	node_update_height(left_node);
	
	return left_node;
}

// Double left-right rotation

static SetNode node_rotate_left_right(SetNode node) {
	node->left = node_rotate_left(node->left);
	return node_rotate_right(node);
}

// Double right-left rotation

static SetNode node_rotate_right_left(SetNode node) {
	node->right = node_rotate_right(node->right);
	return node_rotate_left(node);
}

// Fixing the AVL property, when needed

static SetNode node_repair_balance(SetNode node) {
	node_update_height(node);

	int balance = node_balance(node);
	if (balance > 1) {
		// Left sub-tree is unbalanced
		if (node_balance(node->left) >= 0) 
			return node_rotate_right(node);
		else 
			return node_rotate_left_right(node);

	} else if (balance < -1) {
		// Right sub-tree is unbalanced
		if (node_balance(node->right) <= 0)
			return node_rotate_left(node);
		else
			return node_rotate_right_left(node);
	}

	// There was no need for rotation
	return node;
}


///////// Function's that are almost the same with a normal BST implementation ///////////


// Creates and returns a node with the given value (no children)

static SetNode node_create(Pointer value) {
	SetNode node = malloc(sizeof(*node));
	node->left = NULL;
	node->right = NULL;
	node->value = value;
	node->height = 1;
	return node;
}

// Returns the node, of the sub-tree with node as its root, with a value equivalent to the one given, or NULL if it doesn't exist

static SetNode node_find_equal(SetNode node, CompareFunc compare, Pointer value) {
	// Empty sub-tree, value doesn't exist
	if (node == NULL)
		return NULL;
	
	// The position of the node we are looking for depends on the order of value 
	// when compared with the current node (node->value)
	//
	int compare_res = compare(value, node->value);			// Storing so we don't call compare 2 times
	if (compare_res == 0)									// If they are equivalent, we found the node
		return node;
	else if (compare_res < 0)								// value < node->value, node exists in the left sub-tree
		return node_find_equal(node->left, compare, value);
	else													// value > node->value, node exists in the right sub-tree
		return node_find_equal(node->right, compare, value);
}

// Returns the smallest node of the sub-tree with node as it's root

static SetNode node_find_min(SetNode node) {
	return node != NULL && node->left != NULL
		? node_find_min(node->left)				// If a left sub-tree exists, the smallest node exists there
		: node;									// If not, the current node is the smallest
}

// Returns the largest node of the sub-tree with node as it's root

static SetNode node_find_max(SetNode node) {
	return node != NULL && node->right != NULL
		? node_find_max(node->right)			// If a right sub-tree exists, the largest node exists there
		: node;									// If not, the current node is the largest
}

// Returns the previous node (in order), of the node target in the sub-tree with node as it's root
// or NULL if the target is the smallest of the sub-tree. The sub-tree must contain the target node
// so it can't be empty

static SetNode node_find_previous(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// The target node is the root of the sub-tree, so it's previous is the largest of the left sub-tree.
		// ( If there is no left child, then the target node is the smallest of the sub-tree,
		// so node_find_max will returb NULL like we want it to.)
		return node_find_max(node->left);

	} else if (compare(target->value, node->value) < 0) {
		// The target node is in the left sub-tree, so it's previous is also there.
		return node_find_previous(node->left, compare, target);

	} else {
		// The target node is in the right sub-tree, it's previous might also be there, If not
		// it's previous is the node (root) itself
		SetNode res = node_find_previous(node->right, compare, target);
		return res != NULL ? res : node;
	}
}

// Returns the next (in order) of the node target in the sub-tree with node as it's root
// or NULL if the node target is the largest of the sub-tree. The sub-tree must contain the node target
// so it can't be empty.

static SetNode node_find_next(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// The node target is the root of the sub-tree, so it's next is the largest of the right sub-tree.
		// (If there is no right child, then the target node is the largest of the sub-tree
		// so node_find_max will return NULL like we want it to.)
		return node_find_min(node->right);

	} else if (compare(target->value, node->value) > 0) {
		// The node target is in the right sub-tree, so it's next might also be there
		return node_find_next(node->right, compare, target);

	} else {
		// The node target is in the left sub-tree, so it's next might also be there
		// else it's next is the node(root) itself
		SetNode res = node_find_next(node->left, compare, target);
		return res != NULL ? res : node;
	}
}

// If there is a node with a value equivalent o the given one, it updates it's value with the new one, if not it adds
// a new node with the given value. Returns the new root of the sub-tree, and set's the "inserted" variable to true
// if a node has been inserted, or false if it hasn't.

static SetNode node_insert(SetNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value) {
	// If the sub-tree is empty, we create a new node that becomes the root of the sub-tree
	if (node == NULL) {
		*inserted = true;			// New node inserted
		return node_create(value);
	}

	// The position of the insertion depends on the order of the value
	// in comparison to the current node (node->value)
	//
	int compare_res = compare(value, node->value);
	if (compare_res == 0) {
		// We found an equivalent value, so we update it
		*inserted = false;
		*old_value = node->value;
		node->value = value;

	} else if (compare_res < 0) {
		// value < node->value, we continue to the left
		node->left = node_insert(node->left, compare, value, inserted, old_value);

	} else {
		// value > node->value, we continue to the right
		node->right = node_insert(node->right, compare, value, inserted, old_value);
	}

	return node_repair_balance(node);	// AVL
}

// Removes and stores in the "min_node" variable the smallest node of the sub-tree with node as it's root
// Returns the root of the sub-tree.

static SetNode node_remove_min(SetNode node, SetNode* min_node) {
	if (node->left == NULL) {
		// We don't have a left sub-tree, so the smallest is the node it'self
		*min_node = node;
		return node->right;		// The new root is the right child

	} else {
		// We have a left sub-tree, so the smalles value is there. We continue recursively
		// and update node->left with the new root of the sub-tree.
		node->left = node_remove_min(node->left, min_node);

		return node_repair_balance(node);	// AVL
	}
}

// Removes the node with a value equivalent to the new one, if it exists. Returns the new root
// of the sub-tree, and sets the "removed" variable to true if a node has been removed

static SetNode node_remove(SetNode node, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value) {
	if (node == NULL) {
		*removed = false;		// Empty sub-tree, there is no value
		return NULL;
	}

	int compare_res = compare(value, node->value);
	if (compare_res == 0) {
		// An equivalent value was found at the current node, so we remove. How this happens depends on the number of node's children.
		*removed = true;
		*old_value = node->value;

		if (node->left == NULL) {
			// No left sub-tree, so we just remove the node and the new root is the right child.
			SetNode right = node->right;	// Savind it before freeing!
			free(node);
			return right;

		} else if (node->right == NULL) {
			// No right sub-tree, so we just remove and the new root is the left child.
			SetNode left = node->left;		// Savind it before freeing!
			free(node);
			return left;

		} else {
			// Both children exist. We replace the value of node with the smallest of the right sub-tree, which
			// is removed. The function node_remove_min does this exactly.

			SetNode min_right;
			node->right = node_remove_min(node->right, &min_right);

			// Connecting min_right in the position of node
			min_right->left = node->left;
			min_right->right = node->right;

			free(node);

			return node_repair_balance(min_right);	// AVL
		}
	}

	// compare_res != 0, we continue to the right or left sub-tree, the root doesn't change.
	if (compare_res < 0)
		node->left  = node_remove(node->left,  compare, value, removed, old_value);
	else
		node->right = node_remove(node->right, compare, value, removed, old_value);

	return node_repair_balance(node);	// AVL
}

// Destroyes the whole sub-tree with node as it's root

static void node_destroy(SetNode node, DestroyFunc destroy_value) {
	if (node == NULL)
		return;
	
	// First destroy the children, then free the node
	node_destroy(node->left, destroy_value);
	node_destroy(node->right, destroy_value);

	if (destroy_value != NULL)
		destroy_value(node->value);

	free(node);
}


//// Functions of ADT Set. Generally really simple, since they just call the equivalent node_* //////////////////////////////////
//
// Also the same with a basic BST implementation

Set set_create(CompareFunc compare, DestroyFunc destroy_value) {
	assert(compare != NULL);

	// Creating the struct
	Set set = malloc(sizeof(*set));
	set->root = NULL;			// empty tree
	set->size = 0;
	set->compare = compare;
	set->destroy_value = destroy_value;

	return set;
}

int set_size(Set set) {
	return set->size;
} 

void set_insert(Set set, Pointer value) {
	bool inserted;
	Pointer old_value;
	set->root = node_insert(set->root, set->compare, value, &inserted, &old_value);
	
	// The size changes only if a node is inserted. When we update we destroy the old value
	if (inserted)
		set->size++;
	else if (set->destroy_value != NULL)
		set->destroy_value(old_value); 
}

bool set_remove(Set set, Pointer value) {
	bool removed;
	Pointer old_value = NULL;
	set->root = node_remove(set->root, set->compare, value, &removed, &old_value);

	// The size changes only if a node is removed
	if (removed) {
		set->size--;

		if (set->destroy_value != NULL)
			set->destroy_value(old_value);
	}

	return removed;
}

Pointer set_find(Set set, Pointer value) {
	SetNode node = node_find_equal(set->root, set->compare, value);
	return node == NULL ? NULL : node->value;
}

DestroyFunc set_set_destroy_value(Set vec, DestroyFunc destroy_value) {
	DestroyFunc old = vec->destroy_value;
	vec->destroy_value = destroy_value;
	return old;
}

void set_destroy(Set set) {
	node_destroy(set->root, set->destroy_value);
	free(set);
}

SetNode set_first(Set set) {
	return node_find_min(set->root);
}

SetNode set_last(Set set) {
	return node_find_max(set->root);
}

SetNode set_previous(Set set, SetNode node) {
	return node_find_previous(set->root, set->compare, node);
}

SetNode set_next(Set set, SetNode node) {
	return node_find_next(set->root, set->compare, node);
}

Pointer set_node_value(Set set, SetNode node) {
	return node->value;
}

SetNode set_find_node(Set set, Pointer value) {
	return node_find_equal(set->root, set->compare, value);
}