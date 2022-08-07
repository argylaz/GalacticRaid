#include <stdio.h>
#include "set_utils.h"

Pointer set_find_eq_or_greater(Set set, Pointer value){
    Pointer p = set_find(set, value);
    if(p == NULL){
        // Insert value to find greater
        // If there is no greater value set_next returns SET_EOF
        // and p = NULL is returned
        set_insert(set, value);
        SetNode next = set_next(set, set_find_node(set, value));
        if(next != SET_EOF)
            p = set_node_value(set, next);
        else
            p = NULL;
        set_remove(set, value);
    }
    return p;
}

Pointer set_find_eq_or_smaller(Set set, Pointer value){
    Pointer p = set_find(set, value);
    if(p == NULL){
        // Insert value to find smaller
        // If there is no smaller value set_previous returns SET_BOF
        // and p = NULL is returned
        set_insert(set, value);
        SetNode prev = set_previous(set, set_find_node(set, value));
        if(prev != SET_BOF)
            p = set_node_value(set, prev);
        else
            p = NULL;
        set_remove(set, value);
    }
    return p;
}