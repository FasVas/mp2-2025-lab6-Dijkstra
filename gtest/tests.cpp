#include <gtest.h>
#include <vector>
#include <utility>
#include "triheap.h"
#include "binheap.h"

// ==========================================
// ТЕСТЫ ДЛЯ 3-КУЧИ (TriHeap)
// ==========================================

// Тестируем базовую вставку и извлечение минимума
TEST(TriHeapTest, InsertAndExtractMin) {
    vector<HeapElem> heap;

    // Вставляем пары {расстояние, вершина} в случайном порядке
    Heap_Insert(heap, { 10, 1 });
    Heap_Insert(heap, { 5, 2 });
    Heap_Insert(heap, { 20, 3 });
    Heap_Insert(heap, { 1, 4 });

    // Проверяем, что извлекаться они будут строго по возрастанию приоритета (дистанции)
    EXPECT_EQ(Heap_Extract(heap).first, 1);  // Самый маленький путь
    EXPECT_EQ(Heap_Extract(heap).first, 5);
    EXPECT_EQ(Heap_Extract(heap).first, 10);
    EXPECT_EQ(Heap_Extract(heap).first, 20); // Самый большой
}

// Тестируем, что куча корректно бросает исключение, если попытаться вытащить элемент из пустой кучи
TEST(TriHeapTest, ThrowsWhenEmpty) {
    vector<HeapElem> heap;

    // Проверяем, что вызов функции вызовет ошибку std::exception
    EXPECT_THROW(Heap_Extract(heap), exception);
}

// ==========================================
// ТЕСТЫ ДЛЯ БИНОМИАЛЬНОЙ КУЧИ
// ==========================================

// Тестируем базовое поведение биномиальной кучи
TEST(BinomialHeapTest, BasicOperations) {
    BinomialHeap<pair<int, int>> heap;

    // Проверяем начальное состояние
    EXPECT_TRUE(heap.is_empty());

    heap.insert({ 15, 0 });
    heap.insert({ 3, 1 });
    heap.insert({ 9, 2 });

    EXPECT_FALSE(heap.is_empty());

    // Извлекаем и проверяем порядок
    EXPECT_EQ(heap.extract_min().first, 3);
    EXPECT_EQ(heap.extract_min().first, 9);
    EXPECT_EQ(heap.extract_min().first, 15);

    // После извлечения всех элементов куча снова должна стать пустой
    EXPECT_TRUE(heap.is_empty());
}

// Тестируем обработку ошибок в биномиальной куче
TEST(BinomialHeapTest, ThrowsWhenEmpty) {
    BinomialHeap<int> heap;

    // Тут мы в хэдере бросали std::runtime_error
    EXPECT_THROW(heap.extract_min(), std::runtime_error);
}

// Эту функцию gtest требует для запуска, она заменяет наш привычный main
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}