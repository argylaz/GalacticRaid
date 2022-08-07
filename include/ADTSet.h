////////////////////////////////////////////////////////////////////////
//
// ABSTRACT DATA TYPE SET
//
// The elements are sorted accordint to the compare function
// and each one is present only one time
// Provides fast search with equicalence or comparison
//
////////////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"


// A set is represented by the type Set

typedef struct set* Set;


// Creates and returns a set in which the elements are sorted using the compare function
// When destroy_value != NULL, it's called to destroy an element whenever it is removed (or all elements when the set is destroyed)

Set set_create(CompareFunc compare, DestroyFunc destroy_value);

// Returns the number of elements inside the set

int set_size(Set set);

// Inserts the value in the set, replacing any other element with equivalent values
//
// CAUTION:
// While a value exists within the set, any change to where it points should not change the order of the set
// Otherwise the behaviour is undefined

void set_insert(Set set, Pointer value);

// Removes the only element equivalent to value, if it exists
// Returns true if value is found and false otherwise

bool set_remove(Set set, Pointer value);

// Returns the only element of the set equivalent to value, or NULL if it doesn't exist

Pointer set_find(Set set, Pointer value);

// Changes the destroy function used when an element is removed
// Returns the old destroy function

DestroyFunc set_set_destroy_value(Set set, DestroyFunc destroy_value);

// Free's all the memory allocated for the set
// Any process on the set afterwards is considered undefined

void set_destroy(Set set);


// Iterating the set ////////////////////////////////////////////////////////////
//
// Iteration using its sorted order

// These constants symbolize the nodes before the first one and after the last one
#define SET_BOF (SetNode)0
#define SET_EOF (SetNode)0

typedef struct set_node* SetNode;

// Return the first / last node of the set, or SET_BOF / SET_EOF if the set is empty

SetNode set_first(Set set);
SetNode set_last(Set set);

// Return the next / previous node to the one given, or SET_EOF / SET_BOF
// if the one given is the last / first one

SetNode set_next(Set set, SetNode node);
SetNode set_previous(Set set, SetNode node);

// Returns the value contained in the given node

Pointer set_node_value(Set set, SetNode node);

// Finds the only node that contains a value equivalent to the one
// Returns the node or SET_EOF if not found

SetNode set_find_node(Set set, Pointer value);
