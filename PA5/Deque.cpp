#include "Deque.h"
#include <fstream>

Deque create_deque()
{
    Deque new_Deque;
    new_Deque.sentinel = new Node;
    new_Deque.sentinel->next = new Node;
    new_Deque.sentinel->next->prev = new_Deque.sentinel;
    new_Deque.sentinel->next->next = new_Deque.sentinel;
    new_Deque.sentinel->prev = new_Deque.sentinel->next;
    set_node(new_Deque.start, new_Deque.sentinel->next);
    set_node(new_Deque.end, new_Deque.sentinel->next);
    new_Deque.start.current = new_Deque.sentinel->next->arr;
    new_Deque.end.current = new_Deque.sentinel->next->arr;
    new_Deque.ll_size = 1;
    return new_Deque;
}

void destroy_deque(Deque &deque)
{
    Node *head = deque.sentinel->next;
    while (head != deque.sentinel)
    {
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
    }
    delete deque.sentinel;
    deque.sentinel = nullptr;
    deque.ll_size = 0;
}

Iterator begin(const Deque &deque)
{
    return deque.start;
}

Iterator end(const Deque &deque)
{
    return deque.end;
}

int front(const Deque &deque)
{
    if (empty(deque))
    {
        cout << "Cannot get front: deque is empty" << endl;
        return -1;
    }
    else
    {
        return *deque.start.current;
    }
}

int back(const Deque &deque)
{
    if (empty(deque))
    {
        cout << "Cannot get back: deque is empty" << endl;
        return -1;
    }
    else
    {
        return *(deque.end.current - 1);
    }
}

bool empty(const Deque &deque)
{
    if (equal(deque.start, deque.end))
        return true;
    else
        return false;
}

int size(const Deque &deque)
{
    return CHUNK_SIZE * (deque.ll_size - 2) + deque.end.current - deque.end.node->arr + deque.start.last - deque.start.current;
}

void push_back(Deque &deque, int val)
{
    if (deque.end.current == deque.end.last - 1)
    {
        *(deque.end.current) = val;
        Node *new_node = new Node;
        deque.sentinel->prev->next = new_node;
        new_node->prev = deque.sentinel->prev;
        deque.sentinel->prev = new_node;
        new_node->next = deque.sentinel;
        set_node(deque.end, deque.sentinel->prev);
        deque.end.current = new_node->arr;
        deque.ll_size += 1;
    }

    else
    {
        *(deque.end.current) = val;
        ++deque.end.current;
    }
}

void push_front(Deque &deque, int val)
{   

    if (deque.start.current == deque.start.first)
    {
        Node *new_node = new Node;
        deque.sentinel->next->prev = new_node;
        new_node->next = deque.sentinel->next;
        deque.sentinel->next = new_node;
        new_node->prev = deque.sentinel;
        set_node(deque.start, new_node);
        deque.start.current = new_node->arr + CHUNK_SIZE;
        deque.ll_size += 1;
    }

    --deque.start.current;
    *(deque.start.current) = val;
}

void pop_back(Deque &deque)
{
    if (empty(deque))
    {
        cout << "Cannot pop_back: deque is empty" << endl;
        return;
    }
    else
    {
        if (deque.end.current == deque.end.node->arr)
        {
            deque.sentinel->prev->prev->next = deque.sentinel;
            deque.sentinel->prev = deque.sentinel->prev->prev;
            delete deque.end.node;
            set_node(deque.end, deque.sentinel->prev);
            deque.end.current = deque.end.last - 1;
            deque.ll_size -= 1;
        }
        else
        {
            deque.end.current -= 1;
        }
    }
}

void pop_front(Deque &deque)
{
    if (empty(deque))
    {
        cout << "Cannot pop_front: deque is empty" << endl;
        return;
    }
    else
    {
        if (deque.start.current == deque.start.node->arr + CHUNK_SIZE - 1)
        {
            deque.sentinel->next->next->prev = deque.sentinel;
            deque.sentinel->next = deque.sentinel->next->next;
            delete deque.start.node;
            set_node(deque.start, deque.sentinel->next);
            deque.start.current = deque.sentinel->next->arr;
            deque.ll_size -= 1;
        }
        else
        {
            deque.start.current += 1;
        }
    }
}

void print_deque(const Deque &deque)
{
    if (empty(deque))
    {
        cout << "[]" << endl;
        return;
    }
    else
    {
        cout << "[";
        Iterator cur = deque.start;
        if (deque.end.current == deque.end.node->arr)
        {
            while (cur.current != deque.end.node->prev->arr + CHUNK_SIZE - 1)
            {
                cout << *(cur.current) << ", ";
                cur = next(cur);
            }
        }
        else
        {
            while (cur.current != deque.end.current - 1)
            {
                cout << *(cur.current) << ", ";
                cur = next(cur);
            }
        }
        cout << *(cur.current) << "]" << endl;
    }
}

void insert(Deque &deque, const Iterator &pos, int val)
{
    if (equal(pos, deque.start))
    {
        push_front(deque, val);
        return;
    }
    else
    {
        Iterator cur = prev(pos);
        int temp1 = *(cur.current);
        int temp2;
        *(cur.current) = val;
        while (!equal(cur, deque.start))
        {
            cur = prev(cur);
            temp2 = *(cur.current);
            *(cur.current) = temp1;
            temp1 = temp2;
        }
        push_front(deque, temp1);
    }
}

void erase(Deque &deque, const Iterator &pos)
{
    if (equal(pos, deque.end))
    {
        return;
    }
    else
    {
        Iterator cur = pos;
        Iterator nxt = next(pos);
        while (!equal(nxt, deque.end))
        {
            *(cur.current) = *(nxt.current);
            cur = nxt;
            nxt = next(nxt);
        }
        *(cur.current) = *(nxt.current);
        pop_back(deque);
    }
}

void store_deque(const Deque &deque, const char *filename)
{
    ofstream ofs;
    ofs.open(filename);
    if (!empty(deque))
    {
        Iterator cur = deque.start;
        while (!equal(cur, deque.end))
        {
            ofs << *(cur.current) << endl;
            cur = next(cur);
        }
    }
    ofs.close();
}

Deque load_deque(const char *filename)
{
    ifstream ifs(filename);
    Deque new_deque = create_deque();
    int line;
    while (ifs >> line)
    {
        push_back(new_deque, line);
    }
    ifs.close();

    return new_deque;
}