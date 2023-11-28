#include <iostream>
#include <set>
#include <cctype>
#include <sstream>
#include <stack>
#include <queue>

int main() {
	setlocale(LC_ALL, "RU");
	bool done = false;
	while (!done) {
		int choice;
		std::cout << "Введите (1/2/3/4): ";
		std::cin >> choice;

		if (choice != 4) std::cout << "-------------------------------" << std::endl;
		switch (choice) {
		case 1: {
			std::cin.get();
			std::set<int> mySet;
			std::string inputLine;
			while (true) {
				std::getline(std::cin, inputLine);
				if (inputLine.empty())
					break;
				std::string str = "";
				std::string value = "";
				std::istringstream iss(inputLine);
				iss >> str >> value;
				if (str == "push") {
					try {
						int i = std::stoi(value);
						mySet.insert(i);
					}
					catch (const std::exception& e) {
						std::cout << "Введены неккоректные значения!" << std::endl;
					}
				}
				else if (str == "delete" && std::stoi(value)) {
					try {
						int i = std::stoi(value);
						mySet.erase(i);
					}
					catch (const std::exception& e) {
						std::cout << "Введены неккоректные значения!" << std::endl;
					}
				}
				else
					std::cout << "Введены неккоректные значения!" << std::endl;

			}

			copy(mySet.begin(), mySet.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			break;
		}
		case 2: {
			std::cin.get();
			// Объявление стека
			std::stack<int> myStack;

			// Добавление элементов в стек
			std::cout << "Введите элементы стека: " << std::endl;
			std::string str;
			while (true) {
				std::getline(std::cin, str);
				if (str.empty())
					break;
				try {
					int i = std::stoi(str);
					myStack.push(i); // Добавляем элемент в конец списка
				}
				catch (const std::exception& e) {
					std::cout << "Введены некорректные значения!\n";
				}
			}

			// Вывод верхнего элемента
			std::cout << "Верхний элемент: " << myStack.top() << std::endl;

			// Удаление верхнего элемента 
			myStack.pop();
			std::cout << "Верхний элемент удален " << std::endl;

			// Вывод нового верхнего элемента
			std::cout << "Новый верхний элемент: " << myStack.top() << std::endl;

			std::cout << "Исходный стек: ";
			while (!myStack.empty()) {
				std::cout << myStack.top() << " ";
				myStack.pop();
			}
			std::cout << std::endl;
			break;
		}
		case 3: {

			// Создание очереди
			std::queue<int> myQueue;

			// Ввод 7 чисел пользователем и добавление их в очередь
			std::cout << "Введите 7 чисел:\n";
			for (int i = 0; i < 7; ++i) {
				int number;
				std::cin >> number;
				myQueue.push(number);
			}

			// Вывод первого числа в очереди
			std::cout << "Первое число в очереди: " << myQueue.front() << std::endl;

			// Удаление одного числа из очереди
			myQueue.pop();
			std::cout << "Первое число в очереди удалено ";
			// Проверка, пуста ли очередь
			if (myQueue.empty()) {
				std::cout << "Очередь пуста." << std::endl;
			}
			else {
				std::cout << "Очередь не пуста." << std::endl;
			}
			std::cout << "Исходная очредь: ";
			while(!myQueue.empty()) {
				std::cout << myQueue.front() << " ";
				myQueue.pop();
			}
			std::cout << std::endl;
			break;
		}
		case 4: {
			done = true;
			break;
		}
		default:
			std::cout << "Некорректный выбор. Попробуйте ещё раз." << std::endl;
			break;
		}
		if (choice != 4) std::cout << "-------------------------------" << std::endl;
	}

	return 0;
}