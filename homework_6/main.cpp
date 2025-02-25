#include <Array.hpp>
#include <List.hpp>
#include <BidirectionalList.hpp>
#include <iostream>

using namespace std;

int main() {
    cout << "array" << endl;
    Array<int> arr;

    // Добавляем 10 элементов
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i);
    }

    // Выводим содержимое контейнера
    arr.print();  // Ожидаемый результат: 0 1 2 3 4 5 6 7 8 9
    std::cout << "Size: " << arr.size() << std::endl;  // Ожидаемый результат: 10

    // Удаляем 3-й, 5-й и 7-й элементы
    arr.erase(2);  
    arr.erase(3);  
    arr.erase(4); 

    arr.print();  

    // Добавляем элемент 10 в начало
    arr.insert(0, 10);

    arr.print(); 

    // Добавляем элемент 20 в середину
    arr.insert(4, 20);

    arr.print(); 

    arr.push_back(30);

    arr.print(); 

    Array<int> temp_arr;

    temp_arr.push_back(100);
    temp_arr.push_back(500);

    arr = std::move(temp_arr);

    arr.print();

    Array<int> new_arr(std::move(arr));
    new_arr.push_back(1000);

    new_arr.print();
    arr.print();

    cout << "list" << endl;
    List<int> list;

    for (int i = 0; i < 10; ++i) list.push_back(i);
    list.print();
    std::cout << "Size: " << list.size() << "\n";

    list.erase(2);
    list.erase(3);
    list.erase(4);
    list.print();

    list.insert(0, 10);
    list.print();

    list.insert(4, 20);
    list.print();

    list.push_back(30);
    list.print();

    cout << "bidirectional list" << endl;
    BidirectionalList<int> bidirlist;

    for (int i = 0; i < 10; ++i) bidirlist.push_back(i);

    bidirlist.print();

    std::cout << "Size: " << list.size() << "\n";

    bidirlist.erase(2);
    bidirlist.erase(3);
    bidirlist.erase(4);
    bidirlist.print();

    bidirlist.insert(0, 10);
    bidirlist.print();

    bidirlist.insert(4, 20);
    bidirlist.print();

    bidirlist.push_back(30);
    bidirlist.print();
}