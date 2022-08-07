#include "ADTSet.h"

// Returns the only value of set equivalent to value
// or the one smallest one greater than value (when no equivalent)
// or NULL when there is no such value

Pointer set_find_eq_or_greater(Set set, Pointer value);

// Returns the only value of set equivalent to value
// or the one greatest one smaller than value (when no equivalent)
// or NULL when there is no such value

Pointer set_find_eq_or_smaller(Set set, Pointer value);