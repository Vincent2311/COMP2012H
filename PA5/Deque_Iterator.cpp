#include "Deque_Iterator.h"

bool equal(const Iterator& it1, const Iterator& it2) {
    return it1.current == it2.current && it1.node == it2.node;
}

int value(const Iterator& it) {
    return *it.current;
}

void set_node(Iterator& it, Node* new_node) {
    it.first = new_node->arr;
    it.last = new_node->arr + CHUNK_SIZE;
    it.node = new_node;
}

Iterator next(const Iterator& it) {
    Iterator new_it;
    if (it.current + 1 == it.last) {
        set_node(new_it,it.node->next);
        new_it.current = it.node->next->arr;
    }
    else {
        new_it.first = it.first;
        new_it.last = it.last;
        new_it.node = it.node;
        new_it.current = it.current + 1;
    }
    return new_it;
}

Iterator prev(const Iterator& it) {
    Iterator new_it;
    if (it.current  == it.first) {
        set_node(new_it,it.node->prev);
        new_it.current = it.node->prev->arr + (CHUNK_SIZE - 1);
    }
    else {
        new_it.first = it.first;
        new_it.last = it.last;
        new_it.node = it.node;
        new_it.current = it.current - 1;
    }
    return new_it;
}