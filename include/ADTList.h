///////////////////////////////////////////////////////////////////
//
// ABSTRACT DATA TYPE: LIST
//
// Provides sequential access to the elements
// Insert / Remove anywhere inside it
//
///////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"

// These conastants symbolize virtual nodes before the first one and after the last one
#define LIST_BOF (ListNode)0
#define LIST_EOF (ListNode)0


// A list and a list node are represented by the types List and ListNode
typedef struct list* List;
typedef struct list_node* ListNode;



// Creates and returns a new list
// The destroy function is used to destroy a value when its removed but only if destroy_value != NULL

List list_create(DestroyFunc destroy_value);

// Returns the number of elements inside the list

int list_size(List list);

// Inserts a new node after the given one, or at the start if node == LIST_BOF, with the given value inside

void list_insert_next(List list, ListNode node, Pointer value);

// Removes the node after the given one, or the first one if node == LIST_BOF

void list_remove_next(List list, ListNode node);

// Returns the value equivalent to the one given, or NULL if no such value exists

Pointer list_find(List list, Pointer value, CompareFunc compare);

// Changes the destroy function to destroy_value
// Returns the old destroy function

DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy_value);

// Frees all the memory allocated for the list
// Any proccess on the list afterwards is undefined

void list_destroy(List list);


// Iterating the list /////////////////////////////////////////////
//
// Return the first / last node of the list, or LIST_BOF / LIST_EOF when the list is empty

ListNode list_first(List list);
ListNode list_last(List list);

// Returns the node after the given one, or LIST_EOF if the given one is the last

ListNode list_next(List list, ListNode node);

// Returns the value of the given node

Pointer list_node_value(List list, ListNode node);

// Finds the first element that is equivalent to the given value
// Returns it's node or LIST_EOF if it doesn't exist

ListNode list_find_node(List list, Pointer value, CompareFunc compare);