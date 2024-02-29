#pragma once

#include <iostream>
#include <functional>
#include <cstddef>

//header
class node {
public:
    node *next;
    int data;

    explicit node(int);

    ~node();

    friend std::ostream &operator<<(std::ostream &os, const node &nd) {
        os << nd.data;
        return os;
    };
};

class bucket {
    node *root;
    node **current;

public:
    bucket();

    ~bucket();

    void add_node(int);

    void remove_node(int);

    bool has_key(int);

    node *current_node();

    void next_node();

    void restart();

    friend std::ostream &operator<<(std::ostream &os, const bucket &en) {
        if (!en.root) {
            return os;
        }
        node *x = en.root;
        while (x->next) {
            os << *x << ", ";
            x = x->next;
        }
        os << *x;
        return os;
    }
};

class hash_set {
    bucket *entries;
    int num_entries;
    int card;

    void _add(int);

    [[nodiscard]] int redundancy() const;

    void resize(int);

    [[nodiscard]] std::size_t hash(int) const;

public:
    hash_set();

    ~hash_set();

    void add(int);

    void remove(int);

    bool has_key(int);

    [[nodiscard]] int size() const;

    friend std::ostream &operator<<(std::ostream &os, const hash_set &ht) {
        int first = 0;
        while (!ht.entries[first].current_node()) {
            first++;
        }
        os << "{" << ht.entries[first];
        for (int i = first + 1; i < ht.num_entries; i++) {
            if (ht.entries[i].current_node()) {
                os << ", " << ht.entries[i];
            }
        }
        os << "}";
        return os;
    };

    friend hash_set operator+(const hash_set &first, const hash_set &second) {
        hash_set result;

        for (int i = 0; i < first.num_entries; i++) {
            if (first.entries[i].current_node()) {
                result.add(first.entries[i].current_node()->data);
            }
        }
        for (int i = 0; i < second.num_entries; i++) {
            if (second.entries[i].current_node()) {
                result.add(second.entries[i].current_node()->data);
            }
        }
        return result;

    }
};

node::node(int key) {
    data = key;
    next = nullptr;
}

node::~node() {
    delete next;
}

// end node methods


// bucket methods
bucket::bucket() {
    root = nullptr;
    current = &root;
}

bucket::~bucket() {
    delete root;
}

void bucket::add_node(int key) {
    if (!root) {
        root = new node(key);
        return;
    }
    //__________

    node *x = root;
    while (x->data != key) {
        if (x->next) {
            x = x->next;
        } else {
            x->next = new node(key);
            return;
        }
    }
}

void bucket::remove_node(int key) {
    if (!root) { return; }


    if (root->data == key) {
        node *old = root;
        root = root->next;
        old->next = nullptr;
        delete old;
        return;
    }


    node *x = root;
    while (x->next) {
        if (x->next->data == key) {
            node *old = x->next;
            x->next = x->next->next;
            old->next = nullptr;
            delete old;
            return;
        }
        x = x->next;
    }
}

bool bucket::has_key(int key) {
    node *x = root;
    while (x) {
        if (x->data == key) { return true; }
        x = x->next;
    }
    return false;
}

node *bucket::current_node() {
    return *current;
}

void bucket::next_node() {
    current = &(*current)->next;
}

void bucket::restart() {
    current = &root;
}


//end bucket


//begin hashSet

hash_set::hash_set() {
    num_entries = 10;
    card = 0;
    entries = new bucket[num_entries];
}

hash_set::~hash_set() {
    delete[] entries;
}

std::size_t hash_set::hash(int key) const {
    return std::hash<int>{}(key) % num_entries;
}

void hash_set::_add(int key) {
    std::size_t address = hash(key);
    entries[address].add_node(key);
    card++;
}

void hash_set::add(int key) {
    if (redundancy() > 3) {
        resize(card + 1);
    }
    _add(key);
}

void hash_set::remove(int key) {
    std::size_t address = hash(key);
    entries[address].remove_node(key);
    card--;
}

bool hash_set::has_key(int key) {
    std::size_t address = hash(key);
    return entries[address].has_key(key);
}

int hash_set::redundancy() const {
    return card / num_entries;
}

void hash_set::resize(int newSize) {
    bucket *old_entries = entries;
    const int old_num_entries = num_entries;

    num_entries = newSize;
    entries = new bucket[num_entries];

    for (int i = 0; i < old_num_entries; i++) {
        while (old_entries[i].current_node()) {
            add(old_entries[i].current_node()->data);
            old_entries[i].next_node();
        }
    }
    delete[] old_entries;
}

int hash_set::size() const {
    return card;
}