#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	string InWay = "encryptedtext.bin"; //Входящий файл должен существовать в папке проекта обязательно.
	string OutWay = "decryptedtext.txt";


	ifstream in(InWay, ios::binary);
	ofstream out(OutWay, ios::binary);

	if (!in.is_open() || !out.is_open()) {
		cerr << "File opening error" << endl;
		return 0;
	}

	char text;

	while (in.get(text)) { // .get() - СЧИТЫВАЕТ из файла по байту, без лишнего, все пробелы и так далее сохраняются
			if (text > 255) out.put((text - 3) % 256); // Это проверка для таблицы АSCII, делить на 256.
			else out.put(char(text - 3)); // .put() - то же самое, но только ЗАПИСЫВАЕТ в файл по байту.
	}

	in.close(); // На всякий случай - потоки нужно закрывать, потому что так правильно.
	out.close();


	return 0;
}
