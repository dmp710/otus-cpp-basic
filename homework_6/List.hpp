#pragma once

template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node(T value) : data(value), next(nullptr) {}
};

template <typename T>
class List {
private:
    Node<T>* head;
    size_t length;

    Node<T>* temp;
public:
    List() : head(nullptr), length(0), temp(nullptr) {}

    ~List() {
        while (head) {
            head = head->next;
            delete temp;
        }
    }

    void push_back(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
        } else {
            temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }

        length++;
    }

    void insert(size_t pos, T value) {
        if (pos > length) return;

        Node<T>* newNode = new Node<T>(value);
        if (pos == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            temp = head;

            for (size_t i = 0; i < pos - 1; ++i) temp = temp->next;

            newNode->next = temp->next;
            temp->next = newNode;
        }

        length++;
    }

    void erase(size_t pos) {
        if (pos >= length || !head) return;

        temp = head;

        if (pos == 0) {
            head = head->next;
            delete temp;
        } else {
            Node<T>* prev = nullptr;
            for (size_t i = 0; i < pos; ++i) {
                prev = temp;
                temp = temp->next;
            }
            prev->next = temp->next;
            delete temp;
        }

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