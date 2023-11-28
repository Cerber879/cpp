// 11. В  файле  записан  текст  стихотворения. Форматировать
// текст так, чтобы каждый куплет из 4 строк следовал с одной и
// той же позиции, начинался с прописной буквы и был сдвинут
// относительно предыдущего куплета на 5 позиций вправо или влево
// поочередно(6).

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Функция для пропуска пробелов в файле
void skip_probel(ifstream& file, char& ch) {
    while (file.get(ch) && ch == ' ') {
        if (ch != ' ')
            cout << ch;
    }
}

// Преобразователь символа в верхний регистр
char toUpper(char& ch) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'а' && ch <= 'я')) { 
        return ch - 'a' + 'A'; 
    }
    return ch; 
}

// Функция для обработки файла
void processFile(string& filename, string& formated_file) {
    ifstream file(filename);
    ofstream outf(formated_file);
    const string voidString = "     ";

    if (!file) {
        cout << "Ошибка открытия файла " << filename << endl;
        return;
    }
    else cout << "Файл для чтения успешно открыт!" << endl;

    char ch;
    int endLine = 0;
    bool startLine = true;
    bool kupletEven = true;
    while (file.get(ch)) {
        // Обработка начала строки
        if (startLine) {
            if (ch == ' ' || ch == '\n')
                continue;

            if (endLine % 4 == 0) {
                if (endLine != 0)
                    outf << endl;

                (!kupletEven) ? kupletEven = true : kupletEven = false;
            }
            if (kupletEven)
                outf << voidString;

            ch = toUpper(ch);
            startLine = false;
        }
        // Пропуск лишних пробелов
        if (ch == ' ') {
            outf << ch;
            skip_probel(file, ch);
        }
        // Проверка на конец строки
        if (ch == '\n') {
            startLine = true;
            endLine++;
        }
        outf << ch; // Вывод прочитанного символа
    }
    file.close();
    outf.close();
}

int main() {
    setlocale(LC_ALL, "RU");
    string read_file_name;
    string formatted_data_filename;

    cout << "Введите имя файла для чтения: ";
    getline(cin, read_file_name);

    cout << "Введите имя файла для записи: ";
    getline(cin, formatted_data_filename);

    processFile(read_file_name, formatted_data_filename);

    return 0;
}