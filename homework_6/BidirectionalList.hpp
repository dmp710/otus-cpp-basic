#pragma once

#include <iostream>

template <typename T>
class BiNode {
public:
    T data;
    BiNode* next;
    BiNode* prev;
    BiNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class BidirectionalList {
private:
    BiNode<T>* head;
    BiNode<T>* tail;
    size_t length;

    BiNode<T>* temp;
public:
    BidirectionalList() : head(nullptr), tail(nullptr), length(0), temp(nullptr) {}

    ~BidirectionalList() {
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(T value) {
        BiNode<T>* newBiNode = new BiNode<T>(value);

        if (!tail) {
            head = tail = newBiNode;
        } else {
            tail->next = newBiNode;
            newBiNode->prev = tail;
            tail = newBiNode;
        }

        length++;
    }

    void insert(size_t pos, T value) {
        if (pos > length) return;

        BiNode<T>* newBiNode = new BiNode<T>(value);

        if (pos == 0) {
            newBiNode->next = head;

            if (head) head->prev = newBiNode;
            head = newBiNode;

            if (!tail) tail = newBiNode;
        } else {
            BiNode<T>* temp = head;

            for (size_t i = 0; i < pos - 1; ++i) temp = temp->next;

            newBiNode->next = temp->next;
            newBiNode->prev = temp;

            if (temp->next) temp->next->prev = newBiNode;
            temp->next = newBiNode;

            if (!newBiNode->next) tail = newBiNode;
        }

        length++;
    }

    void erase(size_t pos) {
        if (pos >= length) return;

        temp = head;

        for (size_t i = 0; i < pos; ++i) temp = temp->next;

        if (temp->prev) temp->prev->next = temp->next;
        if (temp->next) temp->next->prev = temp->prev;
        if (temp == head) head = temp->next;
        if (temp == tail) tail = temp->prev;

        length--;
    }

    size_t size() const { return length; }

    void print() {
        temp = head;

        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }

        std::cout << "\n";
    }
};