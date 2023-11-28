// 24. В  листьях  И - ИЛИ  дерева, соответствующего некоторому
// множеству  конструкций, заданы   значения   массы. Известно
// максимально допустимое значение массы изделия. Требуется усечь
// дерево   так, чтобы   дерево    включало    все    элементы,
// соответствующие  допустимым  значениям массы, но не содержало
// "лишних" вершин. Конечное дерево выдать на экран в  наглядном
// виде(13).

// VS 2022  Афанасьев Никита Андреевич  ПС 23

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct Tree {
    std::string name;
    std::string str = "";
    int mas = -1;
    int minWeight = 0;
    int maxWeight = 0;
    int lvl = 0;
    int limitElement = -1;
    bool isDel = false;
    Tree* fath = nullptr;
    std::vector<Tree*> sons;
};

std::string printMasOrCh(Tree* node) {
    if (node->mas == -1)
        return node->str;
    else
        return std::to_string(node->mas);
}

void printTree(Tree* node, int level) {
    for (int i = 0; i < level; i++)
        std::cout << '.';

    std::cout << node->name << " ";
    std::string str = printMasOrCh(node);
    std::cout << str << std::endl;


    if (node->sons.size()) {
        for (int i = 0; i < node->sons.size(); i++) {
            printTree(node->sons[i], level + 1);
        }
    }
 
}

void calculateWeights(Tree* node) {
    if (node->sons.size()) {
        for (int i = 0; i < node->sons.size(); i++) {
            calculateWeights(node->sons[i]);
        }
    }
    if (node->fath != nullptr && node->fath->str == "or") {
        if (node->fath->minWeight == 0 && node->fath->maxWeight == 0) {
            if (node->str == "") {
                node->fath->minWeight = node->mas;
                node->fath->maxWeight = node->mas;
            }
            else {
                node->fath->minWeight = node->minWeight;
                node->fath->maxWeight = node->maxWeight;
            }
        }
        else
            if (node->str == "") {
                if (node->fath->minWeight > node->mas)
                    node->fath->minWeight = node->mas;
                if (node->fath->maxWeight < node->mas)
                    node->fath->maxWeight = node->mas;
            }
            else {
                if (node->fath->minWeight > node->minWeight)
                    node->fath->minWeight = node->minWeight;
                if (node->fath->maxWeight < node->maxWeight)
                    node->fath->maxWeight = node->maxWeight;
            }

    }
    else if (node->fath != nullptr && node->fath->str == "and") {
        if (node->str == "") {
            node->fath->minWeight += node->mas;
            node->fath->maxWeight += node->mas;
        }
        else {
            node->fath->minWeight += node->minWeight;
            node->fath->maxWeight += node->maxWeight;
        }
    }
}

void removeSonFromTree(Tree* parent, Tree* son) {
    if (parent->sons.size()) {
        // Найти нужный элемент в векторе
        auto it = std::find(parent->sons.begin(), parent->sons.end(), son);

        // Если элемент найден, удалить его
        if (it != parent->sons.end()) {
            parent->sons.erase(it);
        }
    }
}

void truncateTree(Tree* node) {
    if (node->sons.size()) {
        for (int k = 0; k < node->sons.size(); k++) {
            if (node->str == "or") {
                node->sons[k]->limitElement = node->limitElement;
            }
            if (node->str == "and") {
                node->sons[k]->limitElement = node->limitElement;
                for (int i = 0; i < node->sons.size(); i++) {
                    if (k != i) {
                        if (node->sons[i]->str == "")
                            node->sons[k]->limitElement -= node->sons[i]->mas;
                        else
                            node->sons[k]->limitElement -= node->sons[i]->minWeight;
                    }
                }
            }
            truncateTree(node->sons[k]);

            if (node->isDel == true) {
                --k;
                node->isDel = false;
            }
        }
    }
    if (node->str == "" && node->mas > node->limitElement) {
        if (node->fath != nullptr) {
            removeSonFromTree(node->fath, node);
            node->fath->isDel = true;
        }
        delete node;
    }
    else if (node->str == "or" && node->limitElement < node->minWeight) {
        if (node->fath != nullptr) {
            removeSonFromTree(node->fath, node);
            node->fath->isDel = true;
        }
        delete node;
    }
    else if (node->str == "and" && node->limitElement < node->minWeight) {
        if (node->fath != nullptr) {
            removeSonFromTree(node->fath, node);
            node->fath->isDel = true;
        }
        delete node;
    }
}


void deleteTree(Tree* node) {
    if (node != nullptr) {
        if (node->sons.size()) {
            for (int i = 0; i < node->sons.size(); i++) {
                deleteTree(node->sons[i]);
            }
        }
        delete node;
    }
}

void readFromFile(Tree*& node) {
    std::ifstream file("input.txt");

    if (!file) {
        std::cout << "Ошибка открытия файла " << "tree.txt" << std::endl;
        return;
    }
    else {
        std::cout << "Файл для чтения успешно открыт!" << std::endl;
    }

    Tree* prev = nullptr, * lastFath = nullptr;
    std::string inputLine;

    while (std::getline(file, inputLine)) {
        if (inputLine.empty())
            break;

        Tree* new_node = new Tree; // Создаем новый узел

        std::istringstream iss(inputLine);
        iss >> new_node->name >> new_node->str;
        if (new_node->name.empty() || new_node->str == " ") {
            std::cout << "Некорректные значение(-я) в файле";
            delete new_node;
            break;
        }

        int k = 0;
        while (new_node->name[0] == '.') {
            k++;
            new_node->name.erase(0, 1);
        }
        new_node->lvl = k;

        if (new_node->str != "or" && new_node->str != "and") {
            new_node->mas = std::stoi(new_node->str);
            new_node->str = "";
        }

        if (k == 0) {      // Инициализация
            node = new_node;
            node->fath = nullptr;
            prev = new_node;  // Сохраняем new_node в prev на первой итерации
            continue;
        }
        if (k == prev->lvl) {   // Тот же уровень
            new_node->fath = prev->fath;
            prev->fath->sons.push_back(new_node);
        }
        else if (k > prev->lvl) {   // Уровень выше
            new_node->fath = prev;
            prev->sons.push_back(new_node);
        }
        else {  // Уровень ниже
            lastFath = prev->fath;

            while (true) {
                if (k <= lastFath->lvl)
                    lastFath = lastFath->fath;
                else break;
            }
            new_node->fath = lastFath;
            lastFath->sons.push_back(new_node);
        }

        prev = new_node;

    }

    file.close();
}

void info() {
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Что делает эта программа ? Она позволяет тебе работать с деревом, где каждой вершине присвоено значение массы." << std::endl;
    std::cout << "Ты можешь задать максимально допустимое значение массы изделия и программа усечет дерево таким образом, чтобы" << std::endl;
    std::cout << "оно включало только те элементы, значения массы которых находятся в рамках допустимого значения." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Важные моменты при использовании программы :" << std::endl;
    std::cout << "Убедись, что у тебя есть файл 'tree.txt', в котором содержатся данные для построения дерева. Если файла нет, " << std::endl;
    std::cout << "создай новый файл и запиши в него данные в следующем формате :" << std::endl;
    std::cout << "   1. Каждая строка представляет собой описание узла дерева." << std::endl;
    std::cout << "   2. Узлы разделены по уровням с помощью символа точки(.)." << std::endl;
    std::cout << "   3. В начале каждой строки может быть несколько символов точки, они указывают на уровень узла." << std::endl;
    std::cout << "   4. После символа точки следует имя узла и его значение массы(если у узла есть значение массы)." << std::endl;
    std::cout << "   5. Если у узла нет значения массы, вместо него ставится символ  or  или  and , в зависимости от типа узла." << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Доступные команды:" << std::endl;
    std::cout << "1. Показать дерево" << std::endl;
    std::cout << "2. Усечь дерево" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "------------------" << std::endl;
}

int main() {
    setlocale(LC_ALL, "RU");

    Tree* root = nullptr;
    readFromFile(root);
    info();
    bool done = false;
    while (!done) {

        int choice;
        std::cout << "Введите (1/2/3): ";
        std::cin >> choice;

        if (choice != 3) std::cout << "-------------------------------" << std::endl;
        switch (choice) {
        case 1: {
            if (root != nullptr) {
                std::cout << "Исходное дерево:" << std::endl;
                printTree(root, 0);
            }
            else
                std::cout << "Дерево пустое!";
            break;
        }
        case 2: {
            if (root != nullptr) {
                calculateWeights(root);
                int maxMass;
                std::cout << "Введите максимальное значение массы от " << root->minWeight << " до " << root->maxWeight << " : ";
                std::cin >> maxMass;
                if (maxMass >= root->minWeight && maxMass <= root->maxWeight) {
                    root->limitElement = maxMass;
                    truncateTree(root);
                    std::cout << "\nУсеченное дерево (максимальная масса = " << maxMass << "):" << std::endl;
                    printTree(root, 0);
                }
                else std::cout << "Введена неккоректная команда!" << std::endl;
               
            }
            else
                std::cout << "Дерево пустое!";
            if (root != nullptr)
                deleteTree(root);
            done = true;
            break;
        }
        case 3: {
            if (root != nullptr)
                deleteTree(root);
            done = true;
            break;
        }
        default:
            std::cout << "Некорректный выбор. Попробуйте ещё раз." << std::endl;
            break;
        }
        if (choice != 3) std::cout << "-------------------------------" << std::endl;
    }

    return 0;
}