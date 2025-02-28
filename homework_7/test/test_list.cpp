#include <gtest/gtest.h>
#include "List.hpp"

// Тест на создание контейнера
TEST(ListTest, CreateContainer) {
    List<int> list;

    EXPECT_EQ(list.size(), 0);
}

// Тест на вставку элементов в конец
TEST(ListTest, PushBack) {
    List<int> list;

    list.push_back(10);
    list.push_back(20);

    EXPECT_EQ(list.size(), 2);
}

// Тест на вставку элементов в начало
TEST(ListTest, InsertAtBeginning) {
    List<int> list;

    list.push_back(20);
    list.push_back(30);
    list.insert(0, 10);

    EXPECT_EQ(list.size(), 3);
}

// Тест на вставку элементов в середину
TEST(ListTest, InsertAtMiddle) {
    List<int> list;

    list.push_back(10);
    list.push_back(30);
    list.insert(1, 20);

    EXPECT_EQ(list.size(), 3);
}

// Тест на удаление элементов из конца
TEST(ListTest, EraseFromEnd) {
    List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.erase(1);

    EXPECT_EQ(list.size(), 1);
}

// Тест на удаление элементов из начала
TEST(ListTest, EraseFromBeginning) {
    List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.erase(0);

    EXPECT_EQ(list.size(), 1);
}

// Тест на удаление элементов из середины
TEST(ListTest, EraseFromMiddle) {
    List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.erase(1);

    EXPECT_EQ(list.size(), 2);
}

// Тест на получение размера контейнера
TEST(ListTest, GetSize) {
    List<int> list;
    list.push_back(10);
    list.push_back(20);

    EXPECT_EQ(list.size(), 2);

    list.erase(1);
    EXPECT_EQ(list.size(), 1);
}

// Тест конструктора копирования
TEST(ListTest, CopyConstructor) {
    List<int> original;

    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    List<int> copy{original}; // Используем конструктор копирования

    // Проверяем, что размеры одинаковые
    ASSERT_EQ(original.size(), copy.size());

    // Проверяем, что элементы одинаковые
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original.get(i), copy.get(i));
    }
}

// Тест оператора присваивания
TEST(ListTest, AssignmentOperator) {
    List<int> original;

    original.push_back(4);
    original.push_back(5);
    original.push_back(6);

    List<int> copy;
    copy = original; // Используем оператор присваивания

    // Проверяем, что размеры одинаковые
    ASSERT_EQ(original.size(), copy.size());

    // Проверяем, что элементы одинаковые
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original.get(i), copy.get(i));
    }
}