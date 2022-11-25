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
    root = nullptr;
    *this = d;
}

void copyFrom(Node* root, Dictionary& mod, string& savedString) { 
    if(!root) return;
    for(int i = 0; i < 26;i++) {
        Node* cur_node = (*root)[i];
        if(cur_node) {
            savedString.push_back(char(i+97));
            Node* node = mod.add_node(savedString.c_str()); 
            if(cur_node->meaning.meaning != "") node->meaning = cur_node->meaning;
            copyFrom(cur_node,mod,savedString);
            savedString.pop_back();
        }
    }
}

void copyFrom_E(Node* root, Node* modRoot) { 
    if(!root) return;
    for(int i = 0; i < 26;i++) {
        Node* cur_node = (*root)[i];
        if(cur_node) {
            Node *const& cur_mod_node = (*modRoot)[i];
            if(!cur_mod_node) {
                Node * new_node = new Node;
                modRoot->set_child(i,new_node);
                new_node->get_parent() = modRoot;
                modRoot = new_node;
            }
            else modRoot = cur_mod_node;
            if(cur_node->meaning.meaning != "") modRoot->meaning = cur_node->meaning;
            copyFrom_E(cur_node,modRoot);
        }
    }
}
/**
 * TODO - Assignment operator for deep copy of dictionary.
*/
Dictionary& Dictionary::operator=(const Dictionary& d){
    if (this != &d) 
    {   
        this->name = d.name;
        delete this->root;
        this->root = new Node;
        string savedString;
        copyFrom(d.root,*this,savedString);        
    }
    return *this;
}


/**
 * TODO - Adds a node with the given key string, which is terminated with '\0'.
 * You should assume the key consists of lowercase letters only.
 * Do not delete the pointer.
*/
Node* Dictionary::add_node(const char* key){
    int index = 0;
    Node * cur = root;
    while(key[index]!='\0') {
        int position = character_to_index(key[index]);
        Node *const& child = (*cur)[position];
        if(!child) {
            Node * new_node = new Node;
            cur->set_child(position,new_node);
            new_node->get_parent() = cur;
            cur = new_node;
        }
        else cur = child;
        index ++;
    }
    return cur;
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
    if(key[0] == '\0') {
        for(int i = 0;i < 26;i++) {
            if ((*root)[i]){
                delete (*root)[i];
                root->set_child(i,nullptr);
            }
        }
    }
    Node * node = find_node(key);
    if(node == nullptr || node == root) return;
    Node * parent = node->get_parent();
    string s = string(key);
    int position  = character_to_index(s.back());
    parent->set_child(position,nullptr);
    delete node;
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
    if(key[0] == '\0') return root;
    int index = 0;
    Node * cur = root;
    while(cur && key[index]!= '\0') {
        int position = character_to_index(key[index]);
        cur = (*cur)[position];
        index ++;
    }
    return cur;
}

template<typename T>
void foreach_traversal (Node* root, T& lambda, vector<int> &cur_key) {
    for(int i = 0; i < 26; i ++) {
        Node * node = (*root)[i];
        if (node) {
            cur_key.push_back(i);
            lambda(node,cur_key);
            foreach_traversal(node,lambda,cur_key);
            cur_key.pop_back();
        }
    }   
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
    vector<int> cur_key;
    foreach_traversal(root,lambda,cur_key);
}

/**
 * TODO - Prints all the nodes in the dictionary, in depth first alphabetical order.
 * See the webpage description for more details.
*/
void Dictionary::print_all_elements(ostream& o) const{
    int count = 0;
    foreach([&](Node* current_node, vector<int>& current_key) {
        string name;
        for(int i = 0; i < current_key.size();i++) name.push_back(static_cast<char>(current_key[i] + 97));
        o << name << current_node << "[" << ++count << "]\n";
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
    int count = 0;
    foreach([&](Node* current_node, vector<int>& current_key) {
        if(!current_node->meaning.meaning.empty() && (type == nullptr || !current_node->meaning.type.compare(string(type)))){
            string name;
            for(int i = 0; i < current_key.size();i++) name.push_back(static_cast<char>(current_key[i] + 97));
            cout << name << current_node << "[" << ++count << "]\n";
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
    Dictionary output(d2);
    string savedString;
    copyFrom(this->root,output,savedString); 
    output.name = "";
    return output;
}

/**
 * TODO - Merges with another dictionary. Moves the contents of d2
 * into a new dictionary. For words that exists in both dictionary,
 * use the word definition in the dictionary this.
*/
Dictionary Dictionary::merge(Dictionary&& d2) const{
    Dictionary output("");
    delete output.root;
    output.root = d2.root;
    d2.root = nullptr;
    copyFrom_E(this->root,output.root); 
    return output;
}

/**
 * TODO - Creates a new dictionary, consisting only of the words
 * starting with the given key.
*/
Dictionary Dictionary::filter_starting_word(const char* key) const{
    if(!key || key[0] == '\0') {
        Dictionary output(*this);
        return output;
    }
    Dictionary output({});
    Node * tarNode = find_node(key);
    Node * newNode = output.add_node(key);
    if(!tarNode) return output;
    newNode->meaning = tarNode->meaning;
    string savedString;
    savedString += string(key); 
    copyFrom(tarNode,output,savedString);
    return output;
}

Dictionary Dictionary::operator+(const Dictionary& d2) const{
    return merge(d2);
}

Dictionary Dictionary::operator+(Dictionary&& d2) const{
    return merge(move(d2));
}