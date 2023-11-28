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
		std::cout << "������� (1/2/3/4): ";
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
						std::cout << "������� ������������ ��������!" << std::endl;
					}
				}
				else if (str == "delete" && std::stoi(value)) {
					try {
						int i = std::stoi(value);
						mySet.erase(i);
					}
					catch (const std::exception& e) {
						std::cout << "������� ������������ ��������!" << std::endl;
					}
				}
				else
					std::cout << "������� ������������ ��������!" << std::endl;

			}

			copy(mySet.begin(), mySet.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			break;
		}
		case 2: {
			std::cin.get();
			// ���������� �����
			std::stack<int> myStack;

			// ���������� ��������� � ����
			std::cout << "������� �������� �����: " << std::endl;
			std::string str;
			while (true) {
				std::getline(std::cin, str);
				if (str.empty())
					break;
				try {
					int i = std::stoi(str);
					myStack.push(i); // ��������� ������� � ����� ������
				}
				catch (const std::exception& e) {
					std::cout << "������� ������������ ��������!\n";
				}
			}

			// ����� �������� ��������
			std::cout << "������� �������: " << myStack.top() << std::endl;

			// �������� �������� �������� 
			myStack.pop();
			std::cout << "������� ������� ������ " << std::endl;

			// ����� ������ �������� ��������
			std::cout << "����� ������� �������: " << myStack.top() << std::endl;

			std::cout << "�������� ����: ";
			while (!myStack.empty()) {
				std::cout << myStack.top() << " ";
				myStack.pop();
			}
			std::cout << std::endl;
			break;
		}
		case 3: {

			// �������� �������
			std::queue<int> myQueue;

			// ���� 7 ����� ������������� � ���������� �� � �������
			std::cout << "������� 7 �����:\n";
			for (int i = 0; i < 7; ++i) {
				int number;
				std::cin >> number;
				myQueue.push(number);
			}

			// ����� ������� ����� � �������
			std::cout << "������ ����� � �������: " << myQueue.front() << std::endl;

			// �������� ������ ����� �� �������
			myQueue.pop();
			std::cout << "������ ����� � ������� ������� ";
			// ��������, ����� �� �������
			if (myQueue.empty()) {
				std::cout << "������� �����." << std::endl;
			}
			else {
				std::cout << "������� �� �����." << std::endl;
			}
			std::cout << "�������� ������: ";
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
			std::cout << "������������ �����. ���������� ��� ���." << std::endl;
			break;
		}
		if (choice != 4) std::cout << "-------------------------------" << std::endl;
	}

	return 0;
}