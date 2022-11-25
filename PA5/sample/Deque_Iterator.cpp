#include "Deque_Iterator.h"

void set_node(Iterator& it, Node* new_node) {
    it.node = new_node;
    it.first = new_node -> arr;
    it.last = it.first + CHUNK_SIZE;
}

Iterator next(const Iterator& it) {
    Iterator it_temp = it;
    ++it_temp.current;
    if (it_temp.current == it_temp.last) {
        set_node(it_temp, it_temp.node -> next);
        it_temp.current = it_temp.first;
    }
    return it_temp;
}

Iterator prev(const Iterator& it) {
    Iterator it_temp = it;
    if (it_temp.current == it_temp.first) {
        set_node(it_temp, it_temp.node -> prev);
        it_temp.current = it_temp.last;
    }
    --it_temp.current;
    return it_temp;
}

bool equal(const Iterator& it1, const Iterator& it2) {
    return it1.current == it2.current;
}

int value(const Iterator& it) {
    return *it.current;
}