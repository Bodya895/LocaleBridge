#include <iostream>
#include <string>
#include <windows.h> 

using namespace std;

string buildMessage(const string& clientName, const string& outMessage)
{
    return "[" + clientName + "] : " + outMessage;
}


string Equal(string actual, string expected, string testName)
{
	if (actual == expected)
	{
		cout << "\n[PASS] " << testName;
		return "PASS";
	}
	else
	{
		cout << "\n[FAIL] " << testName;
		cout << "\n  Expected: " << expected;
		cout << "\n  Actual:   " << actual;
		return "FAIL";
	}
}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	cout << "ТЕСТУВАННЯ ЛОГІКИ КЛІЄНТА";

	string t1_name = "UserA";
	string t1_data = "Привіт, як справи?";
	string t1_expected = "[UserA] : Привіт, як справи?";
	string t1_actual = buildMessage(t1_name, t1_data);
	Equal(t1_actual, t1_expected, "1.Повідомлення з сервера.");

	string t2_name = "ip";
	string t2_data = "Нормально";
	string t2_expected = "[ip] : Нормально";
	string t2_actual = buildMessage(t2_name, t2_data);
	Equal(t2_actual, t2_expected, "2. Повідомлення без нікнейму");



	cout << "\nТЕСТУВАННЯ ЗАВЕРШЕНО" << endl;

	return 0;
}

