#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <locale>

using namespace std;

void Caesar_Cipher(int console_answer) {
	if (console_answer == 1) { //Шифр цезаря. Процесс шифрования исходного текста из файла
		string inputFile;
		cout << "Введите название файла, который необходимо зашифровать: ";
		cin >> inputFile; 

		ifstream fin(inputFile, ios::binary);
		ofstream fout("EncruptedFile.bin", ios::binary);

		if (!fin.is_open() || !fout.is_open()) {
			cerr << "Ошибка открытия файла для чтения" << endl;
			exit(0);
		}

		cout << "Придумайте пароль. Его нужно будет указать при расшифровке для получения доступа к данным. Не потеряйте его!\n";
		string password_;
		cin >> password_;
		cout << "Также для шифрования вам понадобится ключ.\nЕго значение вы должны запомнить! Введите значение ключа: ";
		int caesarKey;
		cin >> caesarKey;

		for (char i : password_) {
			if (i > 255) fout.put((i + caesarKey) % 256);
			else fout.put(char(i + caesarKey));
		}

		fout << " "; // Неккоректно дешифруется, нужно считывать пароль байтами

		char text;
		while (fin.get(text)) {
			if (text > 255) fout.put((text + caesarKey) % 256);
			else fout.put(char(text + caesarKey));
		}

		fin.close();
		fout.close();

	}
	if (console_answer == 2) {//Шифр цезаря. Процесс дешифрования исходного текста из файла
		cout << "Вы пытаетесь прочитать текст, защищённый шифром Цезаря. Введите ключ: ";
		int caesarKey;
		cin >> caesarKey;

		string inputFile;
		cout << "Введите название файла, который необходимо расшифровать: ";
		cin >> inputFile;

		//Расшифровываем пароль с указанным ключем
		ifstream fin(inputFile, ios::binary);
		string EncryptPassword_, DecryptPassword_; //Создаём переменные для приёма пароля из файла и для хранения расшифрованного пароля
		fin >> EncryptPassword_; //Первое слово в зашифрованном тексте(если его никто не менял) - зашифрованный пароль
		for (char i : EncryptPassword_) {
			DecryptPassword_ += char(i - caesarKey);
		}

		cout << "Введите пароль: ";
		string userTryPassword_;
		cin >> userTryPassword_;

		for (int i = 0; i < 3; i++) {
			if (userTryPassword_ == DecryptPassword_) { cout << "Пароль верный!\n"; break; }
			if (i == 3) { cout << "Попытки кончились."; exit(0); }
			cout << "Неверный пароль, попробуйте ещё раз. Попыток осталось: " << 3 - i << endl;
			cout << "Введите пароль: ";
			cin >> userTryPassword_;
		}

		string outputFile;
		cout << "Введите название расшифрованного файла: ";
		cin >> outputFile;
		ofstream fout(outputFile, ios::binary);

		char text;
		while (fin.get(text)) {
			if (text > 255) fout.put((text - caesarKey) % 256);
			else fout.put(char(text - caesarKey));
		}

		fin.close();
		fout.close();
	}
}

int main()
{
	//Блок 1.Блок для записи исходного текста в файл
	setlocale(LC_ALL, "Russian");
	cout << "Здравствуйте! Вы используете программу шифровки/дешифровки текста.\nВведите текст и выберите дальнейшие действия.\nТекст:";

	//

	//Блок 2.Блок для выбора действия. Выбор алгоритма шифрования.
	cout << "Исходный текст записан. Выберите метод шифрования.\n1) Шифр Цезаря\n2) Шифр XOR\n3) Шифр двойной табличной перестановки\n";
	int console_answer;
	cin >> console_answer;
	while (console_answer != 1 && console_answer != 2 && console_answer != 3) {
		cout << "Неккоректный выбор, введите ещё раз:\n";
		cin >> console_answer;
	}

	//

	// Блок 3. Запуск функции шифрования/дешифрования.
	string password;
	if (console_answer == 1) {
		cout << "Вы выбрали шифрование текста методом Цезаря.\nВы хотите зашифровать или расшифровать текст?\n1) Зашифровать\n2) Расшифровать\n";
		cin >> console_answer;
		while (console_answer != 1 && console_answer != 2) {
			cout << "Неккоректный выбор, введите ещё раз:\n";
			cin >> console_answer;
		}
		Caesar_Cipher(console_answer);
		cout << "Успешно! Зашифрованный текст находится в папке проекта в файле 'OutputText.txt'";
	}
	if (console_answer == 2) {
		cout << "Разрабатывается хыхы";
	}
	if (console_answer == 3) {
		cout << "Разрабатывается хыхы";
	}
}
