template <typename K, typename V>
Skiplist<K, V>::Node::Node()
{
    nexts = new Node *[1]{nullptr};
    levels = 1;
}

template <typename K, typename V>
Skiplist<K, V>::Node::Node(K key, V value, int levels)
{
    nexts = new Node *[levels];
    for(int i = 0; i < levels;i++) {
        nexts[i] = nullptr;
    }
    this->levels = levels;
    this->key = key;
    this->value = value;
}

template <typename K, typename V>
Skiplist<K, V>::Node::~Node()
{
    delete[] nexts;
}

template <typename K, typename V>
Skiplist<K, V>::Skiplist(double prob)
{
    this->prob = prob;
    head = new Node;
}

template <typename K, typename V>
Skiplist<K, V>::~Skiplist()
{
    Node *nxt;
    Node *cur = head;
    while (cur)
    {
        nxt = cur->nexts[0];
        delete cur;
        cur = nxt;
    }
}

template <typename K, typename V>
Skiplist<K, V> &Skiplist<K, V>::operator=(const Skiplist<K, V> &other)
{
    if (this != &other)
    {
        Node *nxt;
        Node *cur = head;
        while (cur)
        {
            nxt = cur->nexts[0];
            delete cur;
            cur = nxt;
        }
        prob = other.prob;
        head = new Node;
        head->levels = other.head->levels;
        delete[] head->nexts;
        head->nexts = new Node *[head->levels];
        for(int i = 0; i < head->levels;i++) {
            head->nexts[i] = nullptr;
        }

        Node *cur_other = other.head->nexts[0];
        Node *cur_new = head;
        while (cur_other)
        {
            cur_new->nexts[0] = new Node(cur_other->key, cur_other->value, cur_other->levels);
            cur_other = cur_other->nexts[0];
            cur_new = cur_new->nexts[0];
        }
        for (int i = 1; i < head->levels; i++)
        {
            cur_other = other.head->nexts[i];
            cur_new = head;
            while (cur_other)
            {
                K key = cur_other->key;
                Node *find_Node = head->nexts[i-1];
                while (find_Node->key != key)
                {
                    find_Node = find_Node->nexts[i-1];
                }
                cur_new->nexts[i] = find_Node;
                cur_other = cur_other->nexts[i];
                cur_new = cur_new->nexts[i];
            }
        }
    }
    return *this;
}

template <typename K, typename V>
Skiplist<K, V>::Skiplist(const Skiplist<K, V> &other)
{
    head = nullptr;
    *this = other;
}

template <typename K, typename V>
bool Skiplist<K, V>::get(const K &get_key, V &rtn_value) const
{
    int level = head->levels - 1;
    Node *pre = head;
    Node *cur = head->nexts[level];
    while (level >= 0)
    {
        while (cur && cur->key < get_key)
        {
            pre = cur;
            cur = cur->nexts[level];
        }
        if (cur && cur->key == get_key)
        {
            rtn_value = cur->value;
            return true;
        }
        if(level >0) {
            level--;
            cur = pre->nexts[level];
        }
        else level--;
    }
    return false;
}

template <typename K, typename V>
void Skiplist<K, V>::update(const K& update_key, const V& update_value) {
    int level = head->levels - 1;
    Node *pre = head;
    Node *cur = head->nexts[level];
    while (level >= 0)
    {
        while (cur && cur->key < update_key)
        {
            pre = cur;
            cur = cur->nexts[level];
        }
        if (cur && cur->key == update_key)
        {
            cur->value = update_value;
            return;
        }
        if(level >0) {
            level--;
            cur = pre->nexts[level];
        }
        else level--;
    }

    int new_level = getRandomLevels();
    Node *newNode = new Node(update_key,update_value,new_level);
    if (new_level > head->levels) {
        Node ** new_nxts = new Node * [new_level];
        for(int i = 0; i < new_level;i++) {
            new_nxts[i] = nullptr;
        }
        for(int i = 0; i < head->levels; i++) {
            new_nxts[i] = head->nexts[i];
        }
        delete [] head->nexts;
        head->nexts = new_nxts;
        head->levels = new_level;
    }
    
    level =  new_level - 1;
    while (level >= 0 && !head->nexts[level]) {
        head->nexts[level] = newNode;
        level--;
    }
    if(level < 0) return;

    pre = head;
    cur = head->nexts[level];
    while (level >= 0) {
        while (cur && cur->key < update_key)
        {
            pre = cur;
            cur = cur->nexts[level];
        }
        pre->nexts[level] = newNode;
        newNode->nexts[level] = cur; 
        if(level >0) {
            level--;
            cur = pre->nexts[level];
        }
        else level--;
    }
}

template <typename K, typename V> 
bool Skiplist<K, V>::remove(const K& remove_key) {
    int level = head->levels - 1;
    Node *pre = head;
    Node *cur = head->nexts[level];
    while (level >= 0)
    {
        while (cur && cur->key < remove_key)
        {
            pre = cur;
            cur = cur->nexts[level];
        }
        if (cur && cur->key == remove_key)
        {
            int cur_level = cur->levels - 1;
            while(head->nexts[cur_level] == cur && !cur->nexts[cur_level]) {
                cur_level--;
            }
            if(cur_level!=cur->levels - 1) {
                Node ** new_nxts = new Node * [cur_level + 1];
                for(int i = 0; i <= cur_level; i++) {
                    new_nxts[i] = head->nexts[i];
                }
                delete [] head->nexts;
                head->nexts = new_nxts;
                head->levels = cur_level + 1;
            }
            while(cur_level >= 0) {
                pre->nexts[cur_level] = cur->nexts[cur_level];
                if(cur_level > 0) {
                    cur_level--;
                    cur = pre->nexts[cur_level];
                    while (cur->key != remove_key)
                    {
                        pre = cur;
                        cur = cur->nexts[cur_level];
                    }
                }
                else cur_level--;
            }
            delete cur;
            return true;
        }
        if(level >0) {
            level--;
            cur = pre->nexts[level];
        }
        else level--;
    }
    return false;
}

template <typename K, typename V>
int Skiplist<K, V>::size() const {
    int count = 0;
    Node *cur = head->nexts[0];
    while (cur) {
        count++;
        cur = cur->nexts[0];
    }
    return count;
}

template <typename K, typename V>
bool Skiplist<K, V>::empty() const {
    return size() == 0;
}

template <typename K, typename V>
void Skiplist<K, V>::print() const {
    Node *cur = head->nexts[0];
    std::cout << "{";
    while(cur->nexts[0]) {
        std::cout << cur->key << ": " << cur->value << ", ";
        cur = cur->nexts[0];
    }
    std::cout << cur->key << ": " << cur->value << "}" <<std::endl;
    
}

template <typename K, typename V>
template <typename T>
Skiplist<K, T> Skiplist<K, V>::map(T (*f)(V value)) const {
    Skiplist<K, T> new_Skiplist;
    Node *cur = head->nexts[0];
    while(cur) {
        new_Skiplist.update(cur->key,f(cur->value));
        cur = cur->nexts[0];
    }
    return new_Skiplist;
}

template <typename K, typename V>
Skiplist<K, V> Skiplist<K, V>::filter(bool (*f)(V value)) const {
    Skiplist<K, V> new_Skiplist;
    Node *cur = head->nexts[0];
    while(cur) {
        if(f(cur->value)) {
            new_Skiplist.update(cur->key,cur->value);
        }
        cur = cur->nexts[0];
    }
    return new_Skiplist;
}

template <typename K, typename V>
Skiplist<K, V> Skiplist<K, V>::operator+(const Skiplist& other) const {
    Skiplist<K, V> new_Skiplist;
    Node *cur = head->nexts[0];
    while(cur) {
        new_Skiplist.update(cur->key,cur->value);
        cur = cur->nexts[0];
    }
    cur = other.head->nexts[0];
    while(cur) {
        new_Skiplist.update(cur->key,cur->value);
        cur = cur->nexts[0];
    }
    return new_Skiplist;
}