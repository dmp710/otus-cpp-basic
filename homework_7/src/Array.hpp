#pragma once

#include <cstddef>
#include <iostream>

// Последовательный контейнер (аналог динамического массива)
template <typename T>
class Array {
private:
    T* arr;
    size_t capacity;
    size_t current_size;

    void resize(size_t new_capacity) {
        T* new_arr = new T[new_capacity];

        for (size_t i = 0; i < current_size; ++i) {
            new_arr[i] = arr[i];
        }

        delete[] arr;
        arr = new_arr;
        capacity = new_capacity;
    }

public:
    Array() : arr(new T[2]), capacity(2), current_size(0) {}

    ~Array() {
        delete[] arr;
    }

    // Добавление элемента в конец
    void push_back(const T& value) {
        if (current_size == capacity) {
            resize(capacity * 2);
        }

        arr[current_size++] = value;
    }

    // Вставка элемента в произвольную позицию
    void insert(size_t index, const T& value) {
        if (index > current_size) return;

        if (current_size == capacity) {
            resize(capacity * 2);
        }

        for (size_t i = current_size; i > index; --i) {
            arr[i] = arr[i - 1];
        }

        arr[index] = value;
        ++current_size;
    }

    // Удаление элемента по индексу
    void erase(size_t index) {
        if (index >= current_size) return;

        for (size_t i = index; i < current_size - 1; ++i) {
            arr[i] = arr[i + 1];
        }

        --current_size;
    }

    // Получение текущего размера контейнера
    size_t size() const {
        return current_size;
    }

    // Оператор доступа по индексу
    T& operator[](size_t index) {
        return arr[index];
    }


    // Вывод содержимого контейнера
    void print() const {
        for (size_t i = 0; i < current_size; ++i) {
            std::cout << arr[i] << " ";
        }

        std::cout << std::endl;
    }

    // Конструктор копирования
    Array(const Array& other) : arr(new T[other.capacity]), capacity(other.capacity), current_size(other.current_size) {
        for (size_t i = 0; i < current_size; ++i) arr[i] = other.arr[i];
    }

    // Оператор присваивания копированием
    Array& operator=(const Array& other) {
    if (this != &other) {
        delete[] arr; // Освобождаем память
 
        current_size = other.current_size;
        arr = new T[other.capacity];

        for (size_t i = 0; i < current_size; ++i) arr[i] = other.arr[i];

    }

    return *this;
    }
};