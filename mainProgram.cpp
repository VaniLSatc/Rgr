#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Platform.h"

using namespace std;

typedef void (*CaesarCipher)(int);
typedef void (*XORCipher)(int);
typedef void (*DTPCipher)(int);

namespace ConsoleUtils {
    void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    void showError(const char* message) {
    #ifdef _WIN32
        MessageBoxA(NULL, message, "Error", MB_OK);
    #else
        cout << "Ошибка: " << message << endl;
    #endif
    }
}

enum class EncryptionMethod {
    Caesar,
    XOR,
    DoubleTabularPermutation,
    Exit
};

enum class Action {
    Encrypt,
    Decrypt
};

void methodChoice(const int input, EncryptionMethod& choice) {
    switch (input) {
        case 1: choice = EncryptionMethod::Caesar; break;
        case 2: choice = EncryptionMethod::XOR; break;
        case 3: choice = EncryptionMethod::DoubleTabularPermutation; break;
        case 4: choice = EncryptionMethod::Exit; break;
        default: break;
    }
}

void actionChoice(const int input, Action& choice) {
    if (input == 1) choice = Action::Encrypt;
    else if (input == 2) choice = Action::Decrypt;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    while (true) {
        ConsoleUtils::clearConsole();
        cout << "Здравствуйте! Вы используете программу шифровки/дешифровки текста.\n";
        cout << "Выберите метод шифрования:\n";
        cout << "1) Шифр Цезаря\n";
        cout << "2) Шифр XOR\n";
        cout << "3) Шифр двойной табличной перестановки\n";
        cout << "4) Выход\n";
        
        int console_answer;
        cin >> console_answer;
        
        EncryptionMethod method;
        methodChoice(console_answer, method);
        
        if (method == EncryptionMethod::Exit) break;
        
        if (console_answer < 1 || console_answer > 3) {
            cout << "Некорректный выбор!\n";
            continue;
        }
        
        cout << "Вы хотите зашифровать или расшифровать данные?\n";
        cout << "1) Зашифровать\n2) Расшифровать\n";
        int action_input;
        cin >> action_input;
        
        Action action;
        actionChoice(action_input, action);
        
        if (action_input != 1 && action_input != 2) {
            cout << "Некорректный выбор!\n";
            continue;
        }

        LIB_HANDLE library = nullptr;
        const char* libName = "";
        
        switch (method) {
        case EncryptionMethod::Caesar:
        #ifdef _WIN32
            libName = "CaesarCipherDLL.dll";
        #else
            libName = "./libCaesarCipher.so";
        #endif
            break;
        case EncryptionMethod::XOR:
        #ifdef _WIN32
            libName = "XORCipherDLL.dll";
        #else
            libName = "./libXORCipher.so";
        #endif
            break;
        case EncryptionMethod::DoubleTabularPermutation:
        #ifdef _WIN32
            libName = "DoubleTabularPermutation.dll";
        #else
            libName = "./libDoubleTabularPermutation.so";
        #endif
            break;
        default:
            break;
        }
        
        library = LOAD_LIB(libName);
        if (!library) {
            ConsoleUtils::showError("Библиотека не найдена!");
            continue;
        }
        
        // Вызов соответствующей функции
        int action_code = (action == Action::Encrypt) ? 1 : 2;
        
        switch (method) {
        case EncryptionMethod::Caesar: {
            CaesarCipher func = (CaesarCipher)GET_PROC(library, "Caesar_Cipher");
            if (func) func(action_code);
            break;
        }
        case EncryptionMethod::XOR: {
            XORCipher func = (XORCipher)GET_PROC(library, "XOR_Cipher");
            if (func) func(action_code);
            break;
        }
        case EncryptionMethod::DoubleTabularPermutation: {
            DTPCipher func = (DTPCipher)GET_PROC(library, "doubleTablePermutationCipher");
            if (func) func(action_code);
            break;
        }
        default:
            break;
        }
        
        if (library) FREE_LIB(library);
        
        cout << "\nНажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}
