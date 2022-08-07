///////////////////////////////////////////////////////////
//
// ADTList implementation usin a linked list
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTList.h"


// A list is a pointer to this struct
struct list {
	ListNode dummy;				// We use a dummy node, so that the empty list has one node
	ListNode last;				// Pointer to the last node, or to dummy (if the list is empty)
	int size;					// size, so that list_size function is Ο(1)
	DestroyFunc destroy_value;	// Function that destroys an element of a node
};

struct list_node {
	ListNode next;		// Pointer to the next node
	Pointer value;		// Value of the node
};


List list_create(DestroyFunc destroy_value) {
	// ΠFirst we create the struct
	List list = malloc(sizeof(*list));
	list->size = 0;
	list->destroy_value = destroy_value;

	// The use of a dummy node makes algorithms simpler, se we need to create it
	list->dummy = malloc(sizeof(*list->dummy));
	list->dummy->next = NULL;		// empty list, the dummy hasno next

	// In an empty list, The last node is the dummy
	list->last = list->dummy;

	return list;
}

int list_size(List list) {
	return list->size;
}

void list_insert_next(List list, ListNode node, Pointer value) {
	// If the node is NULL we just insert next to the dummy
	if (node == NULL)
		node = list->dummy;

	// Creating new node
	ListNode new = malloc(sizeof(*new));
	new->value = value;

	// Connecting new between node and node next
	new->next = node->next;
	node->next = new;

	// Updating size and last
	list->size++;
	if (list->last == node)
		list->last = new;
}

void list_remove_next(List list, ListNode node) {
	// If node is null we just delete the first node (the one after dummy)
	if (node == NULL)
		node = list->dummy;

	// The node to be deleted is the one after the one given (which has to exist)
	ListNode removed = node->next;
	assert(removed != NULL);		// LCOV_EXCL_LINE

	if (list->destroy_value != NULL)
		list->destroy_value(removed->value);

	// Connecting the node with the one next to removed
	node->next = removed->next;		// Before freeing

	free(removed);

	// Updating size and last
	list->size--;
	if (list->last == removed)
		list->last = node;
}

Pointer list_find(List list, Pointer value, CompareFunc compare) {
	ListNode node = list_find_node(list, value, compare);
	return node == NULL ? NULL : node->value;
}

DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy_value) {
	DestroyFunc old = list->destroy_value;
	list->destroy_value = destroy_value;
	return old;
}

void list_destroy(List list) {
	// Iterating through the list and freeing all nodes
	// dummy included
	ListNode node = list->dummy;
	while (node != NULL) {
		ListNode next = node->next;		// Keeping node->next before we free

		// We call destroy_value, if it exists (but not for dummy)
		if (node != list->dummy && list->destroy_value != NULL)
			list->destroy_value(node->value);

		free(node);
		node = next;
	}

	// Freeing the whole struct
	free(list);
}


// Iterating the list /////////////////////////////////////////////

ListNode list_first(List list) {
	// First node is the one next to dummy
	return list->dummy->next;
}

ListNode list_last(List list) {
	// If the list is empty last == dummy but we want to return NULL not the dummy
	if (list->last == list->dummy)
		return LIST_EOF;		// empty list
	else
		return list->last;
}

ListNode list_next(List list, ListNode node) {
	assert(node != NULL);   // LCOV_EXCL_LINE
	return node->next;
}

Pointer list_node_value(List list, ListNode node) {
	assert(node != NULL);	// LCOV_EXCL_LINE
	return node->value;
}

ListNode list_find_node(List list, Pointer value, CompareFunc compare) {
	// Iterating the whole list and calling compare until it returns 0
	//
	for (ListNode node = list->dummy->next; node != NULL; node = node->next)
		if (compare(value, node->value) == 0)
			return node;		// Found

	return NULL;	// Doesn't exist
}