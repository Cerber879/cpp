#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <random>
#include <windows.h>

using namespace std;

const enum class Direction { Playing, Pause, GameOver, Win, Left, Up, Right, Down };
const enum class Difficulty { Easy, Normal, Hard, Impossible };

const char heads[4] = { '<', 'A', '>', 'V' };

struct Snake {
    Snake* next = nullptr;
    Snake* prev = nullptr;
    int x = -1;
    int y = -1;
};

class FieldSnake {
private:
    vector<vector<char>> field;
    vector<pair<int, int>> obstacles;
    Snake* head;
    Snake* tail;
    int width;
    int height;
    int x;
    int y;
    int fruitx;
    int fruity;
    int Score;
    int speed;
    int highScore;
    int obstacleCount = 5;
    Direction dir;
    Difficulty diff;
    bool useRandomObstacles = true;

public:
    FieldSnake() : width(20), height(10), field(height, vector<char>(width, ' ')), head(nullptr), tail(nullptr), 
        Score(2), speed(50), highScore(0), dir(Direction::Pause), diff(Difficulty::Normal) {}

    // Инициализация генератора случайных чисел
    void InitializeRandomGenerator() {
        srand(static_cast<unsigned int>(time(nullptr))); // Используем текущее время в качестве начального значения
    }

    // Функция инициализации игрового поля
    void InitializeField() {
        x = width / 2;
        y = height / 2;

        field.assign(height, vector<char>(width, ' '));

        head = new Snake;
        head->x = x;
        head->y = y;

        tail = new Snake;
        tail->x = x - 1;
        tail->y = y;

        head->next = tail;
        tail->prev = head;

        if (diff == Difficulty::Hard or diff == Difficulty::Impossible) {
            GenerateObstacles();
        }
        GenerateFruit();
    }

    // Функция генерации новых координат для фрукта
    void GenerateFruit() {
        do {
            fruitx = rand() % width;
            fruity = rand() % height;
        } while (IsSnakeOnFruit() or IsFruitOnObstacle());
    }

    // Функция проверки, находится ли фрукт на стенке
    bool IsFruitOnObstacle() {
        if (diff == Difficulty::Hard or diff == Difficulty::Impossible) {
            for (int i = 0; i < obstacles.size(); i++) {
                if (obstacles[i].first == fruitx and obstacles[i].second == fruity) {
                    return true;
                }
            }
        }
        return false;
    }

    // Функция проверки, находится ли змейка на фрукте
    bool IsSnakeOnFruit() {
        Snake* curr = head;
        while (curr != nullptr) {
            if (curr->x == fruitx and curr->y == fruity) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    } 

    // Функция для генерации преград
    void GenerateObstacles() {

        if (useRandomObstacles) {
            // Генерируем преграды случайным образом
            for (int i = 0; i < obstacleCount; i++) {
                int obsX, obsY;
                do {
                    obsX = rand() % width;
                    obsY = rand() % height;
                } while (IsSnakeOnObstacle(obsX, obsY));
                obstacles.push_back({ obsX, obsY });
            }
        }
        else {
            int gridSpacingX = width / sqrt(obstacleCount);
            int gridSpacingY = height / sqrt(obstacleCount);

            for (int y = 1; y < height; y += gridSpacingY) {
                for (int x = 1; x < width; x += gridSpacingX) {
                    if (!IsSnakeOnObstacle(x, y))
                        obstacles.push_back({ x, y });
                }
            }
        }
    }

    // Функция для проверки, находится ли змейка на преграде
    bool IsSnakeOnObstacle(int x, int y) {
        Snake* curr = head;
        while (curr != nullptr) {
            if (curr->x == x and curr->y == y) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    // Функция для проверки ячейки на преграду
    bool IsObstacle(int x, int y) {
        for (int i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].first == x and obstacles[i].second == y) {
                return true;
            }
        }
        return false;
    }

    // Функция обработки ввода команд
    void InputCmd() {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' and dir == Direction::Pause and x == width / 2 and y == height / 2) {
                // Скип
            }
            else if ((ch == 'w' && dir != Direction::Down) or
                (ch == 'a' && dir != Direction::Right) or
                (ch == 's' && dir != Direction::Up) or
                (ch == 'd' && dir != Direction::Left)) {
                switch (ch) {
                case 'w':
                    dir = Direction::Up;
                    break;
                case 'a':
                    dir = Direction::Left;
                    break;
                case 's':
                    dir = Direction::Down;
                    break;
                case 'd':
                    dir = Direction::Right;
                    break;
                }
            }
            else if (ch == ' ') {
                if (dir == Direction::Pause) {
                    if (head->next->x == head->x - 1 or head->next->x == width - 1) {
                        dir = Direction::Right;
                    }
                    else if (head->next->x == head->x + 1 or head->next->x == 0) {
                        dir = Direction::Left;
                    }
                    else if (head->next->y == head->y - 1 or head->next->y == height - 1) {
                        dir = Direction::Down;
                    }
                    else if (head->next->y == head->y - 1 or head->next->y == 0) {
                        dir = Direction::Up;
                    }
                }
                else
                    dir = Direction::Pause;
            }
            else if (ch == 'x') {
                dir = Direction::GameOver;
            }
        }
    }

    // Функция обновления направления движения змейки
    void CheckDirection() {
        switch (dir) {
        case Direction::Up:
            y--;
            break;
        case Direction::Left:
            x--;
            break;
        case Direction::Down:
            y++;
            break;
        case Direction::Right:
            x++;
            break;
        }
    }

    // Функция проверки выхода за поле
    void CheckSubscriptOfRangeField() {
        if (diff != Difficulty::Hard and diff != Difficulty::Impossible) {
            if (x >= width)
                x = 0;
            else if (x < 0)
                x = width - 1;
            else if (y >= height)
                y = 0;
            else if (y < 0)
                y = height - 1;
        }
        else if (diff == Difficulty::Hard){
            if (x >= width or x < 0 or y >= height or y < 0)
                dir = Direction::GameOver;
            for (int i = 0; i < obstacles.size(); i++) {
                if (obstacles[i].second == y and obstacles[i].first == x)
                    dir = Direction::GameOver;
            }
        }
        else {
            if ((x == width or IsObstacle(x, y)) and head->next->x == head->x - 1) {
                if (Score == 2)
                    dir = Direction::GameOver;
                else {
                    if (!IsObstacle(x - 1, y - 1) and y != 0) {
                        x--;
                        y--;
                        dir = Direction::Up;
                    }
                    else if (!IsObstacle(x - 1, y + 1) and y != height - 1) {
                        x--;
                        y++;
                        dir = Direction::Down;
                    }
                    else
                        dir = Direction::GameOver;

                    DelTail();
                }
                
            }
            else if ((x == -1 or IsObstacle(x, y)) and head->next->x == head->x + 1) {
                if (Score == 2)
                    dir = Direction::GameOver;
                else {
                    if (!IsObstacle(x + 1, y + 1) and y != height - 1) {
                        x++;
                        y++;
                        dir = Direction::Down;
                    }
                    else if (!IsObstacle(x + 1, y - 1) and y != 0) {
                        x++;
                        y--;
                        dir = Direction::Up;
                    }
                    else
                        dir = Direction::GameOver;

                    DelTail();
                }
                
            }
            else if ((y == height or IsObstacle(x, y)) and head->next->y == head->y - 1) {
                if (Score == 2)
                    dir = Direction::GameOver;
                else {
                    if (!IsObstacle(x + 1, y - 1) and x != width - 1) {
                        y--;
                        x++;
                        dir = Direction::Right;
                    }
                    else if (!IsObstacle(x - 1, y - 1) and x != 0) {
                        y--;
                        x--;
                        dir = Direction::Left;
                    }
                    else
                        dir = Direction::GameOver;

                    DelTail();
                }
                
            }
            else if ((y == -1 or IsObstacle(x, y)) and head->next->y == head->y + 1) {
                if (Score == 2)
                    dir = Direction::GameOver;
                else {
                    if (!IsObstacle(x - 1, y + 1) and x != 0) {
                        y++;
                        x--;
                        dir = Direction::Left;
                    }
                    else if (!IsObstacle(x + 1, y + 1) and x != width - 1) {
                        y++;
                        x++;
                        dir = Direction::Right;
                    }
                    else
                        dir = Direction::GameOver;

                    DelTail();
                }
                
            }
        }
    }

    void DelTail() {
        Snake* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
        Score--;
    }

    // Функция обновления позиции змейки
    void UpdateSnakePosition() {
        if (dir != Direction::Pause) {
            Snake* temp = head;
            CheckDirection();
            CheckSubscriptOfRangeField();
            int prevx = temp->x;
            int prevy = temp->y;
            temp->x = x;
            temp->y = y;
            temp = temp->next;
            int prev2x, prev2y;
            while (temp != nullptr) {
                prev2x = temp->x;
                prev2y = temp->y;
                temp->x = prevx;
                temp->y = prevy;
                prevx = prev2x;
                prevy = prev2y;
                temp = temp->next;
            }
        }
    }

    // Функция обработки игровой логики
    void ProcessGameLogic() {
        UpdateSnakePosition();

        if (dir != Direction::Pause and diff != Difficulty::Easy) {
            Snake* curr = head->next;
            while (curr != nullptr) {
                if (curr->x == head->x && curr->y == head->y) {
                    dir = Direction::GameOver;
                    return;
                }
                curr = curr->next;
            }
        }

        // Проверка, находится ли голова на фрукте
        if (head->x == fruitx and head->y == fruity) {
            GenerateFruit();
            Snake* newTail = new Snake;
            newTail->x = tail->x;
            newTail->y = tail->y;
            tail->next = newTail;
            newTail->prev = tail;
            tail = newTail;
            Score++;
        }

        if (Score == width * height - 396) {
            dir = Direction::GameOver;
        }
        else if (Score > highScore) {
            highScore = Score;
        }
    }

    // Функция отрисовки игрового поля
    void DrawField() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                field[i][j] = ' ';
            }
        }

        if (diff == Difficulty::Hard or diff == Difficulty::Impossible) {
            for (int i = 0; i < obstacles.size(); i++) {
                field[obstacles[i].second][obstacles[i].first] = '#';
            }
        }
        
        char ch = '0';
        int index = static_cast<int>(dir);
        if (index == 7 or index == 4 or index == 5 or index == 6)
            ch = heads[index - 4];


        field[head->y][head->x] = ch;
        field[fruity][fruitx] = '$';
        Snake* curr = head->next;
        while (curr != nullptr) {
            field[curr->y][curr->x] = 'o';
            curr = curr->next;
        }

        /*system("cls");*/
        // Получение дескриптора стандартного вывода
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Структура для хранения координат курсора
        COORD position;
        position.X = 0; // Установка координаты по оси X в 0
        position.Y = 0; // Установка координаты по оси Y в 0

        // Перемещение курсора в указанную позицию
        SetConsoleCursorPosition(hConsole, position);
        for (int i = 0; i < width + 2; i++)
            cout << '#';
        cout << endl;

        for (int i = 0; i < height; i++) {
            cout << '#';
            for (int j = 0; j < width; j++) {
                cout << field[i][j];
            }
            cout << '#' << endl;
        }

        for (int i = 0; i < width + 2; i++)
            cout << '#';
        cout << endl;
    }

    // Функция для отслеживания состояния игры
    bool GameOver() {
        return dir == Direction::GameOver;
    }

    // Функция удаления змейки и сброса игры
    void ResetGame() {
        while (head != nullptr) {
            Snake* temp = head;
            head = head->next;
            delete temp;
        }
        obstacles.clear();
        head = nullptr;
        tail = nullptr;
        dir = Direction::Pause;
        x = width / 2;
        y = height / 2;
        Score = 2;
    }

    // Функция для сброса настроек к изначальным значениям
    void ResetSettings() {
        width = 20;
        height = 20;
        diff = Difficulty::Normal;
        speed = 50;
        highScore = 0;
        obstacleCount = 5;
        useRandomObstacles = true;
        SaveGameParameters();
    }

    // Функция отображения статуса игры
    void DisplayGameStatus() {
        cout << "Score: " << Score - 2 << "  ";
        string statusText;
        if (dir == Direction::Win) {
            statusText = "You win";
        }
        if (dir == Direction::GameOver) {
            statusText = "Game Over";
        }
        else if (dir == Direction::Pause) {
            statusText = "Paused";
        }
        else {
            statusText = "Playing";
        }
        cout << "Status: " << statusText << endl;
        cout << "High Score: " << highScore - 2 << "  ";
    }

    // Функция отображения информации для пользователя
    void DisplayInfo() {
        system("cls");
        do {
            cout << "Snake Game - Инструкция" << endl;
            cout << "1. Управление змейкой: W (вверх), A (влево), S (вниз), D (вправо)." << endl;
            cout << "2. Пауза/Возобновление: Пробел." << endl;
            cout << "3. Соберите фрукты ('$') для увеличения счета." << endl;
            cout << "4. Избегайте столкновения с границами поля и самим собой на сложном уровне игры." << endl;
            cout << "5. Цель игры - получить максимальный счет." << endl;
            cout << "6. Нажмите любую клавишу для возврата в главное меню." << endl;
        } while (!_getch()); // Ждем нажатия клавиши пользователя
    }

    // Функция отображения главного меню игры
    void DisplayMainMenu() {
        system("cls");
        cout << "Snake Game" << endl;
        cout << "1. Start" << endl;
        cout << "2. Settings" << endl;
        cout << "3. Info" << endl;
        cout << "4. Exit" << endl;
    }

    // Функция настройки параметров игры
    void ConfigureGameSettings() {
        char option;
        do {
            system("cls");
            cout << "Settings" << endl;
            cout << "1. Set field width (current: " << width << ")" << endl;
            cout << "2. Set field height (current: " << height << ")" << endl;
            cout << "3. Set difficulty (current: ";
            if (diff == Difficulty::Easy)
                cout << "Easy";
            else if (diff == Difficulty::Normal)
                cout << "Normal";
            else if (diff == Difficulty::Hard)
                cout << "Hard";
            else
                cout << "Impossible";
            cout << ")" << endl;
            cout << "4. Set speed snake (current : " << speed << ")" << endl;
            cout << "5. Set obstacle count (current : " << obstacleCount << ")" << endl;
            cout << "6. Use random obstacles (current : ";
            if (useRandomObstacles)
                cout << "yes";
            else
                cout << "no";
            cout << ")" << endl;
            cout << "7. Reset settings" << endl;
            cout << "8. Back to Main Menu" << endl;

            option = _getch();

            if (option == '1') {
                cout << "Enter field width: ";
                cin >> width;
                x = width / 2;
            }
            else if (option == '2') {
                cout << "Enter field height: ";
                cin >> height;
                y = height / 2;
            }
            else if (option == '3') {
                cout << "Select difficulty:" << endl;
                cout << "1. Easy" << endl;
                cout << "2. Normal" << endl;
                cout << "3. Hard" << endl;
                cout << "4. Impossible" << endl;

                option = _getch();

                if (option == '1') {
                    diff = Difficulty::Easy;
                }
                else if (option == '2') {
                    diff = Difficulty::Normal;
                }
                else if (option == '3') {
                    diff = Difficulty::Hard;
                }
                else if (option == '4') {
                    diff = Difficulty::Impossible;
                }
                else if (option == 27) {
                    // Возвращаемя обратно
                }
            }
            else if (option == '4') {
                cout << "Enter speed snake: ";
                cin >> speed;
            }
            else if (option == '5') {
                cout << "Enter obstacle count: ";
                cin >> obstacleCount;
            }
            else if (option == '6') {
                if (useRandomObstacles)
                    useRandomObstacles = false;
                else
                    useRandomObstacles = true;
            }
            else if (option == '7') {
                ResetSettings();
            }
            else if (option == 27) {
                option = '8';
            }
        } while (option != '8' and option != ' ');
    }

    // Функция сохранения параметров игры в файл
    void SaveGameParameters() {
        ofstream outFile("game_data.txt", ios_base::trunc);
        if (outFile.is_open()) {
            outFile << "Width:" << width << "\n";
            outFile << "Height:" << height << "\n";
            outFile << "Difficulty:" << static_cast<int>(diff) << "\n";
            outFile << "Speed:" << speed << "\n";
            outFile << "HighScore:" << highScore << "\n";
            outFile << "ObstacleCount:" << obstacleCount << "\n";
            if (useRandomObstacles)
                outFile << "UseRandomObstacles:yes";
            else
                outFile << "UseRandomObstacles:no";
        }
        else {
            cout << "Failed to save game parameters!" << endl;
        }
        outFile.close();
    }

    // Функция загрузки параметров игры из файла
    void LoadGameParameters() {
        ifstream inFile("game_data.txt");
        string line;
        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                if (line.find("Width:") != string::npos) {
                    width = stoi(line.substr(6));
                    x = width / 2;
                }
                else if (line.find("Height:") != string::npos) {
                    height = stoi(line.substr(7));
                    y = height / 2;
                }
                else if (line.find("Difficulty:") != string::npos) {
                    diff = static_cast<Difficulty>(stoi(line.substr(11)));
                }
                else if (line.find("Speed:") != string::npos) {
                    speed = stoi(line.substr(6));
                }
                else if (line.find("HighScore:") != string::npos) {
                    highScore = stoi(line.substr(10));
                }
                else if (line.find("HighScore:") != string::npos) {
                    highScore = stoi(line.substr(10));
                }
                else if (line.find("ObstacleCount:") != string::npos) {
                    obstacleCount = stoi(line.substr(14));
                }
                else if (line.find("UseRandomObstacles:") != string::npos) {
                    if (line.substr(19) == "yes")
                        useRandomObstacles = true;
                    else 
                        useRandomObstacles = false;
                }
            }
            inFile.close();
        }
        else {
            cout << "Unable to load game parameters. Using default values." << endl;
        }
    }

    // Основная функция запуска игры
    void RunGame() {
        char option;
        do {
            DisplayMainMenu();
            option = _getch();
            switch (option) {
            case '1': {
                InitializeRandomGenerator();
                InitializeField();
                while (!GameOver()) {
                    DrawField();
                    InputCmd();
                    ProcessGameLogic();
                    DisplayGameStatus();
                    Sleep(speed);
                }
                ResetGame();
                cout << "\nPress any key to return to the main menu...";
                _getch();
                break;
            }
            case '2':
                ConfigureGameSettings();
                break;
            case '3':
                DisplayInfo();
                break;
            case 27:
                option = '4';
                break;
            default:
                break;
            }
        } while (option != '4' and option != ' ' and option != 'x');

    }
};

int main() {
    setlocale(LC_ALL, "RU");

    FieldSnake fs;
    fs.LoadGameParameters(); // Загрузка параметров при запуске программы

    fs.RunGame();

    fs.SaveGameParameters(); // Сохранение параметров при завершении программы

    return 0;
}