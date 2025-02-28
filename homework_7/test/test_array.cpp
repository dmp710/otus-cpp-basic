#include <gtest/gtest.h>
#include "Array.hpp"

// Тест на создание контейнера
TEST(ArrayTest, CreateContainer) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
}

// Тест на вставку элементов в конец
TEST(ArrayTest, PushBack) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
}

// Тест на вставку элементов в начало
TEST(ArrayTest, InsertAtBeginning) {
    Array<int> arr;
    arr.push_back(20);
    arr.push_back(30);
    arr.insert(0, 10);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

// Тест на вставку элементов в середину
TEST(ArrayTest, InsertAtMiddle) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(30);
    arr.insert(1, 20);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

// Тест на удаление элементов из конца
TEST(ArrayTest, EraseFromEnd) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.erase(1);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 10);
}

// Тест на удаление элементов из начала
TEST(ArrayTest, EraseFromBeginning) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.erase(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 20);
}

// Тест на удаление элементов из середины
TEST(ArrayTest, EraseFromMiddle) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    arr.erase(1);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 30);
}

// Тест на получение элементов из контейнера
TEST(ArrayTest, AccessElements) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
}

// Тест на получение размера контейнера
TEST(ArrayTest, GetSize) {
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    EXPECT_EQ(arr.size(), 2);
    arr.erase(1);
    EXPECT_EQ(arr.size(), 1);
}

void test_copy_constructor() {
    Array<int> arr1;
    arr1.push_back(10);
    arr1.push_back(20);
    arr1.push_back(30);

    // Используем конструктор копирования
    Array<int> arr2 = arr1;

    // Проверяем, что содержимое обоих контейнеров одинаково
    assert(arr1.size() == arr2.size());
    for (size_t i = 0; i < arr1.size(); ++i) {
        assert(arr1[i] == arr2[i]);
    }

    std::cout << "Copy constructor test passed!" << std::endl;
}

void test_copy_assignment() {
    Array<int> arr1;
    arr1.push_back(10);
    arr1.push_back(20);
    arr1.push_back(30);

    Array<int> arr2;
    arr2.push_back(100); // Начальное содержимое arr2

    // Используем оператор присваивания копированием
    arr2 = arr1;

    // Проверяем, что содержимое обоих контейнеров одинаково
    assert(arr1.size() == arr2.size());
    for (size_t i = 0; i < arr1.size(); ++i) {
        assert(arr1[i] == arr2[i]);
    }

    std::cout << "Copy assignment test passed!" << std::endl;
}