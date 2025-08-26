#include "CaesarCipher.h"

using namespace std;

void Caesar_Cipher(int console_answer) {
    const size_t PASSWORD_SIZE = 32; // Фиксированный размер пароля

    if (console_answer == 1) { // Шифрование
        string inputFile;
        cout << "Введите название файла, который необходимо зашифровать: ";
        cin >> inputFile;

        string outputFile = "EncryptedFile.bin";

        ifstream fin(inputFile, ios::binary);
        ofstream fout(outputFile, ios::binary);

        if (!fin.is_open() || !fout.is_open()) {
            cerr << "Ошибка открытия файла!" << endl;
            exit(1);
        }

        cout << "Придумайте пароль (макс. " << PASSWORD_SIZE << " символов): ";
        string password;
        cin >> password;

        // Обрезаем или дополняем пароль до нужного размера
        password.resize(PASSWORD_SIZE, '\0'); // Дополняем нулями, если пароль короче

        cout << "Введите ключ шифрования: ";
        int caesarKey;
        cin >> caesarKey;

        // Записываем зашифрованный пароль (фиксированный размер)
        for (char c : password) {
            fout.put(static_cast<char>((c + caesarKey) % 256));
        }

        // Шифруем и записываем остальной файл
        char ch;
        while (fin.get(ch)) {
            fout.put(static_cast<char>((ch + caesarKey) % 256));
        }

        fin.close();
        fout.close();
    }

    if (console_answer == 2) { // Дешифрование
        string inputFile;
        cout << "Введите название файла для расшифровки: ";
        cin >> inputFile;

        ifstream fin(inputFile, ios::binary);
        if (!fin.is_open()) {
            cerr << "Ошибка открытия файла!" << endl;
            exit(1);
        }

        cout << "Введите ключ дешифрования: ";
        int caesarKey;
        cin >> caesarKey;

        // Читаем ровно PASSWORD_SIZE байт (зашифрованный пароль)
        char encryptedPassword[PASSWORD_SIZE];
        fin.read(encryptedPassword, PASSWORD_SIZE);

        // Дешифруем пароль
        string decryptedPassword;
        for (size_t i = 0; i < PASSWORD_SIZE; ++i) {
            decryptedPassword += static_cast<char>((encryptedPassword[i] - caesarKey) % 256);
        }

        // Удаляем нулевые символы, если пароль был короче PASSWORD_SIZE
        decryptedPassword.erase(decryptedPassword.find('\0'));

        // Проверка пароля
        string userPassword;
        int attempts = 3;
        while (attempts > 0) {
            cout << "Введите пароль (попыток: " << attempts << "): ";
            cin >> userPassword;

            if (userPassword == decryptedPassword) {
                cout << "Пароль верный!\n";
                break;
            }
            attempts--;
            if (attempts == 0) {
                cout << "Доступ запрещен!\n";
                fin.close();
                exit(0);
            }
        }

        // Дешифруем и сохраняем остальной файл
        string outputFile;
        cout << "Введите название для расшифрованного файла: ";
        cin >> outputFile;

        ofstream fout(outputFile, ios::binary);
        char ch;
        while (fin.get(ch)) {
            fout.put(static_cast<char>((ch - caesarKey) % 256));
        }

        fin.close();
        fout.close();
    }
}
