#pragma once // #include at most once

// Various types used in various modules

// Including stdbool.h for boolean variables
#include <stdbool.h> 

// General use pointer (easier to pronounce than void*)
typedef void* Pointer;


typedef unsigned int uint;

// Pointer to a function that compares to elements of the same type
// < 0  when a < b
//   0  when a και b are considered equivalent
// > 0  when a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to a function that destroyes an element of a certain type
typedef void (*DestroyFunc)(Pointer value);