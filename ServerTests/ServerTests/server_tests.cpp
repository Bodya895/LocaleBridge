#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

string buildBroadcastMessage(const string& clientName, const string& receivedData) {
    return "[" + clientName + "] : " + receivedData;
}

void assertEqual(const string& actual, const string& expected, const string& testName) {
    if (actual == expected) {
        cout << "[УСПІХ] " << testName << endl;
    }
    else {
        cout << "[ПОМИЛКА] " << testName << endl;
        cout << "Очікувано: " << expected << endl;
        cout << "Отримано:  " << actual << endl;
    }
}

void runUnitTests() {
    cout << "\n--- ТЕСТУВАННЯ СЕРВЕРНОЇ ЛОГІКИ ---\n" << endl;

    string t1_name = "UserA";
    string t1_data = "Привіт, як справи?";
    string t1_expected = "[UserA] : Привіт, як справи?";
    string t1_actual = buildBroadcastMessage(t1_name, t1_data);
    assertEqual(t1_actual, t1_expected, "1. Стандартне повідомлення");

    string t2_name = "TestBot";
    string t2_data = "Код: 404. Помилка!";
    string t2_expected = "[TestBot] : Код: 404. Помилка!";
    string t2_actual = buildBroadcastMessage(t2_name, t2_data);
    assertEqual(t2_actual, t2_expected, "2. Повідомлення з цифрами");

    string t3_name = "";
    string t3_data = "Повідомлення від невідомого.";
    string t3_expected = "[] : Повідомлення від невідомого.";
    string t3_actual = buildBroadcastMessage(t3_name, t3_data);
    assertEqual(t3_actual, t3_expected, "3. Повідомлення без імені");

    cout << "\n--- ТЕСТУВАННЯ ЗАВЕРШЕНО ---\n" << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    runUnitTests();

    return 0;
}