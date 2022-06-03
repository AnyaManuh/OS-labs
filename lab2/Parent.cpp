#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS 

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	char lpszAppName[] = "Lab2myvar ";


	cout << "Введите символ Х:\n";
	char* x = new char[10];
	cin >> x;

	cout << "Введите размер массива:\n";
	int size;
	cin >> size;

	char* tmp = new char[10];


	_itoa(size, tmp, 10);
	strcat(lpszAppName, tmp);
	strcat_s(lpszAppName, " ");

	strcat_s(lpszAppName, x);
	strcat_s(lpszAppName, " ");

	cout << "Введите " << size << " символов:\n";
	for (int i = 0; i < size; i++)
	{
		cin >> tmp;
		strcat_s(lpszAppName, tmp);
	}

	STARTUPINFOA si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (!CreateProcessA(NULL, lpszAppName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}

	_cputs("The new process is created.\n");

	WaitForSingleObject(piApp.hProcess, INFINITE);
	system("pause");
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	TerminateProcess(piApp.hProcess, 1);
	ExitProcess(1);


	
	return 0;
}