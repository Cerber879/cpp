// 16. У множества шпионов, собравшихся вместе для наблюдения
// секретного     объекта, имеется    единственный    бинокль.
// Сформировалась очередь на этот  бинокль. Для  каждого  шпиона
// задан   период   наблюдения   в  минутах  и  предельное  время
// нахождения в очереди. После наблюдения каждый шпион становится
// снова  в  конец  очереди. Как  только  для какого - либо шпиона
// истекает предельное время нахождения в очереди, он покидает ее
// (даже  если  в  этот момент владеет биноклем) и отправляется к
// резиденту.Вывести протокол наблюдения шпионов за объектом(9).

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class SpyObservation {
private:
    int* data; 
    int sumData;
    int capacity;  
    int front; 
    int rear; 
    int count; 

public:
    SpyObservation(int capacity) {
        this->capacity = capacity;
        data = new int[capacity];
        sumData = 0;
        front = 0;
        rear = -1;
        count = 0;
    }

    void enqueue(int item) {
        if (count == capacity) {
            return;
        }

        rear = (rear + 1) % capacity;
        data[rear] = item;
        count++;
    }

    void dequeue() {
        if (count == 0) {
            // Очередь пуста
            return;
        }

        front = (front + 1) % capacity;
        count--;
    }

    int getFront() {
        if (count == 0) {
            // Очередь пуста
            return -1;
        }

        return data[front];
    }

    int getElemID(int i) {
        if (count == 0) {
            // Очередь пуста
            return -1;
        }

        return data[i];
    }

    int returnSumData() const {
        return sumData;
    }

    void changeSumData(int count) {
        this->sumData = count;
    }

    void printCapacity() {
        std::cout << "capacity: " << capacity << std::endl;
    }

    int returnCapacity() const {
        return capacity;
    }

    int returnCount() const {
        return count;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }

    ~SpyObservation() {
        delete[] data;
    }
};

class QueNode {
public:
    QueNode* Next = nullptr;
    SpyObservation* spy = nullptr;
    int maxQueTime = -1;
    int observationPeriod = -1;
    int indexSpyObeserv = 0;
    int indexSpy = 0;
};

class Queue {
private:
    QueNode* FirstPtr;
    QueNode* Prev;
    int countSpy;
public:

    Queue() : FirstPtr(nullptr), Prev(nullptr), countSpy(0) {}

    QueNode* getFirstPtr() const {
        return FirstPtr;
    }

    void setFirstPtr(QueNode* ptr) {
        FirstPtr = ptr;
    }

    int getCount() const {
        return countSpy;
    }

    void setCount(int count) {
        countSpy = count;
    }

    void enqueue(int count) {

        /*if (count == maxSize) {
            std::cout << "Очередь уже заполнена. Пожалуйста, удалите некоторых шпионов перед добавлением новых." << std::endl;
            return;
        }*/

        QueNode* NewPtr;
        // Ввод данных
        std::cout << "Введите через пробел по 2 значения в строчке для каждого шпиона и, если хотите, время кажого наблюдения" << std::endl;
        std::cout << "Они означают период наблюдения в минутах и предельное время нахождения в очереди" << std::endl;
        std::cout << "Для завершения ввода введите пустую строку:" << std::endl;
        std::string inputLine;
        std::cin.get();
        int ix = 1;
        while (std::getline(std::cin, inputLine)) {
            if (inputLine.empty()) break;

            NewPtr = new QueNode;
            NewPtr->spy = new SpyObservation(count);

            std::istringstream iss(inputLine);
            iss >> NewPtr->maxQueTime >> NewPtr->observationPeriod;
            NewPtr->indexSpy = ix;
            int i = 0;
            int sum = 0;
            for (i; i < count; i++) {
                if (iss.rdbuf()->in_avail() != 0) {
                    int item;
                    iss >> item;
                    if (item < 0) {
                        std::cout << "Введены неккоректные значения!" << std::endl;
                        delete NewPtr;
                        break;
                    }
                    NewPtr->spy->enqueue(item);
                    sum += item;
                }
                else break;
            }

            if (NewPtr->observationPeriod < 0 || NewPtr->maxQueTime < 0) {
                std::cout << "Введены неккоректные значения!" << std::endl;
                delete NewPtr;
                break;
            }

            if (sum != NewPtr->observationPeriod && sum != 0){
                std::cout << "Сумма наблюдений должна быть равна общему периоду наблюдения!" << std::endl;
                delete NewPtr;
                break;
            }
            else if (i != 0) {
                NewPtr->spy->changeSumData(sum);
            }

            NewPtr->Next = nullptr;

            if (FirstPtr == nullptr) {
                FirstPtr = NewPtr;
            }
            else {
                Prev->Next = NewPtr;
            }
            Prev = NewPtr;
            ix++;
            countSpy++;
        }
   
    }

    void printMonitoringProtocol() {
        if (!isEmpty()) {
            QueNode* Ptr = FirstPtr;
            QueNode* temp = nullptr;
            QueNode* PrevTemp = Prev;
            int time = 0;
            while (Ptr != nullptr) {
                int observTime;
                if (Ptr->spy->returnCount() == 0) {
                    observTime = Ptr->observationPeriod;
                }
                else if (Ptr->indexSpyObeserv > Ptr->spy->returnCount() - 1)
                    observTime = Ptr->observationPeriod - Ptr->spy->returnSumData();
                else {
                    observTime = Ptr->spy->getElemID(Ptr->indexSpyObeserv);
                    Ptr->indexSpyObeserv++;
                }
                int queTime = Ptr->maxQueTime;
                // Обработка случая, когда оставшееся время в очереди для шпиона истекло
                if (queTime <= time || Ptr->observationPeriod <= time){
                    std::cout << "Шпион " << Ptr->indexSpy << " покидает очередь и отправляется к резиденту." << std::endl;
                    Ptr = Ptr->Next;
                    continue;
                }
                // Наблюдение шпиона за объектом
                if ((time + observTime > Ptr->maxQueTime) && (Ptr->observationPeriod - time != 0)) {
                    std::cout << "Шпион " << Ptr->indexSpy << " наблюдает за объектом в течение " << Ptr->maxQueTime - time << " минут." << std::endl;
                    time += (Ptr->maxQueTime - time);
                }
                else if ((time + observTime > Ptr->observationPeriod) && (Ptr->observationPeriod - time != 0)){
                    std::cout << "Шпион " << Ptr->indexSpy << " наблюдает за объектом в течение " << Ptr->observationPeriod - time << " минут." << std::endl;
                    time += (Ptr->observationPeriod - time);
                }
                else if (observTime != 0) {
                    std::cout << "Шпион " << Ptr->indexSpy << " наблюдает за объектом в течение " << observTime << " минут." << std::endl;
                    time += observTime;
                }

                if (Ptr->spy->returnCount() > 1) {

                    QueNode* tempPtr = new QueNode;
                   
                    tempPtr->observationPeriod = Ptr->observationPeriod;
                    tempPtr->maxQueTime = Ptr->maxQueTime;
                    tempPtr->indexSpyObeserv = Ptr->indexSpyObeserv;
                    tempPtr->indexSpy = Ptr->indexSpy;
                    tempPtr->spy = Ptr->spy;
                    tempPtr->Next = nullptr;

                    Ptr->indexSpyObeserv = 0;

                    if (Prev != NULL)
                        Prev->Next = tempPtr;
                    Prev = tempPtr;

                    if (temp == nullptr) 
                        temp = tempPtr;
                }
                if ((queTime <= time || Ptr->observationPeriod <= time) && Ptr->indexSpyObeserv >= Ptr->spy->returnCount()) {
                    std::cout << "Шпион " << Ptr->indexSpy << " покидает очередь и отправляется к резиденту." << std::endl;
                    Ptr = Ptr->Next;
                    continue;
                }
                
                Ptr = Ptr->Next;
            }

            Prev = PrevTemp;
            if (Prev != nullptr) Prev->Next = nullptr;
            while (temp != nullptr) {
                QueNode* temp2 = temp;
                temp = temp->Next;
                delete temp2;
            }
        
        }
        else std::cout << "Перед выводом информации о шпионах, внесите их в очередь" << std::endl;
    }

    void printQue(){
        QueNode* Ptr = FirstPtr;
        int num = 1;
        if (!isEmpty()) {
            while (Ptr != nullptr) {
                std::cout << "Шпион " << num << " : maxQueTime - " << Ptr->maxQueTime << "min" << std::endl;
                std::cout << "          " << "observationPeriod - " << Ptr->observationPeriod << "min" << " : " << std::endl;
                if (!Ptr->spy->isEmpty())
                    for (int i = 1; i <= Ptr->spy->returnCount(); i++)
                        std::cout << "              " << "Время наблюдения " << i << " - " << Ptr->spy->getElemID(i - 1) << "min" << std::endl;
                else std::cout << "              " << "Время наблюдения 1 - " << Ptr->observationPeriod << "min" << std::endl;
                Ptr = Ptr->Next;
                std::cout << std::endl;
                num++;
            }
        }
        else std::cout << "Очередь пуста" << std::endl;
    }

    void isEmptyForUser() {
        if (isEmpty()) {
            std::cout << "Очередь пуста" << std::endl;
        }
        else std::cout << "Очередь не пуста" << std::endl;
    }

    bool isEmpty() {
        return FirstPtr == nullptr;
    }

    void printCountSpy() {
        std::cout << "Количество шпионов - " << countSpy << std::endl;
    }

    void dequeue() {
        while (FirstPtr != nullptr) {
            QueNode* temp = FirstPtr;
            FirstPtr = FirstPtr->Next;
            delete temp;
        }
        countSpy = 0;
        std::cout << "Очередь очищена" << std::endl;
    }

    void clear() {
        dequeue();
    }

    ~Queue() {
        clear();
    }

};

bool checkCorrectInput() {
    if (std::cin.fail()) {
        std::cout << "Ошибка ввода! Пожалуйста, введите корректное значение." << std::endl;
        std::cin.clear(); // Сброс флага ошибки ввода
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
        return false;
    }
    else
        return true;
}

int main() {
    setlocale(LC_ALL, "RU");

    /*std::cout << "Введите максимальный размер очереди: ";
    int maxSize;
    std::cin >> maxSize;*/
    
    Queue* que = new Queue;

    int count;
    while (true) {
        std::cout << "Введите максимальное кол-во наблюдений 1-го шпиона: ";
        std::cin >> count;
        if (!checkCorrectInput())
            continue;
        else if (count < 0)
            std::cout << "у нас здесь так то положительный ввод" << std::endl;
        else break;
        continue;
    }


    /*SpyObservation* spy = new SpyObservation(count);
    delete spy;
    spy->printCapacity();*/

    bool readComand = true;
    while (readComand) {
        std::cout << "1 - Помещение шпионов в очередь" << std::endl;
        std::cout << "2 - Информация о шпионах" << std::endl;
        std::cout << "3 - Проверка, является ли очередь пустой" << std::endl;
        std::cout << "4 - Вывод количества шпионов" << std::endl;
        std::cout << "5 - Удаление шпионов из очереди" << std::endl;
        std::cout << "6 - Вывод результатов наблюдения(пока в разработке)" << std::endl;
        std::cout << "7 - Выход" << std::endl;
        int num;
        std::cout << "Введите 1, 2, 3, 4, 5 или 7: ";
        std::cin >> num;

        if (!checkCorrectInput()) 
            continue; 

        if(num != 7) std::cout << "###############################" << std::endl;
        switch (num) {
        case 1: {
            que->enqueue(count);
            break;
        }
        case 2: {
            que->printQue();
            break;
        }
        case 3: {
            que->isEmptyForUser();
            break;
        }
        case 4: {
            que->printCountSpy();
            break;
        }
        case 5: {
            que->dequeue();
            break;
        }
        case 6: {
            que->printMonitoringProtocol();
            break;
        }
        case 7: {
            readComand = false;
            break;
        }
        default: std::cout << "Введена неккоректная команда, попробуйте ещё раз" << std::endl;
        }
        if(num != 7) std::cout << "###############################" << std::endl;
    }

    delete que;
    return 0;
}