#include <iostream>
#include <cstdlib>
#include <random>
#include <windows.h>

template<typename T>
struct Node {
    T data;
    Node* next;

    Node() : next(nullptr) {}
    Node(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    Node<T>* _head;

public:
    // Конструктор по умолчанию
    LinkedList() : _head(nullptr) {}

    // Конструктор копирования
    LinkedList(const LinkedList<T>& other) : _head(nullptr) {
        Node<T>* otherPtr = other._head;
        if (otherPtr) {
            _head = new Node<T>(otherPtr->data);
            Node<T>* temp = _head;
            otherPtr = otherPtr->next;
            while (otherPtr != other._head) {
                temp->next = new Node<T>(otherPtr->data);
                otherPtr = otherPtr->next;
                temp = temp->next;
            }
            temp->next = _head;
        }
    }

    // Конструктор, заполняющий список случайными значениями
    LinkedList(int size) : _head(nullptr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dis(-100, 100);
            for (int i = 0; i < size; ++i) {
                T value = dis(gen);
                if (!_head) {
                    _head = new Node<T>(value);
                    _head->next = _head;
                }
                else {
                    Node<T>* temp = _head;
                    while (temp->next != _head) {
                        temp = temp->next;
                    }
                    temp->next = new Node<T>(value);
                    temp->next->next = _head;
                }
            }
        }
        else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dis(-100, 100);
            for (int i = 0; i < size; ++i) {
                T value = dis(gen);
                if (!_head) {
                    _head = new Node<T>(value);
                    _head->next = _head;
                }
                else {
                    Node<T>* temp = _head;
                    while (temp->next != _head) {
                        temp = temp->next;
                    }
                    temp->next = new Node<T>(value);
                    temp->next->next = _head;
                }
            }
        }
        else {
            // Другие типы данных могут быть обработаны здесь
            static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "Unsupported type for LinkedList");
        }
    }

    // Деструктор
    ~LinkedList() {
        if (_head) {
            Node<T>* temp = _head->next;
            _head->next = nullptr; // Разрыв циклической связи
            while (temp) {
                Node<T>* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }

    // Операция присваивания
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            LinkedList<T> temp(other);
            std::swap(_head, temp._head);
        }
        return *this;
    }

    // Добавление элемента в конец списка
    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!_head) {
            _head = newNode;
            _head->next = _head;
        }
        else {
            Node<T>* temp = _head;
            while (temp->next != _head) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = _head;
        }
    }

    // Перегруженный метод добавления другого списка в конец текущего списка
    void push_tail(const LinkedList<T>& otherList) {
        if (!otherList._head) {
            return;
        }

        Node<T>* otherTemp = otherList._head;
        do {
            push_tail(otherTemp->data);
            otherTemp = otherTemp->next;
        } while (otherTemp != otherList._head);
    }

    // Добавление элемента в начало списка
    void push_head(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!_head) {
            _head = newNode;
            _head->next = _head;
        }
        else {
            Node<T>* temp = _head;
            while (temp->next != _head) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = _head;
            _head = newNode;
        }
    }

    // Перегруженный метод добавления другого списка в начало текущего списка
    void push_head(const LinkedList<T>& otherList) {
        if (otherList._head == nullptr) {
            throw std::out_of_range("List is empty");
        }

        Node<T>* otherCurrent = otherList._head;
        Node<T>* lastAdded = nullptr;

        do {
            Node<T>* newNode = new Node<T>(otherCurrent->data);
            newNode->next = lastAdded;
            lastAdded = newNode;

            otherCurrent = otherCurrent->next;
        } while (otherCurrent != otherList._head);

        Node<T>* current = lastAdded;
        do {
            push_head(current->data);
            current = current->next;
        } while (current != nullptr);
    }

    // Удаление элемента из начала списка
    void pop_head() {
        if (!_head) {
            std::cout << "List is empty. Cannot pop from empty list." << std::endl;
            return;
        }

        Node<T>* temp = _head;
        Node<T>* last = _head;

        while (last->next != _head) {
            last = last->next;
        }

        if (temp->next == _head) {
            delete temp;
            _head = nullptr;
        }
        else {
            _head = temp->next;
            last->next = _head;
            delete temp;
        }
    }

    // Удаление элемента из конца списка
    void pop_tail() {
        if (!_head) {
            std::cout << "List is empty. Cannot pop from empty list." << std::endl;
            return;
        }

        Node<T>* temp = _head;
        Node<T>* last = nullptr;

        while (temp->next != _head) {
            last = temp;
            temp = temp->next;
        }

        if (last == nullptr) {
            delete temp;
            _head = nullptr;
        }
        else {
            last->next = _head;
            delete temp;
        }
    }

    // Удаление всех элементов Node с информационным полем, равным переданному
    void delete_node(const T& value) {
        if (!_head) {
            std::cout << "List is empty. No elements to delete." << std::endl;
            return;
        }

        Node<T>* temp = _head;
        Node<T>* prev = nullptr;
        Node<T>* toDelete = nullptr;
        bool found = false;

        do {
            if (temp->data == value) {
                toDelete = temp;
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        } while (temp != _head);

        if (!found) {
            std::cout << "Element with value " << value << " not found in the list." << std::endl;
            return;
        }

        if (toDelete == _head) {
            pop_head();
        }
        else {
            prev->next = toDelete->next;
            delete toDelete;
        }
    }

    // Метод для вывода списка
    void display() const {
        if (!_head) {
            std::cout << "List is empty" << std::endl;
            return;
        }

        Node<T>* temp = _head;
        do {
            std::cout << temp->data << " ";
            temp = temp->next;
        } while (temp != _head);
        std::cout << std::endl;
    }
};

int main() {
    // Создание списка типа int и добавление элементов
    LinkedList<int> intList;
    intList.push_tail(10);
    intList.push_tail(20);
    intList.push_tail(30);
    intList.push_tail(40);
    intList.display();

    // Удаление элемента из начала списка
    intList.pop_head();
    intList.display();

    // Удаление элемента из конца списка
    intList.pop_tail();
    intList.display();

    // Создание списка типа double и добавление элементов
    LinkedList<double> doubleList;
    doubleList.push_tail(3.14);
    doubleList.push_tail(6.28);
    doubleList.push_tail(9.42);
    doubleList.display();

    // Удаление элемента из начала списка
    doubleList.pop_head();
    doubleList.display();

    // Удаление всех элементов Node с информационным полем, равным 6.28
    doubleList.delete_node(6.28);
    doubleList.display();

    return 0;
}