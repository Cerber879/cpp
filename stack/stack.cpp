// 16. У множества шпионов, собравшихся вместе для наблюдения
// секретного     объекта, имеется    единственный    бинокль.
// Сформировалась очередь на этот  бинокль. Для  каждого  шпиона
// задан   период   наблюдения   в  минутах  и  предельное  время
// нахождения в очереди. После наблюдения каждый шпион становится
// снова  в  конец  очереди. Как  только  для какого - либо шпиона
// истекает предельное время нахождения в очереди, он покидает ее
// (даже  если  в  этот момент владеет биноклем) и отправляется к
// резиденту.Вывести протокол наблюдения шпионов за объектом(9).

// VS 2022  Афанасьев Никита Андреевич  ПС 23

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class QueNode {
public:
    QueNode* Next = nullptr;
    QueNode* NextQueTime = nullptr;
    int maxQueTime = -1;
    int observationPeriod = -1;
    bool isInQue = true;
    std::string name;
};

class Queue {
private:
    QueNode* FirstPtr;
    QueNode* Prev;
    QueNode* FirstByTime;
    QueNode* PrevByTime;
    int countSpy;
public:

    Queue() : FirstPtr(nullptr), Prev(nullptr), FirstByTime(nullptr), PrevByTime(nullptr), countSpy(0) {}

    void enqueue() {
        std::ifstream file("spions.txt");

        if (!file) {
            std::cout << "Ошибка открытия файла " << "spions.txt" << std::endl;
            return;
        }
        else std::cout << "Файл для чтения успешно открыт!" << std::endl;

        QueNode* NewPtr;
        std::cin.get();
        std::string inputLine;
        while (std::getline(file, inputLine)) {
            if (inputLine.empty()) 
                break;
            NewPtr = new QueNode;
            std::istringstream iss(inputLine);
            iss >> NewPtr->name >> NewPtr->maxQueTime >> NewPtr->observationPeriod;
            if (NewPtr->observationPeriod <= 0 || NewPtr->maxQueTime <= 0) {
                std::cout << "Неккоректные значение(-я) в строчке " << countSpy + 1 << std::endl;
                std::cout << "Исправьте ошибки в файле, не все шпионы могли быть добавлены!!!" << std::endl;
                delete NewPtr;
                break;
            }

            NewPtr->Next = nullptr;
            if (FirstPtr == nullptr) {
                FirstPtr = NewPtr;
            }
            else {
                Prev->Next = NewPtr;
            }
            Prev = NewPtr;


            if (FirstByTime == nullptr || NewPtr->maxQueTime < FirstByTime->maxQueTime) {
                NewPtr->NextQueTime = FirstByTime;
                if (FirstByTime != nullptr) {
                    PrevByTime = NewPtr;
                }
                FirstByTime = NewPtr;
            }
            else {
                QueNode* current = FirstByTime;
                while (current->NextQueTime != nullptr && current->NextQueTime->maxQueTime <= NewPtr->maxQueTime) {
                    current = current->NextQueTime;
                }
                NewPtr->NextQueTime = current->NextQueTime;
                if (current->NextQueTime != nullptr) {
                    PrevByTime = NewPtr;
                }
                current->NextQueTime = NewPtr;
                PrevByTime = current;
            }
            
            countSpy++;
        }
        if (FirstPtr != nullptr) std::cout << "Очередь успешно добавлена!" << std::endl;
    }

    void printMonitoringProtocol(std::ostream& outf) {
        if (!isEmpty()) {
            QueNode* Ptr = FirstPtr;
            QueNode* temp = nullptr;
            QueNode* checkTemp = FirstByTime;
            QueNode* PrevTemp = nullptr;
            int time = 0;
            int endSpyes = 0;
            while (Ptr != nullptr) {
                int observTime = Ptr->observationPeriod;
                int queTime = Ptr->maxQueTime;

                if (queTime <= time) {
                    Ptr = Ptr->Next;
                    endSpyes++;
                    continue;
                }
                // Наблюдение шпиона за объектом
                if (time + observTime > queTime) {
                    outf << time << " " << Ptr->name << " взял бинокль " << std::endl;
                    time = queTime;
                    checkEndSpyes(Ptr, checkTemp, &endSpyes, time, outf);
                    outf << time << " " << Ptr->name << " сдал бинокль  " << std::endl;
                }
                else if (checkForTime(time + Ptr->observationPeriod) == 1) { 
                    outf << time << " " << Ptr->name << " взял бинокль " << std::endl;
                    time = queTime;
                    checkEndSpyes(Ptr, checkTemp, &endSpyes, time, outf);
                    outf << time << " " << Ptr->name << " сдал бинокль  " << std::endl;
                    outf << time << " " << Ptr->name << " покидает очередь и отправляется к резиденту." << std::endl;
                }
                else {
                    outf << time << " " << Ptr->name << " взял бинокль " << std::endl;
                    time += observTime;
                    checkEndSpyes(Ptr, checkTemp, &endSpyes, time, outf);
                    outf << time << " " << Ptr->name << " сдал бинокль " << std::endl;
                }

                if (time < queTime) {

                    QueNode* tempPtr = new QueNode;

                    tempPtr->observationPeriod = Ptr->observationPeriod;
                    tempPtr->maxQueTime = Ptr->maxQueTime;
                    tempPtr->name = Ptr->name;
                    tempPtr->Next = nullptr;

                    if (Prev != NULL && (time - queTime == 0 || time - observTime == 0))
                        Prev->Next = tempPtr;
                    else if (PrevTemp != NULL)
                        PrevTemp->Next = tempPtr;
                    PrevTemp = tempPtr;

                    if (temp == nullptr)
                        temp = tempPtr;
                }

                Ptr = Ptr->Next;
            }
            if (Prev != nullptr) 
                Prev->Next = nullptr;

            std::cout << "Итоги наблюдения сформированны!" << std::endl;

            while (temp != nullptr) {
                QueNode* temp2 = temp;
                temp = temp->Next;
                delete temp2;
            }
        }
        else std::cout << "Перед выводом информации о шпионах, внесите их в очередь" << std::endl;
    }

    int checkForTime(int time) {
        QueNode* Ptr = FirstPtr;
        int cnt = 0;
        int k = 0;
        if (!isEmpty()) {
            while (k != countSpy) {
                if (Ptr->maxQueTime > time)
                    cnt += 1;
                Ptr = Ptr->Next;
                k++;
            }
        }
        return cnt;
    }

    void checkEndSpyes(QueNode* Ptr, QueNode* checkTemp, int* endSpyes, int time, std::ostream& outf) {
        while (checkTemp != PrevByTime->NextQueTime && checkTemp != nullptr) {
            if (checkTemp->maxQueTime <= time && checkTemp->isInQue && checkTemp->maxQueTime != Ptr->maxQueTime) {
                outf << checkTemp->maxQueTime << " " << checkTemp->name << " покидает очередь и отправляется к резиденту." << std::endl;
                endSpyes++;
                checkTemp->isInQue = false;
            }
            checkTemp = checkTemp->NextQueTime;
        }
        checkTemp = FirstByTime;
    }

    void printQue() {
        QueNode* Ptr = FirstPtr;
        if (!isEmpty()) {
            while (Ptr != nullptr) {
                std::cout << "Шпион " << Ptr->name << " : maxQueTime - " << Ptr->maxQueTime << "min" << std::endl;
                std::cout << "          " << "observationPeriod - " << Ptr->observationPeriod << "min" << std::endl;  
                Ptr = Ptr->Next;
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

    void printProgramDescription() {
        std::cout << "Программа предназначена для моделирования очереди шпионов, желающих наблюдать за секретным объектом, " << std::endl;
        std::cout << "имеющим единственный бинокль. Для каждого шпиона задается период наблюдения в минутах и  предельное " << std::endl;
        std::cout << "время нахождения в очереди. После  наблюдения  каждый  шпион  становится  снова  в  конец  очереди." << std::endl;
        std::cout << "Как только для какого - либо шпиона истекает предельное время нахождения в очереди,  он покидает ее" << std::endl;
        std::cout << "(даже если в этот момент владеет биноклем) и отправляется к резиденту." << std::endl << std::endl;;
        std::cout << "Для  использования  программы   необходимо  следить  за  корректностью   ввода   данных  и  выбирать" << std::endl;
        std::cout << "соответствующие команды из меню. Для внесения информации о шпионах в очередь необходимо создать файл" << std::endl;
        std::cout << "\"spions.txt\" в корневой папке программы и заполнить его данными о шпионах в формате:" << std::endl;
        std::cout << "----------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "имя_шпиона период_наблюдения_в_минутах предельное_время_нахождения_в_очереди_в_минутах\"." << std::endl;
        std::cout << "----------------------------------------------------------------------------------------------------" << std::endl << std::endl;
        std::cout << "Доступные команды:" << std::endl;
        std::cout << "1 - Помещение шпионов в очередь" << std::endl;
        std::cout << "2 - Информация о шпионах" << std::endl;
        std::cout << "3 - Проверка, является ли очередь пустой" << std::endl;
        std::cout << "4 - Вывод количества шпионов" << std::endl;
        std::cout << "5 - Удаление шпионов из очереди" << std::endl;
        std::cout << "6 - Вывод результатов наблюдения" << std::endl;
        std::cout << "7 - Выход" << std::endl;
    }

    void dequeue() {
        while (FirstPtr != nullptr) {
            QueNode* temp = FirstPtr;
            FirstPtr = FirstPtr->Next;
            delete temp;
        }
        FirstByTime = nullptr;
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
    const std::string logFile = "log.txt";
    std::ofstream outf(logFile);
    Queue* que = new Queue;
    bool readComand = true;
    std::cout << "0 - ИНФОРМАЦИЯ О ПРОГРАММЕ" << std::endl;
    while (readComand) {
        int num;
        std::cout << "Введите (1/2/3/4/5/6/7): ";
        std::cin >> num;

        if (!checkCorrectInput())
            continue;
                                                                   
        if (num != 7) std::cout << "-------------------------------" << std::endl;
        switch (num) {
        case 0: {
            que->printProgramDescription();
            break;
        }
        case 1: {
            que->enqueue();
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
            que->printMonitoringProtocol(outf);
            break;
        }
        case 7: {
            readComand = false;
            break;
        }
        default: std::cout << "Введена неккоректная команда, попробуйте ещё раз" << std::endl;
        }
        if (num != 7) std::cout << "-------------------------------" << std::endl;
    }

    delete que;
    return 0;
}