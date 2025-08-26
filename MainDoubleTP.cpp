#include "DTPdll.h"

using namespace std;

const int PASSWORD_SIZE = 32;
const int PASS_COLS = 8;
const int PASS_ROWS = PASSWORD_SIZE / PASS_COLS;

// Общие функции
vector<string> stringToTable(const string& text, int cols) {
    int rows = text.size() / cols;
    vector<string> table(rows);
    for (int i = 0; i < rows; ++i) {
        table[i] = text.substr(i * cols, cols);
    }
    return table;
}

string tableToString(const vector<string>& table) {
    string result;
    for (const auto& row : table) result += row;
    return result;
}

vector<string> permuteColumns(const vector<string>& table, const vector<int>& colKey) {
    int rows = table.size(), cols = table[0].size();
    vector<string> result(rows, string(cols, ' '));
    for (int c = 0; c < cols; ++c) {
        int newCol = colKey[c] - 1;
        for (int r = 0; r < rows; ++r) {
            result[r][c] = table[r][newCol];
        }
    }
    return result;
}

vector<string> permuteRows(const vector<string>& table, const vector<int>& rowKey) {
    int rows = table.size();
    vector<string> result(rows);
    for (int r = 0; r < rows; ++r) {
        result[r] = table[rowKey[r] - 1];
    }
    return result;
}

vector<int> inverseKey(const vector<int>& key) {
    vector<int> invKey(key.size());
    for (int i = 0; i < key.size(); ++i) {
        invKey[key[i] - 1] = i + 1;
    }
    return invKey;
}

string encryptPassword(const string& password, const vector<int>& passKey) {
    vector<string> table = stringToTable(password, PASS_COLS);
    vector<string> encryptedTable(PASS_ROWS, string(PASS_COLS, ' '));
    for (int c = 0; c < PASS_COLS; ++c) {
        for (int r = 0; r < PASS_ROWS; ++r) {
            encryptedTable[r][c] = table[r][passKey[c] - 1];
        }
    }
    return tableToString(encryptedTable);
}

string decryptPassword(const string& encryptedPassword, const vector<int>& passKey) {
    vector<int> invPassKey = inverseKey(passKey);
    vector<string> table = stringToTable(encryptedPassword, PASS_COLS);
    vector<string> decryptedTable(PASS_ROWS, string(PASS_COLS, ' '));
    for (int c = 0; c < PASS_COLS; ++c) {
        for (int r = 0; r < PASS_ROWS; ++r) {
            decryptedTable[r][c] = table[r][invPassKey[c] - 1];
        }
    }
    return tableToString(decryptedTable);
}

string encryptText(const string& plaintext, const vector<int>& colKey, const vector<int>& rowKey) {
    vector<string> table = stringToTable(plaintext, colKey.size());
    return tableToString(permuteRows(permuteColumns(table, colKey), rowKey));
}

string decryptText(const string& ciphertext, const vector<int>& colKey, const vector<int>& rowKey) {
    vector<string> table = stringToTable(ciphertext, colKey.size());
    vector<int> invRowKey = inverseKey(rowKey);
    vector<int> invColKey = inverseKey(colKey);
    return tableToString(permuteColumns(permuteRows(table, invRowKey), invColKey));
}

void saveKeys(const string& filename, const vector<int>& colKey, const vector<int>& rowKey, const vector<int>& passKey) {
    ofstream out(filename);
    for (int c : colKey) out << c << " "; out << "\n";
    for (int r : rowKey) out << r << " "; out << "\n";
    for (int k : passKey) out << k << " "; out << "\n";
    out.close();
}

bool loadKeys(const string& filename, vector<int>& colKey, vector<int>& rowKey, vector<int>& passKey) {
    ifstream in(filename);
    if (!in) return false;

    string line;
    auto readLine = [](const string& line, vector<int>& vec) {
        istringstream ss(line);
        int x;
        while (ss >> x) vec.push_back(x);
        };

    if (getline(in, line)) readLine(line, colKey);
    if (getline(in, line)) readLine(line, rowKey);
    if (getline(in, line)) readLine(line, passKey);

    return true;
}

void encryptFile() {
    string inputFile, outputFile, keyFile;
    cout << "Введите имя исходного файла: ";
    cin >> inputFile;
    cout << "Введите имя файла для зашифрованного текста: ";
    cin >> outputFile;
    cout << "Введите имя файла для ключей: ";
    cin >> keyFile;

    ifstream in(inputFile);
    if (!in) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    string plaintext((istreambuf_iterator<char>(in)), {});
    in.close();
    replace(plaintext.begin(), plaintext.end(), ' ', '_');

    int rows, cols;
    cout << "Введите число строк и столбцов для таблицы: ";
    cin >> rows >> cols;

    if (plaintext.size() < rows * cols) plaintext.resize(rows * cols, '_');
    else if (plaintext.size() > rows * cols) plaintext = plaintext.substr(0, rows * cols);

    vector<int> colKey(cols), rowKey(rows), passKey(PASS_COLS);
    cout << "Введите ключ столбцов (" << cols << " чисел): ";
    for (int& x : colKey) cin >> x;
    cout << "Введите ключ строк (" << rows << " чисел): ";
    for (int& x : rowKey) cin >> x;
    cout << "Введите ключ пароля (" << PASS_COLS << " чисел): ";
    for (int& x : passKey) cin >> x;

    cout << "Введите пароль: ";
    string password;
    cin.ignore();
    getline(cin, password);
    if (password.size() < PASSWORD_SIZE) password.resize(PASSWORD_SIZE, ' ');
    else if (password.size() > PASSWORD_SIZE) password = password.substr(0, PASSWORD_SIZE);

    string encryptedPassword = encryptPassword(password, passKey);
    string encryptedText = encryptText(plaintext, colKey, rowKey);

    saveKeys(keyFile, colKey, rowKey, passKey);

    ofstream out(outputFile, ios::binary);
    out.write(encryptedPassword.c_str(), encryptedPassword.size());
    out.write(encryptedText.c_str(), encryptedText.size());
    out.close();

    cout << "Файл зашифрован: " << outputFile << endl;
}

void decryptFile() {
    string inputFile, outputFile, keyFile;
    cout << "Введите имя зашифрованного файла: ";
    cin >> inputFile;
    cout << "Введите имя выходного файла: ";
    cin >> outputFile;
    cout << "Введите имя файла с ключами: ";
    cin >> keyFile;

    vector<int> colKey, rowKey, passKey;
    if (!loadKeys(keyFile, colKey, rowKey, passKey)) {
        cout << "Ошибка чтения ключей!" << endl;
        return;
    }

    ifstream in(inputFile, ios::binary);
    if (!in) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    string encryptedPassword(PASSWORD_SIZE, '\0');
    in.read(&encryptedPassword[0], PASSWORD_SIZE);
    string encryptedText((istreambuf_iterator<char>(in)), {});
    in.close();

    string decryptedPassword = decryptPassword(encryptedPassword, passKey);

    for (int attempts = 3; attempts >= 1; attempts--) {
        cout << "Введите пароль (" << attempts << " попыток): ";
        string userPassword;
        cin.ignore();
        getline(cin, userPassword);

        string userPasswordCompare = userPassword;
        if (userPasswordCompare.size() < PASSWORD_SIZE) userPasswordCompare.resize(PASSWORD_SIZE, ' ');
        else if (userPasswordCompare.size() > PASSWORD_SIZE) userPasswordCompare = userPasswordCompare.substr(0, PASSWORD_SIZE);

        if (userPasswordCompare == decryptedPassword) {
            cout << "Пароль верный!" << endl;
            string decryptedText = decryptText(encryptedText, colKey, rowKey);
            replace(decryptedText.begin(), decryptedText.end(), '_', ' ');

            ofstream out(outputFile);
            out << decryptedText;
            out.close();

            cout << "Файл расшифрован: " << outputFile << endl;
            return;
        }
        cout << "Неверный пароль!" << endl;
    }
    cout << "Доступ запрещен!" << endl;
}

void doubleTablePermutationCipher(int console_answer) {
    switch (console_answer) {
    case 1: encryptFile(); break;
    case 2: decryptFile(); break;
    }
}
