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

    virtual void clear() {
        Node<T>* temp;

        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
    
public:
    List() : head(nullptr), length(0) {}

    ~List() {
        clear();
    }

    List(const List& other) : head(nullptr), length(0) {
        Node<T>* temp = other.head;

        while (temp) {
            push_back(temp->data);  
            temp = temp->next;
        }
    }

    List& operator=(const List& other) {
        if (this != &other) {  
            clear();
    
            auto temp = other.head;
            // Копирование элементов из другого списка
            while (temp) {
                push_back(temp->data); 
                temp = temp->next;
            }
    
            length = other.length;
        }

        return *this;
    }

    void push_back(T value) {
        Node<T>* newNode = new Node<T>(value);

        if (!head) {
            head = newNode;
        } else {
            auto temp = head;

            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }

        length++;
    }

    void insert(size_t pos, T value) {
        if (pos > length) return;

        auto newNode = new Node<T>(value);

        if (pos == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            Node<T>* temp;
            temp = head;

            for (size_t i = 0; i < pos - 1; ++i) temp = temp->next;

            newNode->next = temp->next;
            temp->next = newNode;
        }

        length++;
    }

    void erase(size_t pos) {
        if (pos >= length || !head) return;

        auto temp = head;

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

    const T& get(size_t pos) {
        if (pos >= length) {
            throw std::out_of_range("Index out of range");
        }
    
        auto temp = head;
        for (size_t i = 0; i < pos; ++i) {
            temp = temp->next;
        }
    
        return temp->data;
    }

    size_t size() const { return length; }

    void print() {
        auto temp = head;

        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }

        std::cout << "\n";
    }
};