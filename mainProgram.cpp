#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <locale>

using namespace std;

void Caesar_Cipher(int console_answer) {
	if (console_answer == 1) { //Шифр цезаря. Процесс шифрования исходного текста из файла
		ifstream fin("InputText.txt");
		ofstream fout("OutputText.txt");
		cout << "Придумайте пароль. Его нужно будет указать при расшифровке для получения доступа к данным. Не потеряйте его!\n";
		string pswrd;
		cin >> pswrd;
		cout << "Также чтобы зашифровать текст вам понадобится ключ.\nЕго значение вы также должны запомнить! Введите значение ключа(положительное целое число): ";
		int n;
		cin >> n;

		for (char i : pswrd) {
			fout << char(i + n); //Запись зашифрованного пароля в файл
		}
		fout << " ";
		fout << char(n + n); // Запись ключа в зашифрованном виде в файл
		fout << " ";
		string txt;
		while (fin >> txt) {
			for (char i : txt) {
				fout << char(i + n);
			}
			fout << " ";
		}
		fin.close();
		fout.close();
	}
	if (console_answer == 2) {//Шифр цезаря. Процесс дешифрования исходного текста из файла
		cout << "Вы пытаетесь прочитать текст, защищённый шифром Цезаря. Введите ключ: ";
		int n;
		cin >> n;
		//

		//Расшифровываем пароль с указанным ключем
		ifstream fin("InputText.txt");
		string fpswrd, decrypt_fpswrd; //Создаём переменные для приёма пароля из файла и для хранения расшифрованного пароля
		fin >> fpswrd; //Первое слово в зашифрованном тексте(если его никто не менял) - зашифрованный пароль
		for (char i : fpswrd) {
			decrypt_fpswrd += char(i - n);
		}
		cout << "Введите пароль: ";
		string pswrd;
		cin >> pswrd;
		for (int i = 0; i < 3; i++) {
			if (pswrd == decrypt_fpswrd) { cout << "Пароль верный!\n"; break; }
			if (i == 3) { cout << "Попытки кончились."; exit(0); }
			cout << "Неверный пароль, попробуйте ещё раз. Попыток осталось: " << 3 - i << endl;
			cout << "Введите пароль: ";
			cin >> pswrd;
		}

		//

		ofstream fout("OutputText.txt");
		string txt;
		fin >> txt; // Считали значение ключа, чтобы убрать его из финального текста
		while (fin >> txt) {
			for (char i : txt) {
				fout << char(i - 3);
			}
			fout << " ";
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
	string text;
	getline(cin, text);
	ofstream fout("InputText.txt");
	for (auto i : text) {
		fout << i;
	}
	fout.close();
	text = "";

	//

	//Блок 2.Блок для выбора действия. Выбор алгоритма шифрования.
	cout << "Исходный текст записан. Выберите метод шифрования.\n1) Шифр Цезаря\n2) Шифр чего там\n3) Шифр двойной табличной перестановки\n";
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
