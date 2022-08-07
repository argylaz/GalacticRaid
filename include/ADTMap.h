///////////////////////////////////////////////////////////
//
// ABSTRACT DATA TYPE MAP
//
// PROVIDES MAPPING KEY => VALUE
//
///////////////////////////////////////////////////////////

#pragma once // #include at most one time

#include "common_types.h"


// A map is portrayed by the type Set

typedef struct map* Map;


// Createes and returns a Map
// Elements are compared using the compare function passed to this one
// Each destroy function is called whenever an element is removed 
// from the map but only if they are != NULL

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

// Returns the number of elements inside the map

int map_size(Map map);

// Adds a new element (key/value pair) in the map
// If an element with equivalent key exists then its value is replaced with the new one
//
// CAUTION:
// Any change to a key that already exists in the map will result in undefined behaviour

void map_insert(Map map, Pointer key, Pointer value);

// Removes the key/value pair that is equivalent to key (if that exists)
// Returns true in when successful and false when not

bool map_remove(Map map, Pointer key);

// Returns the value that is equivalent to key or NULL when the key doesn't exist
//
// Also returns NULL when the value of the key is NULL so we need to use
// map_find_node if we want to discreminate the cases

Pointer map_find(Map map, Pointer key);

// Changes the destroy function value
// Returns the old destroy function

DestroyFunc map_set_destroy_key  (Map map, DestroyFunc destroy_key  );
DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value);

// Frees the memory allocated for the map
// Any process on the map afterwards is undefined

void map_destroy(Map map);



// Iterating the map through nodes //////////////////////////////////////////////////
//
// With no specific order

// A constant that symbolizes the node after the last one of the map
#define MAP_EOF (MapNode)0

typedef struct map_node* MapNode;

// Returns the first node or MPAO_EOF when map is empty

MapNode map_first(Map map);

// Returns the node next to node or MAP_EOF when node is the last in the map

MapNode map_next(Map map, MapNode node);

// Returns the key of node

Pointer map_node_key(Map map, MapNode node);

// Returns the value of node

Pointer map_node_value(Map map, MapNode node);

// Finds and returns the node with an equivalent key
// or MAP_EOF when the key doesn't exist

MapNode map_find_node(Map map, Pointer key);


//// Other functions based on hashing ////////////////////////////

// Hash function type

typedef uint (*HashFunc)(Pointer);

// Implemented hash functions for frequently used data types

uint hash_string(Pointer value);		// When key is char*
uint hash_int(Pointer value);			// When key is int*
uint hash_pointer(Pointer value);		// When key is a pointer considered different from any other pointer

// Set's the hash function for the map
// Must be called after map_create but before any other function

void map_set_hash_function(Map map, HashFunc hash_func);
