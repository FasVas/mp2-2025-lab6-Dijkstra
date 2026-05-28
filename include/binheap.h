#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class BinomialHeap {
private:
    struct Node {
        T key;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;

        Node(T val) : key(val), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
    };

    Node* head;

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->child);
        clear(node->sibling);
        delete node;
    }

    void link_trees(Node* child, Node* parent) {
        child->parent = parent;
        child->sibling = parent->child;
        parent->child = child;
        parent->degree++;
    }

    Node* merge_root_lists(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        Node* new_head = nullptr;
        Node** tail = &new_head;

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *tail = h1;
                h1 = h1->sibling;
            }
            else {
                *tail = h2;
                h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = (h1) ? h1 : h2;
        return new_head;
    }

    Node* union_heaps(Node* h1, Node* h2) {
        Node* new_head = merge_root_lists(h1, h2);
        if (!new_head) return nullptr;

        Node* prev = nullptr;
        Node* curr = new_head;
        Node* next = curr->sibling;

        while (next != nullptr) {
            if ((curr->degree != next->degree) ||
                (next->sibling != nullptr && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            }
            else if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                link_trees(next, curr);
            }
            else {
                if (prev == nullptr) {
                    new_head = next;
                }
                else {
                    prev->sibling = next;
                }
                link_trees(curr, next);
                curr = next;
            }
            next = curr->sibling;
        }
        return new_head;
    }

    Node* reverse_list(Node* node) {
        Node* prev = nullptr;
        Node* curr = node;
        Node* next = nullptr;
        while (curr != nullptr) {
            next = curr->sibling;
            curr->sibling = prev;
            curr->parent = nullptr;
            prev = curr;
            curr = next;
        }
        return prev;
    }

public:
    BinomialHeap() : head(nullptr) {}
    ~BinomialHeap() { clear(head); }

    BinomialHeap(const BinomialHeap&) = delete;
    BinomialHeap& operator=(const BinomialHeap&) = delete;

    bool is_empty() const { return head == nullptr; }

    void insert(T key) {
        Node* new_node = new Node(key);
        head = union_heaps(head, new_node);
    }

    T extract_min() {
        if (is_empty()) {
            throw std::runtime_error("Куча пуста");
        }

        Node* min_node = head;
        Node* prev_min = nullptr;
        Node* curr = head;
        Node* prev = nullptr;

        while (curr != nullptr) {
            if (curr->key < min_node->key) {
                min_node = curr;
                prev_min = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }

        if (prev_min == nullptr) {
            head = min_node->sibling;
        }
        else {
            prev_min->sibling = min_node->sibling;
        }

        Node* child_head = reverse_list(min_node->child);
        T extracted_key = min_node->key;
        delete min_node;

        head = union_heaps(head, child_head);
        return extracted_key;
    }
};

#endif