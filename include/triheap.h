#ifndef TRIHEAP_HPP
#define TRIHEAP_HPP

#include <vector>
#include <exception>
#include <utility>

using namespace std;

// Пара: {расстояние, номер_вершины}
typedef pair<int, int> HeapElem;

void Heapify(vector<HeapElem>& A, size_t length, size_t i) {
    size_t l = 3 * i + 1;
    size_t m = 3 * i + 2;
    size_t r = 3 * i + 3;
    size_t smallest = i;

    if (l < length && A[l].first < A[smallest].first) smallest = l;
    if (m < length && A[m].first < A[smallest].first) smallest = m;
    if (r < length && A[r].first < A[smallest].first) smallest = r;

    if (smallest != i) {
        swap(A[i], A[smallest]);
        Heapify(A, length, smallest);
    }
}

void Build_Heap(vector<HeapElem>& A) {
    if (A.empty()) return;
    for (size_t i = (A.size() - 2) / 3; i != (size_t)(-1); i--)
        Heapify(A, A.size(), i);
}

void Heap_Decrease_Key(vector<HeapElem>& A, size_t i, HeapElem key) {
    if (key.first > A[i].first)
        throw exception("Новый ключ больше предыдущего (а мы уменьшаем!)");
    A[i] = key;
    while (i > 0 && A[(i - 1) / 3].first > A[i].first) {
        swap(A[(i - 1) / 3], A[i]);
        i = (i - 1) / 3;
    }
}

void Heap_Insert(vector<HeapElem>& A, HeapElem key) {
    // Вставляем "бесконечность" в конец и уменьшаем ключ до нужного
    A.push_back({ 2e9, key.second });
    Heap_Decrease_Key(A, A.size() - 1, key);
}

HeapElem Heap_Extract(vector<HeapElem>& A) {
    if (A.empty())
        throw exception("Куча пуста");
    HeapElem min_element = A[0];
    A[0] = A[A.size() - 1];
    A.pop_back();
    if (!A.empty()) {
        Heapify(A, A.size(), 0);
    }
    return min_element;
}

#endif