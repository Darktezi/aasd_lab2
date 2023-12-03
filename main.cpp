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
    // ����������� �� ���������
    LinkedList() : _head(nullptr) {}

    // ����������� �����������
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

    // �����������, ����������� ������ ���������� ����������
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
            // ������ ���� ������ ����� ���� ���������� �����
            static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "Unsupported type for LinkedList");
        }
    }

    // ����������
    ~LinkedList() {
        if (_head) {
            Node<T>* temp = _head->next;
            _head->next = nullptr; // ������ ����������� �����
            while (temp) {
                Node<T>* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }

    // �������� ������������
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            LinkedList<T> temp(other);
            std::swap(_head, temp._head);
        }
        return *this;
    }

    // ���������� �������� � ����� ������
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

    // ������������� ����� ���������� ������� ������ � ����� �������� ������
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

    // ���������� �������� � ������ ������
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

    // ������������� ����� ���������� ������� ������ � ������ �������� ������
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

    // ����� ��� ������ ������
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
    SetConsoleOutputCP(1251);

    LinkedList<int> intList;
    intList.push_tail(10);
    intList.push_tail(20);
    intList.push_tail(30);

    intList.display();

    LinkedList<int> additionalIntList;
    additionalIntList.push_head(40);
    additionalIntList.push_head(50);

    additionalIntList.display();

    // ���������� ������ intList � ������ additionalIntList
    additionalIntList.push_head(intList);

    std::cout << "������ ����� �����: ";
    additionalIntList.display();

    return 0;
}