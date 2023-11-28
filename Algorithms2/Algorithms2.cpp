/*
                    2.2. Линейные списки

   Общими требованиями к лабораторной работе являются:
   1) организовать  ввод  данных  из  файла  в   понятной  для
пользователя форме;
   2) обеспечить   возможность   многократных   запросов   без
повторного запуска программы;
   3) при реализации в С++ не использовать контейнерные классы
для работы с линейными списками типа stack, queue и т. п.


Цветников Валериан ПС-23. Лабораторная работа №2
Среда разработки Visual Studio Code v1.81.1
Компилятор gcc version 7.5.0, Ubuntu 18.04

   20. Организовать  в основной  памяти  с помощью указателей
очередь  из  стеков. Обеспечить  операции  ведения  стека  из
начала  очереди, дополнения  и  продвижения  очереди,  выдачи
содержимого очереди (9).
*/


#include <iostream>
#include <fstream>
#include <sstream>

class Node {
public:
    int data;
    Node* next;

    Node(int data) {
        this->data = data;
        this->next = nullptr;
    }
};


class Stack {
private:
    Node* head;
    int stackSize;

public:
    Stack() {
        head = nullptr;
        stackSize = 0;
    }

    void push(int data) {
        Node* newNode = new Node(data);
        if (isEmpty()) {
            head = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
        stackSize++;
    }

    int pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty!");
            return 0;
        }
        Node* temp = head;
        int value = head->data;
        head = head->next;
        delete temp;
        stackSize--;
        return value;
    }

    int size() {
        return stackSize;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    int peek() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty!");
            return 0;
        }
        else {
            return head->data;
        }
    }

    void display() {
        Node* temp = head;
        if (temp != nullptr) {
            std::cout << temp->data;
            temp = temp->next;
        }
        while (temp != nullptr) {
            std::cout << " " << temp->data;
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    void clear() {
        while (!isEmpty()) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~Stack() {
        std::cout << "Stack Destructor" << std::endl;
    }
};


class StackNode {
public:
    Stack& stack;
    StackNode* next;

    StackNode(Stack& stack) : stack(stack), next(nullptr) {
    }
};


class StackQueue {
private:
    StackNode* head;
    StackNode* tail;
    int queueSize;

public:
    StackQueue() {
        head = nullptr;
        tail = nullptr;
        queueSize = 0;
    }

    void enqueue(Stack& stack) {
        StackNode* newNode = new StackNode(stack);
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        queueSize++;
    }

    Stack& dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty!");
        }
        StackNode* temp = head;
        Stack& stackObj = head->stack;
        head = head->next;
        delete temp;
        queueSize--;
        return stackObj;
    }

    int size() {
        return queueSize;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    Stack& front() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty!");
        }
        else {
            return head->stack;
        }
    }

    void display() {
        StackNode* temp = head;
        while (temp != nullptr) {
            temp->stack.display();
            temp = temp->next;
        }
    }

    void clear() {
        while (!isEmpty()) {
            StackNode* temp = head;
            head = head->next;
            temp->stack.clear();
            delete temp;
        }
    }

    ~StackQueue() {
        std::cout << "StackQueue Destructor" << std::endl;
    }
};


int main() {
    setlocale(LC_ALL, "RU");
    StackQueue queue;

    std::cout << "Info:" << std::endl;
    std::cout << "            First queue element ↑" << std::endl;
    std::cout << "← Last stack element    First stack element →" << std::endl;
    std::cout << "            Last queue element ↓" << std::endl;

    while (true) {
        std::cout << "#########################" << std::endl;
        if (!queue.isEmpty()) {
            std::cout << "Текущая очередь: " << std::endl;
            queue.display();
            std::cout << std::endl;
        }
        else {
            std::cout << "Текущая очередь пустая: " << std::endl;
        }

        std::cout << "Command list:" << std::endl;
        std::cout << "1. Enqueue stack" << std::endl;
        std::cout << "2. Dequeue stack" << std::endl;
        std::cout << "3. Show front stack" << std::endl;
        std::cout << "4. Queue size" << std::endl;
        std::cout << "5. Изменить верхний стек" << std::endl;
        std::cout << "6. Выйти из программы" << std::endl;
        std::cout << "Выберите команду (1/2/3/4/5/6): ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            Stack* stack = new Stack();
            std::cout << "Введите элементы стека через пробел (или нажмите Enter для завершения ввода): ";
            std::cin.get();
            std::string inputLine;
            while (true) {
                getline(std::cin, inputLine);
                std::istringstream iss(inputLine);
                int element;
                while (iss >> element) {
                    stack->push(element);
                }
                break;
            }
            if (!stack->isEmpty()) {
                queue.enqueue(*stack);
                std::cout << "Стек добавлен в очередь." << std::endl;
            }
            else {
                std::cout << "Введён пустой стек." << std::endl;
            }

            break;
        }
        case 2: {
            if (!queue.isEmpty()) {
                Stack& frontStack = queue.dequeue();
                std::cout << "Содержимое первого стека в очереди:" << std::endl;
                frontStack.display();
                frontStack.clear();
            }
            else {
                std::cout << "Очередь пустая!" << std::endl;
            }
            break;
        }
        case 3: {
            if (!queue.isEmpty()) {
                Stack& frontStack = queue.front();
                std::cout << "Содержимое первого стека в очереди:" << std::endl;
                frontStack.display();
            }
            else {
                std::cout << "Очередь пустая!" << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "Количество элементов в очереди: " << queue.size() << std::endl;
            break;
        }
        case 5: {
            if (queue.isEmpty()) {
                std::cout << "Очередь пустая!" << std::endl;
                break;
            }
            while (true) {
                std::cout << "#########################" << std::endl << "Верхний стек: " << std::endl;
                queue.front().display();
                std::cout << "Список команд над стеком:" << std::endl;
                std::cout << "1. Push" << std::endl;
                std::cout << "2. Pop" << std::endl;
                std::cout << "3. Peek" << std::endl;
                std::cout << "4. Size" << std::endl;
                std::cout << "5. Назад в меню" << std::endl;
                std::cout << "Выберите команду (1/2/3/4/5): ";
                std::cin >> choice;

                if (choice == 5) {
                    break;
                }
                switch (choice) {
                case 1:
                    int number;
                    std::cout << "Введите элемент, который нужно добавить в стек: ";
                    std::cin >> number;
                    queue.front().push(number);
                    break;
                case 2:
                    if (!queue.front().isEmpty()) {
                        std::cout << "Верхний элемент стека: " << queue.front().pop() << std::endl;
                    }
                    else {
                        std::cout << "Стек пустой!" << std::endl;
                    }
                    break;
                case 3:
                    if (!queue.front().isEmpty()) {
                        std::cout << "Верхний элемент стека: " << queue.front().peek() << std::endl;
                    }
                    else {
                        std::cout << "Стек пустой!" << std::endl;
                    }
                    break;
                case 4:
                    std::cout << "Количество элементов стека: " << queue.front().size() << std::endl;
                    break;
                default: {
                    std::cout << "Некорректная команда. Пожалуйста, выберите команду из списка (1/2/3/4/5)." << std::endl;
                    queue.clear();
                    return 1;
                }
                }
            }
            break;
        }
        case 6: {
            std::cout << "Выход из программы." << std::endl;
            return 0;
        }
        default: {
            std::cout << "Некорректная команда. Пожалуйста, выберите команду из списка (1/2/3/4/5/6)." << std::endl;
            queue.clear();
            return 1;
        }
        }
    }

    queue.clear();

    return 0;
}