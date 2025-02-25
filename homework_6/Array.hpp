#pragma once

#ifndef ARRAY_HPP
#define ARRAY_HPP

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

    // Конструктор перемещения
    Array(Array&& other) noexcept : arr(other.arr), capacity(other.capacity), current_size(other.current_size) {
        other.arr = nullptr;
        other.capacity = 0;
        other.current_size = 0;
    }

    // Оператор присваивания перемещением
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] arr;
            arr = other.arr;
            capacity = other.capacity;
            current_size = other.current_size;
            other.arr = nullptr;
            other.capacity = 0;
            other.current_size = 0;
        }

        return *this;
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
};

#endif // ARRAY_HPP