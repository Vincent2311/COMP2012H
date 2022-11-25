#include "Deque.h"
#include <fstream>

Deque create_deque() {
    Deque new_deque{};
    Node* sentinel = new Node{};
    new_deque.sentinel = sentinel;

    // allocate one chunks
    Node* first = new Node{};
    first -> next = first -> prev = sentinel;
    sentinel -> next = sentinel -> prev = first;
    new_deque.ll_size = 1;

    // start and end both points at first position of first chunk
    // you may have different implementations
    set_node(new_deque.start, first);
    set_node(new_deque.end, first);
    new_deque.start.current = new_deque.end.current = new_deque.start.first;

    return new_deque;
}

void destroy_deque(Deque& deque) {
    // Nodes are dynamically allocated in deque
    Node* curr = deque.sentinel -> next;
    while (curr != deque.sentinel) {
        Node* temp = curr;
        curr = curr -> next;
        delete temp;
    }
    delete deque.sentinel;
    deque.sentinel = nullptr;
    deque.ll_size = 0;
}

Iterator begin(const Deque& deque) {
    return deque.start;
}

Iterator end(const Deque& deque) {
    return deque.end;
}

bool empty(const Deque& deque) {
    return deque.start.current == deque.end.current;
}

int front(const Deque& deque) {
    if (empty(deque)) {
        cout << "Cannot get front: deque is empty" << endl;
        return -1;
    }
    return value(deque.start);
}

int back(const Deque& deque) {
    if (empty(deque)) {
        cout << "Cannot get back: deque is empty" << endl;
        return -1;
    }
    return value(prev(deque.end));
}

int size(const Deque& deque) {
    return (deque.ll_size - 2) * CHUNK_SIZE +
           deque.start.last - deque.start.current +
           deque.end.current - deque.end.first;
}

// helper function
Node* create_new_node(Deque& deque, Node* const prev, Node* const next) {
    Node* new_node = new Node{};
    new_node -> prev = prev;
    new_node -> next = next;
    prev -> next = new_node;
    next -> prev = new_node;
    ++deque.ll_size;

    return new_node;
}

Node* create_new_node_back(Deque& deque) {
    return create_new_node(deque, deque.sentinel -> prev, deque.sentinel);
}

Node* create_new_node_front(Deque& deque) {
    return create_new_node(deque, deque.sentinel, deque.sentinel -> next);
}

void push_back(Deque& deque, int val) {
    if (deque.end.current != deque.end.last - 1) {
        // enough space, put val at deque.end.current
        *deque.end.current = val;
        deque.end = next(deque.end);
    } else {
        // create a new node, put val at deque.end.current,
        Node* new_node = create_new_node_back(deque);
        *deque.end.current = val;
        // and move current to next Node
        set_node(deque.end, new_node);
        deque.end.current = deque.end.first;
    }
}

void push_front(Deque& deque, int val) {
    if (deque.start.current != deque.start.first) {
        // enough space, put val at deque.start.current - 1
        deque.start = prev(deque.start);
        *deque.start.current = val;
    } else {
        // create a new node, move current to prev node,
        Node* new_node = create_new_node_front(deque);
        set_node(deque.start, new_node);
        deque.start.current = deque.start.last - 1;
        // and put val at new deque.start.current,
        *deque.start.current = val;
    }
}

void destroy_node(Deque& deque, Node* node) {
    node -> prev -> next = node -> next;
    node -> next -> prev = node -> prev;
    node -> prev = node -> next = nullptr;
    --deque.ll_size;
    delete node;
}

void pop_back(Deque& deque) {
    if (empty(deque)) {
        cout << "Cannot pop_back: deque is empty" << endl;
        return;
    }
    if (deque.end.current != deque.end.first) {
        // not first item in last chunk, just move curr to prev pos
        --deque.end.current;
    } else {
        // first item in last chunk, destroy chunk and move curr to prev chunk
        destroy_node(deque, deque.sentinel -> prev);
        set_node(deque.end, deque.sentinel -> prev);
        deque.end.current = deque.end.last - 1;
    }
}

void pop_front(Deque& deque) {
    if (empty(deque)) {
        cout << "Cannot pop_front: deque is empty" << endl;
        return;
    }
    if (deque.start.current != deque.start.last - 1) {
        // not last item in the chunk, just move curr to next pos
        ++deque.start.current;
    } else {
        // last item in the chunk, delete chunk and move curr to next chunk
        destroy_node(deque, deque.sentinel -> next);
        set_node(deque.start, deque.sentinel -> next);
        deque.start.current = deque.start.first;
    }
}

void print_deque(const Deque& deque) {
    cout << "[";
    for (Iterator it = begin(deque); !equal(it, end(deque)); it = next(it)) {
        cout << value(it);
        if (!equal(next(it), deque.end)) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

void insert(Deque& deque, const Iterator& pos, int val) {
    if (equal(pos, begin(deque))) push_front(deque, val);
    else if (equal(pos, end(deque))) push_back(deque, val);
    else {
        if (deque.end.current != deque.end.last - 1) {
            // enough space
            deque.end = next(deque.end);
        } else {
            // create a new node
            Node* new_node = create_new_node_back(deque);
            // and move current to next Node
            set_node(deque.end, new_node);
            deque.end.current = deque.end.first;
        }
        // move items to the end
        for (Iterator it = prev(end(deque)); !equal(it, pos); it = prev(it)) {
            *it.current = *(prev(it)).current;
        }
        // put new value here
        *pos.current = val;
    }
}

void erase(Deque& deque, const Iterator& pos) {
    if (equal(pos, begin(deque))) pop_front(deque);
    else if (equal(pos, prev(end(deque)))) pop_back(deque);
    else {
        // move items to the start
        Iterator rend = prev(end(deque));
        for (Iterator it = pos; !equal(it, rend); it = next(it)) {
            *it.current = *(next(it)).current;
        }
        if (deque.end.current != deque.end.first) {
            // not first item in last chunk, just move curr to prev pos
            --deque.end.current;
        } else {
            // first item in last chunk, destroy chunk and move curr to prev chunk
            destroy_node(deque, deque.sentinel -> prev);
            set_node(deque.end, deque.sentinel -> prev);
            deque.end.current = deque.end.last - 1;
        }
    }
}

void store_deque(const Deque &deque, const char *filename) {
    ofstream ofs(filename);

    ofs << size(deque) << endl;     // the size of deque
    for (Iterator it = begin(deque); !equal(it, end(deque)); it = next(it)) {
        ofs << value(it) << " ";
    }

    ofs.close();
}

Deque load_deque(const char *filename) {
    ifstream ifs(filename);

    // read each item and construct a new deque
    Deque deque = create_deque();
    int length, temp;
    ifs >> length;
    for (int i = 0; i < length; ++i) {
        ifs >> temp;
        push_back(deque, temp);
    }

    ifs.close();
    return deque;
}
