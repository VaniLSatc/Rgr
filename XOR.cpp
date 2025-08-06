#include <iostream>
#include <string>
#include <vector>

// Функция для шифрования/дешифрования строки с помощью XOR
std::string xor_crypt(const std::string& data, const std::string& key) {
    std::string output;
    output.resize(data.size());

    for (size_t i = 0; i < data.size(); ++i) {
        output[i] = data[i] ^ key[i % key.size()];
    }

    return output;
}

// Функция для шифрования/дешифрования файла
bool xor_file(const std::string& input_path,
    const std::string& output_path,
    const std::string& key) {
    // Чтение входного файла
    FILE* input_file = fopen(input_path.c_str(), "rb");
    if (!input_file) {
        std::cerr << "Ошибка открытия входного файла!" << std::endl;
        return false;
    }

    // Определение размера файла
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Чтение данных
    std::vector<char> buffer(file_size);
    fread(buffer.data(), 1, file_size, input_file);
    fclose(input_file);

    // Шифрование данных
    for (size_t i = 0; i < buffer.size(); ++i) {
        buffer[i] ^= key[i % key.size()];
    }

    // Запись выходного файла
    FILE* output_file = fopen(output_path.c_str(), "wb");
    if (!output_file) {
        std::cerr << "Ошибка создания выходного файла!" << std::endl;
        return false;
    }

    fwrite(buffer.data(), 1, buffer.size(), output_file);
    fclose(output_file);

    return true;
}

int main() {
    // Пример использования для строки
    std::string message = "Hello, World!";
    std::string key = "secret";

    std::cout << "Исходное сообщение: " << message << std::endl;

    // Шифрование
    std::string encrypted = xor_crypt(message, key);
    std::cout << "Зашифрованное сообщение: ";
    for (char c : encrypted) {
        std::cout << std::hex << (int)(unsigned char)c << " ";
    }
    std::cout << std::endl;

    // Дешифрование (XOR обратим)
    std::string decrypted = xor_crypt(encrypted, key);
    std::cout << "Расшифрованное сообщение: " << decrypted << std::endl;

    // Пример использования для файла
    std::string input_file = "document.txt";
    std::string encrypted_file = "document_encrypted.txt";
    std::string decrypted_file = "document_decrypted.txt";

    // Шифруем файл
    if (xor_file(input_file, encrypted_file, key)) {
        std::cout << "Файл успешно зашифрован!" << std::endl;
    }

    // Дешифруем файл
    if (xor_file(encrypted_file, decrypted_file, key)) {
        std::cout << "Файл успешно расшифрован!" << std::endl;
    }

    return 0;
}
