#include "XORCipher.h"

using namespace std;

// Генерация случайного ключа
vector<unsigned char> generate_key() {

    vector<unsigned char> key(256);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);

    for (unsigned char& byte : key) {
        byte = dis(gen);
    }
    return key;
}

// Функция для XOR-шифрования/дешифрования файла
void XOR_Cipher(int console_answer) {
    const size_t passwordSize = 32; // Фиксированный размер пароля

    switch (console_answer)
    {
    
    case 1: // Шифрование
    {
        string inputFile, outputFile;
        cout << "Введите имя исходного файла: ";
        cin >> inputFile;
        outputFile = inputFile + ".bin";

        ifstream in(inputFile, ios::binary);
        if (!in.is_open()) {
            cout << "Ошибка: не удалось открыть исходный файл" << endl;
            return;
        }

        vector<unsigned char> key = generate_key(); // Генерируем ключ шифрования

        cout << "Имя файла с ключом по умолчанию 'key.bin', хотите его изменить?\n1. Изменить\n2. Оставить по умолчанию\n";
        cin >> console_answer;
        while (console_answer != 1 && console_answer != 2) {
            cout << "Неверно! Введите ещё раз: ";
            cin >> console_answer;
        }

        if (console_answer == 1) {
            string keyFile;
            cout << "Введите название файла для записи ключа: ";
            cin >> keyFile;

            ofstream outKey(keyFile, ios::binary);
            for (unsigned char byte : key) {
                outKey.put(static_cast<char>(byte)); // Запись ключа
            }
            outKey.close();

        }
        if (console_answer == 2) {
            ofstream outKey("key.bin", ios::binary);
            for (unsigned char byte : key) {
                outKey.put(static_cast<char>(byte)); // Запись ключа
            }
            outKey.close();
        }

        cout << "Придумайте пароль не больше, чем " << passwordSize << " символа (Пароль не должен содержать в себе пробелы!): ";
        string password;
        cin >> password;
        password.resize(passwordSize, '\0');

        // Шифрование пароля
        vector<unsigned char> encryptedPassword(password.begin(), password.end());
        for (size_t i = 0; i < encryptedPassword.size(); ++i) {
            encryptedPassword[i] ^= key[i]; // Используем ключ с позиции 0
        }

        // Запись зашифрованного пароля в файл
        ofstream out(outputFile, ios::binary);
        out.write(reinterpret_cast<const char*>(encryptedPassword.data()), passwordSize);

        // Шифрование данных
        vector<unsigned char> buffer(1024);
        size_t totalBytesProcessed = 0;

        while (in.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
            size_t bytesRead = in.gcount();

            for (size_t i = 0; i < bytesRead; ++i) {
                // Используем ключ с позиции passwordSize + totalBytesProcessed + i
                size_t keyPosition = passwordSize + totalBytesProcessed + i;
                buffer[i] ^= key[keyPosition % key.size()];
            }

            // Запись зашифрованных данных в файл
            out.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
            totalBytesProcessed += bytesRead;
        }

        // Обработка остатка
        size_t bytesRead = in.gcount();
        if (bytesRead > 0) {
            for (size_t i = 0; i < bytesRead; ++i) {
                size_t keyPosition = passwordSize + totalBytesProcessed + i;
                buffer[i] ^= key[keyPosition % key.size()];
            }
            out.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
        }

        in.close();
        out.close();
        cout << "Файл зашифрован: " << outputFile << endl;
    }
    break;

    case 2: // Дешифрование
    {
        string inputFile, outputFile, keyFile;
        cout << "Введите имя зашифрованного файла: ";
        cin >> inputFile;

        ifstream in(inputFile, ios::binary);
        if (!in.is_open()) {
            cout << "Ошибка: не удалось открыть зашифрованный файл\n";
            return;
        }

        cout << "Введите имя выходного файла: ";
        cin >> outputFile;
        cout << "Введите имя файла с ключом: ";
        cin >> keyFile;

        // Чтение ключа
        ifstream inKey(keyFile, ios::binary);
        if (!inKey.is_open()) {
            cout << "Ошибка: не удалось открыть файл с ключом\n";
            return;
        }

        vector<unsigned char> key;
        char byte;
        while (inKey.get(byte)) {
            key.push_back(static_cast<unsigned char>(byte));
        }
        inKey.close();

        if (key.empty()) {
            cout << "Ошибка: ключ пустой\n";
            return;
        }

        // Чтение зашифрованного пароля
        vector<unsigned char> encryptedPassword(passwordSize);
        in.read(reinterpret_cast<char*>(encryptedPassword.data()), passwordSize);

        // Дешифровка пароля
        string decryptedPassword;
        for (size_t i = 0; i < encryptedPassword.size(); ++i) {
            char decryptedSymbol = encryptedPassword[i] ^ key[i]; // Ключ с позиции 0
            if (decryptedSymbol != '\0') {
                decryptedPassword += decryptedSymbol;
            }
        }

        // Проверка пароля
        for (int attempts = 3; attempts >= 0; attempts--) {
            if (attempts == 0) {
                cout << "Доступ запрещен!\n";
                in.close();
                return;
            }

            string userPassword;
            cout << "Введите пароль (осталось попыток: " << attempts << "): ";
            cin >> userPassword;

            if (userPassword == decryptedPassword) {
                cout << "Пароль верный! Доступ разрешен.\n";
                break;
            }
            cout << "Неверный пароль!\n";
        }

        // Дешифровка данных
        ofstream out(outputFile, ios::binary);
        vector<unsigned char> buffer(1024);
        size_t totalBytesProcessed = 0;

        while (in.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
            size_t bytesRead = in.gcount();

            for (size_t i = 0; i < bytesRead; ++i) {
                // Используем ключ с позиции passwordSize + totalBytesProcessed + i
                size_t keyPosition = passwordSize + totalBytesProcessed + i;
                buffer[i] ^= key[keyPosition % key.size()];
            }

            // Запись дешифрованных данных в файл
            out.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
            totalBytesProcessed += bytesRead;
        }

        // Обработка остатка
        size_t bytesRead = in.gcount();
        if (bytesRead > 0) {
            for (size_t i = 0; i < bytesRead; ++i) {
                size_t keyPosition = passwordSize + totalBytesProcessed + i;
                buffer[i] ^= key[keyPosition % key.size()];
            }
            out.write(reinterpret_cast<const char*>(buffer.data()), bytesRead);
        }

        in.close();
        out.close();
        cout << "Файл расшифрован: " << outputFile << endl;
    }
    break;

    }
}
