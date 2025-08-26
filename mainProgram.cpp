#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Windows.h"

// Сделать этот проект многофайловым
// Добавить шифрование с консоли
// Сделать менюшку красивой, добавить очистку консоли
// Запустить на линуксе


using namespace std;

typedef void (*CaesarCipher)(int);
typedef void (*XORCipher)(int);
typedef void (*DTPCipher)(int);

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
	
	
	switch (console_answer)
	{
	case 1:
	{
		cout << "Вы выбрали шифрование методом Цезаря.\nВы хотите зашифровать или расшифровать данные?\n1) Зашифровать\n2) Расшифровать\n";
		cin >> console_answer;

		while (console_answer != 1 && console_answer != 2) {
			cout << "Неккоректный выбор, введите ещё раз:\n";
			cin >> console_answer;
		}

		HMODULE loadCesar = LoadLibrary(L"CaesarCipherDLL.dll"); // Загружаем DLL
		if (!loadCesar) {
			MessageBox(NULL, L"DLL not found!", L"Error", MB_OK);  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return 1;
		}

		// Получаем указатель на функцию
		CaesarCipher Caesar_Cipher = (CaesarCipher)GetProcAddress(loadCesar, "Caesar_Cipher");
		if (!Caesar_Cipher) {
			MessageBox(NULL, L"Function not found!", L"Error", MB_OK); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			FreeLibrary(loadCesar);
			return 1;
		}

		Caesar_Cipher(console_answer); // Вызываем функцию

		FreeLibrary(loadCesar); // Выгружаем DLL

		cout << "Успешно! Зашифрованный текст находится в папке проекта";

		return 0;
	}

	case 2:
	{
		cout << "Вы выбрали шифрование методом XOR.\nВы хотите зашифровать или расшифровать данные?\n1) Зашифровать\n2) Расшифровать\n";
		cin >> console_answer;

		while (console_answer != 1 && console_answer != 2) {
			cout << "Неккоректный выбор, введите ещё раз:\n";
			cin >> console_answer;
		}

		HMODULE loadXOR = LoadLibrary(L"XORCipherDLL.dll"); // Загружаем DLL
		if (!loadXOR) {
			MessageBox(NULL, L"DLL not found!", L"Error", MB_OK);  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return 1;
		}

		// Получаем указатель на функцию
		XORCipher XOR_Cipher = (XORCipher)GetProcAddress(loadXOR, "XOR_Cipher");
		if (!XOR_Cipher) {
			MessageBox(NULL, L"Function not found!", L"Error", MB_OK); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			FreeLibrary(loadXOR);
			return 1;
		}

		XOR_Cipher(console_answer); // Вызываем функцию

		FreeLibrary(loadXOR); // Выгружаем DLL

		cout << "Успешно! Зашифрованный текст находится в папке проекта";

		return 0;
	}
	case 3:
	{
		cout << "Вы выбрали шифрование методом двойной табличной перестановки.\nВы хотите зашифровать или расшифровать данные?\n1) Зашифровать\n2) Расшифровать\n";
		cin >> console_answer;

		while (console_answer != 1 && console_answer != 2) {
			cout << "Неккоректный выбор, введите ещё раз:\n";
			cin >> console_answer;
		}

		HMODULE loadDTP = LoadLibrary(L"DoubleTabularPermutation.dll"); // Загружаем DLL
		if (!loadDTP) {
			MessageBox(NULL, L"DLL not found!", L"Error", MB_OK);  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return 1;
		}

		// Получаем указатель на функцию
		DTPCipher DTP_Cipher = (DTPCipher)GetProcAddress(loadDTP, "doubleTablePermutationCipher");
		if (!DTP_Cipher) {
			MessageBox(NULL, L"Function not found!", L"Error", MB_OK); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			FreeLibrary(loadDTP);
			return 1;
		}

		DTP_Cipher(console_answer); // Вызываем функцию

		FreeLibrary(loadDTP); // Выгружаем DLL

		cout << "Успешно! Зашифрованный текст находится в папке проекта";

		return 0;
	}

	}
}
