// 21. Имеется  сеть  автомобильных  дорог. Для каждой дороги
// известна максимальная масса груза, которую можно  провезти  по
// этой   дороге. С   помощью  алгоритма  Дейкстры   определить
// максимальный   груз, который  можно  провезти  между   двумя
// указанными городами (10).

// VS 2022  Афанасьев Никита Андреевич  ПС 23

#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <Windows.h>

class Graph {
private:
    typedef std::pair<int, int> pair;
    std::vector<std::vector<pair>> graph;
    std::vector<std::string> cities;
public:
    // Метод для вывода графа
    void printGraph() {
        for (int i = 0; i < graph.size(); i++) {
            for (int j = 0; j < graph[i].size(); j++) {
                int startVertex = i + 1;
                int endVertex = graph[i][j].first + 1;
                int weight = graph[i][j].second;

                // Проверяем порядок вершин и выводим ребро
                if (startVertex < endVertex) {
                    std::cout << cities[startVertex - 1] << " " << cities[endVertex - 1] << " " << weight << std::endl;
                }
            }
        }
    }

    // Метод для чтения названий городов из файла
    void readCities() {
        if (!graph.empty()) {
            std::ifstream file("cities.txt");
            if (!file) {
                std::cout << "Ошибка открытия файла cities.txt" << std::endl;
                return;
            }
            else {
                std::cout << "Файл с городами для чтения успешно открыт!" << std::endl;
            }

            std::string inputLine;
            while (std::getline(file, inputLine)) {
                if (inputLine.empty())
                    break;

                std::istringstream iss(inputLine);
                std::string str;
                std::string name = "";
                iss >> str;
                while (iss >> str) {
                    name += str;
                }
                cities.push_back(name);
            }
        }
        else
            std::cout << "Граф пустой!" << std::endl;
    }

    // Метод поиска ID города
    int searchCityID(std::string& city) {
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i] == city) {
                return i + 1;
                break;
            }
        }
        std::cout << "Ошибка: Не удалось найти номер города " << city << std::endl;
        return 0;

    }

    // Метод для чтения графа из файла
    void readGraph() {
        std::ifstream file("graph.txt");
        if (!file) {
            std::cout << "Ошибка открытия файла graph.txt" << std::endl;
            return;
        }
        else {
            std::cout << "Файл с графом для чтения успешно открыт!" << std::endl;
        }

        int len;
        file >> len;
        graph.resize(len); // Инициализируем вектор графа
        file.get();

        std::string inputLine;
        while (std::getline(file, inputLine)) {
            if (inputLine.empty())
                break;

            std::istringstream iss(inputLine);
            int start, end, mas;
            iss >> start >> end >> mas;

            if (start <= 0 || end <= 0 || mas <= 0) {
                std::cout << "Некорректные значения в файле" << std::endl;
                break;
            }

            graph[start - 1].push_back({ end - 1, mas });
            graph[end - 1].push_back({ start - 1, mas });
        }

    }

    bool empty() {
        return graph.empty();
    }

    int size() const {
        return graph.size();
    }

    // Метод для поиска максимального пути с помощью алгоритма Дейкстры
    void Dijkstra(int start, int end) {

        std::vector<int> maxWeight(graph.size(), INT_MIN);
        std::vector<int> prevVertex(graph.size(), -1);
        maxWeight[start - 1] = INT_MAX;

        std::priority_queue<pair> prQue;
        prQue.push({ maxWeight[start - 1], start - 1 });

        while (!prQue.empty()) {
            int weightCell = prQue.top().first;
            int vertexStart = prQue.top().second;
            prQue.pop();

            for (int i = 0; i < graph[vertexStart].size(); i++) {
                int vertexEnd = graph[vertexStart][i].first;
                int weightRoad = graph[vertexStart][i].second;

                int minWeight = min(weightCell, weightRoad);

                if (minWeight > maxWeight[vertexEnd]) {
                    maxWeight[vertexEnd] = minWeight;
                    prevVertex[vertexEnd] = vertexStart;
                    prQue.push({ maxWeight[vertexEnd], vertexEnd });
                }

            }
        }

        if (maxWeight[end - 1] == INT_MIN)
            std::cout << "Пути между городами не существует!" << std::endl;
        else {
            std::cout << "Максимальный груз, который можно провезти между городами ";
            std::cout << cities[start - 1] << " и " << cities[end - 1] << " равен " << maxWeight[end - 1] << std::endl;

            // Восстановление пути
            std::vector<int> path;
            int currentVertex = end - 1;
            while (currentVertex != start - 1) {
                path.push_back(currentVertex);
                currentVertex = prevVertex[currentVertex];
            }
            path.push_back(start - 1);

            std::cout << "Путь от " << cities[start - 1] << " до " << cities[end - 1] << ": ";
            for (int i = path.size() - 1; i >= 0; i--) {
                std::cout << cities[path[i]];
                if (i != 0)
                    std::cout << " -> ";
            }
            std::cout << std::endl;
        }
    }

    // Метод для очистки графа
    void clear() {
        graph.clear();
    }
};

void info() { 
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Что делает эта программа? Она позволяет тебе определить максимальный груз, который можно провезти между двумя" << std::endl;
    std::cout << "указанными городами." << std::endl;
    std::cout << "Ты можешь добавлять вершины, соединять их ребрами, а также выполнять различные операции с графом." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Важные моменты при использовании программы:" << std::endl;
    std::cout << "Убедись, что у тебя есть файл 'graph.txt', в котором содержится информация о графе. Если файла нет," << std::endl;
    std::cout << "создай новый файл и запиши в него данные в следующем формате:" << std::endl;
    std::cout << "   1. Первая строка содержит количество городов" << std::endl;
    std::cout << "   2. Каждая последующая строка представляет собой описание путей между городами." << std::endl;
    std::cout << "   3. Пути указываются в формате 'город_1 город_2 допустимая_масса_груза_на_пути_между_ними '." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Доступные команды:" << std::endl;
    std::cout << "1. Поиск максимальной массы" << std::endl;
    std::cout << "2. Структура путей между городами" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "------------------" << std::endl;
}

int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Graph graph;
    graph.readGraph();
    graph.readCities();
    info();

    bool done = false;
    while (!done) {
        int choice;
        std::cout << "Введите (1/2/3): ";
        std::cin >> choice;

        if (choice != 3)
            std::cout << "-------------------------------" << std::endl;

        switch (choice) {
        case 1: {
            std::cout << "Введите начальный и конечный города (ч/з пробел): ";
            std::cin.ignore();
            std::string inputLine;
            std::getline(std::cin, inputLine);
            std::istringstream iss(inputLine);

            std::string start, end;
            int startID, endID;
            if (!(iss >> start >> end)) {
                std::cout << "Введены некорректные значения!" << std::endl;
                break;
            }
            if (std::isdigit(start[0]) && std::isdigit(end[0])) {
                startID = std::stoi(start);
                endID = std::stoi(end);
            }
            else {
                startID = graph.searchCityID(start);
                endID = graph.searchCityID(end);
                if (startID == 0 || endID == 0)
                    break;
            }
            if (endID > graph.size() || startID > graph.size()) {
                std::cout << "Выход за пределы количества городов" << std::endl;
                break;
            }
            else if (startID < 0 || endID < 0) {
                std::cout << "Город не может быть отрицательным значением" << std::endl;
                break;
            }
            else if (startID == endID) {
                std::cout << "Максимальный груз не ограничен!" << std::endl;
                break;
            }

            graph.Dijkstra(startID, endID);
            break;
        }
        case 2: {
            graph.printGraph();
            break;
        }
        case 3: {
            graph.clear();
            done = true;
            break;
        }
        default:
            std::cout << "Некорректный выбор. Попробуйте ещё раз." << std::endl;
            break;
        }

        if (choice != 3)
            std::cout << "-------------------------------" << std::endl;
    }

    return 0;
}