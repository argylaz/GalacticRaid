/////////////////////////////////////////////////////////////////////////////
//
// ADTMap implementation using CUCKOO HASHING
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "ADTMap.h"

#define MAX_LOAD_FACTOR 0.5

// Different states taht a map_node can have
// The deleted state is not needed but i liked it so i kept it
typedef enum {
	EMPTY, OCCUPIED, DELETED
} State;

// Struct of each node that the hash table contains
struct map_node{
	Pointer key;
	Pointer value;
	State state;
};

// struct of the map (contains all info about the hashtable)
struct map {
	MapNode HashTable1;
	MapNode HashTable2;

	int size;
	int capacity; // In this case capacity is the size of each HashTable

	HashFunc hash_function;
	CompareFunc compare;
	DestroyFunc destroy_key;
	DestroyFunc destroy_value;
};


////////////////////// FUNCTIONS USED ONLY IN THIS IMPLEMENTATION ///////////////////////


// Secondary hash function used for the second HashTable
uint hash2(Map map, Pointer value){
	return (map->hash_function(value) * map->hash_function(value)) / map->capacity;
}


/////////////////////////////////// ADTMap.h FUNCTIONS //////////////////////////////////

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Initialising HashTables
	Map map = malloc(sizeof(*map));
	map->capacity = 20;
	map->HashTable1 = malloc(map->capacity * sizeof(struct map_node));
	map->HashTable2 = malloc(map->capacity * sizeof(struct map_node));

	// Initialising MapNode states
	for (int i = 0; i < map->capacity; i++){
		map->HashTable1[i].state = EMPTY;
		map->HashTable2[i].state = EMPTY;
	}

	// Other map info
	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;
	map->hash_function = hash_pointer;  // Default hash_function so the user doesn't always have to set it himself

	return map;
}

// Expanding the HashTable 
static void rehash(Map map){
	// First we save the old values
	int old_capacity = map->capacity;
	MapNode old_HT1 = map->HashTable1;
	MapNode old_HT2 = map->HashTable2;

	// Then we allocate new space
	map->capacity = 2 * old_capacity;
	map->HashTable1 = malloc(map->capacity * sizeof(struct map_node));
	map->HashTable2 = malloc(map->capacity * sizeof(struct map_node));

	// Initialising the state of the nodes
	for(int i = 0; i < map->capacity; i++){
        map->HashTable1[i].state = EMPTY;
		map->HashTable2[i].state = EMPTY;
	}

	map->size = 0;
	// Then we insert all key/value paris from the old to the new one
	for(int i = 0; i < old_capacity; i++){
		if(old_HT1[i].state == OCCUPIED)
			map_insert(map, old_HT1[i].key, old_HT1[i].value);

		if(old_HT2[i].state == OCCUPIED)
			map_insert(map, old_HT2[i].key, old_HT2[i].value);
	}

	free(old_HT1);
	free(old_HT2);
}

// Returns the current number of entries in the map
int map_size(Map map) {
	return map->size;
}

// Inserts a key/value pair in the map. If the key already exists
// it just update's its value with the new one

void map_insert(Map map, Pointer key, Pointer value) {
	// This variable will hold the node that is about to be changed
	MapNode node = &map->HashTable1[map->hash_function(key) % map->capacity];

	// Looping the inserting process until a non-occupied node is found
	// or map->size repetitions have been completed so we rehash
	int count = 0;
	while(node->state == OCCUPIED){
		// First we check if the key is the same
		if(map->compare(key, node->key) == 0){
			node->value = value;
			return;
		}

		// Then we swap the key/value pairs
		Pointer temp_key = node->key;
		Pointer temp_value = node->value;
		node->key = key;
		node->value = value;
		key = temp_key;
		value = temp_value;

		count++;

        // Check whether rehashing is needed
		if(count == map->size){
			rehash(map);
			map_insert(map, key, value);
			return;
		}

		// Then we update the node to the hashing point of the new key/value pair in the new hashtable
		if(count % 2 == 0)
			node = &map->HashTable1[map->hash_function(key) % map->capacity];
		else
			node = &map->HashTable2[hash2(map, key) % map->capacity];
	}

	// After the process we insert the values in the node
	node->key = key;
	node->value = value;
	node->state = OCCUPIED;
	map->size++;

	float load_factor = (float) map->size / map->capacity;
	if(load_factor > MAX_LOAD_FACTOR)
		rehash(map);
	
}

// Deletes from the hash table the node with key equivalent to the one given
bool map_remove(Map map, Pointer key) {
	// First we search for the key inside the map
	MapNode node = map_find_node(map, key);

	// Then we remove it if found or return false otherwise
	if(node != MAP_EOF){
		if(map->destroy_key != NULL)
			map->destroy_key(node->key);

		if(map->destroy_value != NULL)
			map->destroy_value(node->value);

		node->state = DELETED; // Same as EMPTY state
		map->size--;
		return true;
	} else 
		return false;
}

// Searching through the map and returning the correspondent value

Pointer map_find(Map map, Pointer key) {
	// First we search for the key inside the map
	MapNode node = map_find_node(map, key);

	// Then we return its value if found or NULL otherwise
	if(node != MAP_EOF)
		return node->value;
	else
		return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = map->destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

// Freeing the memory allocated for the map
void map_destroy(Map map) {
	// First we destroy the key/value pairs on each array
	for(int i = 0; i < map->capacity; i++){
		if(map->HashTable1[i].state == OCCUPIED){
			if(map->destroy_key != NULL)
				map->destroy_key(map->HashTable1[i].key);

			if(map->destroy_value != NULL)
				map->destroy_value(map->HashTable1[i].value);
		}

		if(map->HashTable2[i].state == OCCUPIED){
			if(map->destroy_key != NULL)
				map->destroy_key(map->HashTable2[i].key);

			if(map->destroy_value != NULL)
				map->destroy_value(map->HashTable2[i].value);
		}
	}

	// Then we free the rest
	free(map->HashTable1);
	free(map->HashTable2);
	free(map);
}

/////////////////////// Iterating the map through nodes ///////////////////////////

MapNode map_first(Map map) {
	// Search in the first HashTable for an occupied node
	for(int i = 0; i < map->capacity; i++){
		if(map->HashTable1[i].state == OCCUPIED)
			return &map->HashTable1[i];
	}

	// If there was no occupied node in the first one
	// Search through the second
	for(int i = 0; i < map->capacity; i++){
		if(map->HashTable2[i].state == OCCUPIED)
			return &map->HashTable2[i];
	}

	// In this case there are no occupied nodes in the map (empty map)
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	// First we search for the node
	MapNode Mnode = MAP_EOF;
	int HT = 0; // Will indicate in which hashtable the node is located

	int HashingPos = map->hash_function(node->key) % map->capacity;
	if(map->compare(node->key, map->HashTable1[HashingPos].key) == 0){
		Mnode = &map->HashTable1[HashingPos];
		HT = 1;
	} else {
		HashingPos = hash2(map, node->key) % map->capacity;
		if(map->compare(node->key, map->HashTable2[HashingPos].key) == 0){
			Mnode = &map->HashTable2[HashingPos];
			HT = 2;
		}
	}

	// If the value of node hasn't chnged return
	if(Mnode == MAP_EOF)
		return NULL; 

	MapNode HashTable = (HT == 1) ? map->HashTable1 : map->HashTable2;
	// Search for the next occupied node in the map
	for(int i = HashingPos + 1; i < map->capacity; i++){
		if(HashTable[i].state == OCCUPIED)
			return &HashTable[i];
	}

	// If we searched through the first map and didnt find any, search through the second
	if(HT == 1){
		HashTable = map->HashTable2;
		for(int i = 0; i < map->capacity; i++){
			if(HashTable[i].state == OCCUPIED)
				return &HashTable[i];
		}
	} 
	
	return MAP_EOF; // In this case we didnt find a next in the second HT so we reached the end of the map
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}

MapNode map_find_node(Map map, Pointer key) {
	// Searching in the two possible places
	MapNode pos1 = &map->HashTable1[map->hash_function(key) % map->capacity];
	MapNode pos2 = &map->HashTable2[hash2(map, key) % map->capacity];
	
	if(pos1->state == OCCUPIED && map->compare(key, pos1->key) == 0)
		    return pos1;

	if (pos2->state == OCCUPIED && map->compare(key, pos2->key) == 0)
		    return pos2;

    return MAP_EOF;
}

// Set's the map's hash function 
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {
	// djb2 hash function, simple, fast, and generaly efficient
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. foo << 5 is faster than foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast to sizt_t, that has the same length as a pointer
}