#include <vector>
#include <iostream>

#include "dictionary.h"

using namespace std;

// this function will not be used in any way. For C++, templates are usually defined in header files. 
// However, for ZINC grading, we separate it into different files. So we need to use this 'trick' to
// instantiate the temnplate.
void compiler_trick_useless_function(){
    Dictionary d{""};
    lambda_for_main_cpp ct;
    d.foreach(ct);
}

int Dictionary::character_to_index(const char& ch){
    return ch - 97;
}

char Dictionary::index_to_character(const int& idx){
    return (char) (idx + 97);
}

const string& Dictionary::get_name() const{
    return name;
}

void Dictionary::set_name(const string& name){
    this->name = name;
}

Dictionary::Dictionary(const string& name)
    : name(name) 
{
    root = new Node;
}

Dictionary::~Dictionary(){
    delete root;
}

Dictionary::Dictionary(Dictionary&& d){
    root = d.root;
    d.root = nullptr;
}

Dictionary& Dictionary::operator=(Dictionary&& d){
    delete root;
    root = d.root;
    d.root = nullptr;

    return *this;
}

void helper_print_string(vector<int>& vect){
    vector<int>::iterator it = vect.begin();
    for(; it != vect.end(); it++){
        cout << Dictionary::index_to_character(*it);
    }
}

/**
 * Using a lambda with current_node and idx as argument, returns the 'found' node. 
 * The loop breaks either when the key is exhausted, or when the lambda returns nullptr.
 * The lambda represents the stage of setting the current_node to the child with index idx.
*/
template<typename T> Node* helper_loop_find_key(T&& lambda, const char* key, Node* const& root){
    int index = 0;
    Node* current_node = root;
    while( key[index] != '\0' )
    {
        int idx = Dictionary::character_to_index(key[index]);
        Node* result = lambda(current_node, idx);
        if(result == nullptr){
            return nullptr;
        }
        current_node = result;

        ++index;
    }
    return current_node;
}

/**
 * Does preorder traversal on the tree starting with root. Calls the lambda whenever the
 * node is first encountered. The dual tree is traversed with respect to the result of lambda function.
 * The tree corresponding to root is to be traversed, while the separate tree corresponding to dual_root
 * should have elements created/modified with respect to the original tree.
 * 
 * The lambda takes Node*& current_node, Node*& current_dual_node and vector<int>& current_key as arguments.
 * The lambda function should adjust the current_dual_node for the next step.
 * 
 * When the lambda function is called, the current current_node is passed to lambda, while the dual node that
 * corresponds to the parent of current_node is passed to lambda. The lambda function should update current_dual_node
 * if necessary.
*/
template<typename T> void helper_preorder_traverse_dual_tree(T&& lambda, Node* const& root, Node* const& dual_root){
    vector<int> current_key;
    current_key.push_back(0);

    Node* current_node = root;
    Node* current_dual_node = dual_root;

    while(true)
    {
        int idx = current_key.back();
        Node *child = (*current_node)[ idx ]; // potential child

        // if child exists, call the lambda and report it to next function.
        // if child does not exist, move onto its next sibling, or the closest cousin
        // which exists.
        if(child != nullptr)
        {
            current_node = child;
            lambda(current_node, current_dual_node, current_key); // create a new node if necessary for the dual node.
            current_key.push_back(0); // the first suggestion for next node is always 'a'.
        }
        else
        {
            // try out next child
            int next = current_key.back() + 1;
            while(next >= 26)
            {
                current_key.pop_back();
                current_node = current_node->get_parent();
                current_dual_node = current_dual_node->get_parent();

                if(current_key.size() == 0){
                    // end of loop
                    return;
                }

                next = current_key.back() + 1;
            }
            current_key.pop_back();
            current_key.push_back(next);
        }
    }
}

ostream& operator<<(ostream& o, Node* n){
    o << (*n);
    return o;
}

ostream& operator<<(ostream& o, const Node& n){
    o << " (" << n.meaning.type << ") " << n.meaning.meaning << " ";
    return o;
}

/**
 * TODO - Constructor for deep copy of dictionary.
*/
Dictionary::Dictionary(const Dictionary& d){
    root = new Node;
    helper_preorder_traverse_dual_tree([](Node*& current_node, Node*& current_dual_node, vector<int>& current_key){
        Node *nn = new Node;
        current_dual_node->set_child( current_key.back(), nn);
        nn->get_parent() = current_dual_node;
        nn->meaning = current_node->meaning;

        current_dual_node = nn;
    }, d.root, root);
}

/**
 * TODO - Assignment operator for deep copy of dictionary.
*/
Dictionary& Dictionary::operator=(const Dictionary& d){
    if(this == &d){
        return *this;
    }
    delete root;
    root = new Node;
    helper_preorder_traverse_dual_tree([](Node*& current_node, Node*& current_dual_node, vector<int>& current_key){
        Node *nn = new Node;
        current_dual_node->set_child( current_key.back(), nn);
        nn->get_parent() = current_dual_node;
        nn->meaning = current_node->meaning;

        current_dual_node = nn;
    }, d.root, root);

    return *this;
}

/**
 * TODO - Adds a node with the given key string, which is terminated with '\0'.
 * You should assume the key consists of lowercase letters only.
 * Do not delete the pointer.
*/
Node* Dictionary::add_node(const char* key){
    Node* result = helper_loop_find_key([](Node*& current_node, const int& idx) -> Node*
    {
        Node* child = (*current_node)[idx];
        if(child == nullptr)
        {
            child = new Node;
            current_node->set_child(idx, child);
            child->get_parent() = current_node;
        }
        return child;
    }, key, root);
    return result;
}

/**
 * TODO - Shorthand for add_node.
*/
Node* Dictionary::operator+=(const char* key){
    return add_node(key);
}

/**
 * TODO - Removes all nodes starting with the given key string, which is terminated with '\0'.
 * You should assume the key consists of lowercase letters only.
 * Do not delete the pointer const char* key (of course you should clean up the nodes that are removed).
*/
void Dictionary::remove_node(const char* key){
    int last_idx = -1;
    Node* result = helper_loop_find_key([&last_idx](Node*& current_node, const int& idx) -> Node*
    {
        last_idx = idx;

        Node* child = (*current_node)[idx];
        if(child != nullptr)
            return child;
        else
            return nullptr;
    }, key, root);

    if(result == root)
    {
        for(int i = 0; i < 26; ++i){
            delete (*root)[i];
            root->set_child(i, nullptr);
        }
        return;
    }
    else if (result != nullptr)
    {
        result->get_parent()->set_child(last_idx, nullptr);
        delete result;
    }
}

/**
 * TODO - Shorthand for remove_node.
*/
void Dictionary::operator-=(const char* key){
    remove_node(key);
}

/**
 * TODO - Finds a node with the given key string, which is terminated with '\0'.
 * Returns nullptr if no such node is found.
 * You should assume the key consists of lowercase letters only.
 * Do not delete the pointer.
*/
Node* Dictionary::find_node(const char* key) const{
    return helper_loop_find_key([](Node*& current_node, const int& idx) -> Node*
    {
        return (*current_node)[idx];
    }, key, root);
}

/**
 * TODO - A function to do pre-order traversal on the tree. The function accepts a lambda function as argument,
 * and then the lambda function would be called for every node in the tree (except the root node). 
 * The lambda function accepts two arguments, (Node* current_node, vector<int>& current_key).
 * For each node accessed via pre-order traversal (except root node), call the lambda function.
 * 
 * Of course current_node should be the pointer to the node accessed. current_key should contain
 * a list of integers which corresponds to the indices required to travel to current_node from
 * the root node. For more explanation see the webpage.
 * 
 * The lambda is not supposed to change the dictionary.
*/
template<typename T> void Dictionary::foreach(T&& lambda) const{
    vector<int> current_key;
    current_key.push_back(0);

    Node* current_node = root;

    while(true)
    {
        Node *child = (*current_node)[ current_key.back() ]; // potential child

        // if child exists, call the lambda and report it to next function.
        // if child does not exist, move onto its next sibling, or the closest cousin
        // which exists.
        if(child != nullptr)
        {
            current_node = child;
            lambda(current_node, current_key);
            current_key.push_back(0); // the first suggestion for next node is always 'a'.
        }
        else
        {
            // try out next child
            int next = current_key.back() + 1;
            while(next >= 26)
            {
                current_key.pop_back();
                current_node = current_node->get_parent();

                if(current_key.size() == 0){
                    // end of loop
                    return;
                }

                next = current_key.back() + 1;
            }
            current_key.pop_back();
            current_key.push_back(next);
        }
    }
}

/**
 * TODO - Prints all the nodes in the dictionary, in depth first alphabetical order.
 * See the webpage description for more details.
*/
void Dictionary::print_all_elements(ostream& o) const{
    int count = 1;
    foreach([&count](Node*& current_node, vector<int>& current_key){
        helper_print_string(current_key);
        cout << current_node << "[" << count << "]\n";
        ++count;
    });
}

/**
 * TODO - Calls print_all_elements
*/
std::ostream& operator<<(std::ostream& o, const Dictionary& dict){
    dict.print_all_elements(o);
    return o;
}

/**
 * TODO - Prints all the words in the dictionary, such that the word type is equal to the given type,
 * in depth first alphabetical order. See the webpage description for more details.
*/
void Dictionary::print_elements_given_type(const char* type) const{
    int count = 1;
    foreach([&type,&count](Node*& current_node, vector<int>& current_key){
        if(!current_node->meaning.meaning.empty()){
            if(type == nullptr || current_node->meaning.type.compare(type) == 0){
                helper_print_string(current_key);
                cout << current_node << "[" << count << "]\n";
                ++count;
            }
        }
    });
}

/**
 * TODO - Merges with another dictionary. Creates a new dictionary,
 * and does not modify the contents of the original dictionaries.
 * For words that exists in both dictionary,
 * use the word definition in the dictionary this.
*/
Dictionary Dictionary::merge(const Dictionary& d2) const{
    return merge(Dictionary{d2});
}

/**
 * TODO - Merges with another dictionary. Moves the contents of d2
 * into a new dictionary. For words that exists in both dictionary,
 * use the word definition in the dictionary this.
*/
Dictionary Dictionary::merge(Dictionary&& d2) const{
    // move the contents of d2 to the new dictionary
    Dictionary merged{""};
    delete merged.root;
    merged.root = d2.root;
    d2.root = nullptr;

    helper_preorder_traverse_dual_tree([](Node*& current_node, Node*& current_dual_node, vector<int>& current_key){
        Node* child_of_dual = (*current_dual_node)[ current_key.back() ];

        // create a child if it does not exist.
        if(child_of_dual == nullptr)
        {
            child_of_dual = new Node;
            current_dual_node->set_child(current_key.back(), child_of_dual);
            child_of_dual->get_parent() = current_dual_node;
        }

        current_dual_node = child_of_dual;

        // if there is a word in (*this), replace the meaning with the meaning in (*this).
        if(!current_node->meaning.meaning.empty())
        {
            current_dual_node->meaning = current_node->meaning;
        }
    }, root, merged.root);

    return merged;
}

/**
 * TODO - Creates a new dictionary, consisting only of the words
 * starting with the given key.
*/
Dictionary Dictionary::filter_starting_word(const char* key) const{
    if(key == nullptr){
        return Dictionary{*this};
    }

    Dictionary filtered{""};
    Node* node = find_node(key);
    if(node == nullptr){
        return filtered;
    }

    Node* created = filtered.add_node(key);
    created->meaning = node->meaning;

    helper_preorder_traverse_dual_tree([](Node*& current_node, Node*& current_dual_node, vector<int>& current_key){
        Node* nn = new Node;
        current_dual_node->set_child(current_key.back(), nn);
        nn->get_parent() = current_dual_node;
        nn->meaning = current_node->meaning;

        current_dual_node = nn;
    }, node, created);

    return filtered;
}

Dictionary Dictionary::operator+(const Dictionary& d2) const{
    return merge(d2);
}

Dictionary Dictionary::operator+(Dictionary&& d2) const{
    return merge(move(d2));
}