template <typename K, typename V>
Skiplist<K, V>::Node::Node()
    : nexts(new Node*[1]()), levels(1) {}

template <typename K, typename V>
Skiplist<K, V>::Node::Node(K key, V value, int levels)
    : key(key), value(value), nexts(new Node*[levels]()), levels(levels) {}

template <typename K, typename V>
Skiplist<K, V>::Node::~Node() {
    delete[] this->nexts;
}

template <typename K, typename V>
Skiplist<K, V>::Skiplist(double prob) : prob(prob), head(new Node) {}

template <typename K, typename V>
Skiplist<K, V>::~Skiplist() {
    while (this->head != nullptr) {
        Node* tbd = this->head;
        this->head = tbd->nexts[0];
        delete tbd;
    }
}

template <typename K, typename V>
bool Skiplist<K, V>::get(const K& find_key, V& rtn_value) const {
    int curr_level = this->head->levels - 1;
    Node* prev, *curr;
    prev = curr = this->head;

    while (curr_level >= 0) {
        prev = curr;
        curr = curr->nexts[curr_level];

        if (curr == nullptr || curr->key > find_key) {
            curr_level--;
            curr = prev;
            continue;
        }

        if (curr->key == find_key) {
            rtn_value = curr->value;
            return true;
        }
    }

    return false;
}

template <typename K, typename V>
void Skiplist<K, V>::update(const K& update_key, const V& update_value) {
    int curr_level = this->head->levels - 1;
    Node* prev, *curr;
    prev = curr = this->head;

    Node* prev_nodes[this->head->levels];

    while (curr_level >= 0) {
        prev = curr;
        curr = curr->nexts[curr_level];
        
        if (curr == nullptr || curr->key > update_key) {
            prev_nodes[curr_level] = prev;
            curr_level--;
            curr = prev;
            continue;
        }

        if (curr->key == update_key) {
            curr->value = update_value;
            return;
        }
    }

    //Not found!
    Node* new_node = new Node(update_key, update_value, this->getRandomLevels());
    
    int levels_to_add = this->head->levels > new_node->levels ? new_node->levels : this->head->levels;
    for (int i = 0; i < levels_to_add; i++) {
        new_node->nexts[i] = prev_nodes[i]->nexts[i];
        prev_nodes[i]->nexts[i] = new_node;
    }

    int old_head_level = this->head->levels;

    // Add head levels
    if (new_node->levels > this->head->levels) {
        Node** old_nexts = this->head->nexts;
        this->head->nexts = new Node*[new_node->levels]();
        for (int i = 0; i < this->head->levels; i++)
            this->head->nexts[i] = old_nexts[i];
        
        this->head->levels = new_node->levels;
        delete[] old_nexts;
    }

    for (int i = old_head_level; i < new_node->levels; i++) {
        new_node->nexts[i] = nullptr;
        this->head->nexts[i] = new_node;
    }
}

template <typename K, typename V>
bool Skiplist<K, V>::remove(const K& remove_key) {
    int curr_level = this->head->levels - 1;
    Node* prev, *curr;
    prev = curr = this->head;

    Node* prev_nodes[this->head->levels];

    while (curr_level >= 0) {
        prev = curr;
        curr = curr->nexts[curr_level];
        
        if (curr == nullptr || curr->key >= remove_key) {
            prev_nodes[curr_level] = prev;
            curr_level--;
            curr = prev;
            continue;
        }
    }

    //If end of the list OR not in list
    if (prev_nodes[0]->nexts[0] == nullptr || prev_nodes[0]->nexts[0]->key != remove_key)
        return false;

    Node* tbd = prev_nodes[0]->nexts[0];
    for (int i = 0; i < tbd->levels; i++) {
        prev_nodes[i]->nexts[i] = tbd->nexts[i];
    }
    delete tbd;

    // Remove excess levels
    int remain_levels;
    for (remain_levels = this->head->levels; remain_levels > 1 && this->head->nexts[remain_levels - 1] == nullptr; remain_levels--);
    
    Node** old_nexts = this->head->nexts;
    this->head->nexts = new Node*[remain_levels]();
    for (int i = 0; i < remain_levels; i++)
        this->head->nexts[i] = old_nexts[i];
    
    this->head->levels = remain_levels;
    delete[] old_nexts;

    return true;
}

template <typename K, typename V>
int Skiplist<K, V>::size() const {
    int s = 0;
    for (Node* curr = this->head->nexts[0]; curr != nullptr; curr = curr->nexts[0], s++);
    return s;
}

template <typename K, typename V>
bool Skiplist<K, V>::empty() const {
    return this->size() == 0;
}

template <typename K, typename V>
void Skiplist<K, V>::print() const {
    std::cout << "{";
    for (Node* curr = this->head->nexts[0]; curr != nullptr; curr = curr->nexts[0]) {
        std::cout << curr->key << ": " << curr->value;
        if (curr->nexts[0] != nullptr)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl;
}

template <typename K, typename V>
template <typename T>
Skiplist<K, T> Skiplist<K, V>::map(T (*f)(V value)) const {
    Skiplist<K, T> rtn(this->prob);
    for (Node* curr = this->head->nexts[0]; curr != nullptr; curr = curr->nexts[0])
        rtn.update(curr->key, f(curr->value));
    return rtn;
}

template <typename K, typename V>
Skiplist<K, V> Skiplist<K, V>::filter(bool (*f)(V value)) const {
    Skiplist<K, V> rtn(this->prob);
    for (Node* curr = this->head->nexts[0]; curr != nullptr; curr = curr->nexts[0])
        if (f(curr->value))
            rtn.update(curr->key, curr->value);
    return rtn;
}

template <typename K, typename V>
Skiplist<K, V>& Skiplist<K, V>::operator=(const Skiplist<K, V>& other) {
    if (this != &other) {
        while (this->head != nullptr) {
            Node* tbd = this->head;
            this->head = tbd->nexts[0];
            delete tbd;
        }

        this->head = new Node;

        delete[] this->head->nexts;
        this->head->levels = other.head->levels;
        this->head->nexts = new Node*[this->head->levels]();

        Node* prevs[this->head->levels];
        for (int i = 0; i < this->head->levels; i++)
            prevs[i] = this->head;
        
        for (Node* curr = other.head->nexts[0]; curr != nullptr; curr = curr->nexts[0]) {
            Node* new_node = new Node(curr->key, curr->value, curr->levels);

            for (int i = 0; i < curr->levels; i++)
                prevs[i] = prevs[i]->nexts[i] = new_node;

        }

        this->prob = other.prob;
    }
    
    return *this;
}

template <typename K, typename V>
Skiplist<K, V>::Skiplist(const Skiplist& other) {
    this->head = nullptr;
    *this = other;
}

template <typename K, typename V>
Skiplist<K, V> Skiplist<K, V>::operator+(const Skiplist<K, V>& other) const {
    Skiplist<K, V> rtn(*this);
    for (Node* curr = other.head->nexts[0]; curr != nullptr; curr = curr->nexts[0])
        rtn.update(curr->key, curr->value);
    return rtn;
}
