#include <windows.h>
#include <iostream>
#include <conio.h>
#include <list>
#define _CRT_SECURE_NO_WARNINGS 

using namespace std;

HANDLE event1;
HANDLE event2;
HANDLE event3;
HANDLE event4;

HANDLE parentEvent1;
HANDLE parentEvent2;
HANDLE parentEndEvent;

HANDLE childEvent1;
HANDLE childEvent2;
HANDLE childEndEvent;



HANDLE semaphore;

bool flag1 = true;
bool flag2 = true;
bool flag3 = true;

volatile int messageParentEventCount = 0;
volatile int messageParentEndCount = 0;

struct ProcessInformation 
{
	STARTUPINFO info;
	PROCESS_INFORMATION processInformation;

	ProcessInformation() 
	{
		ZeroMemory(&info, sizeof(STARTUPINFO));
		info.cb = sizeof(STARTUPINFO);
	}
};

void sendMessage(int type)
{
	switch (type) 
	{
	case 1:
		SetEvent(event1);
		break;
	case 2:
		SetEvent(event2);
		break;
	case 3:
		SetEvent(event3);
		break;
	case 4:
		SetEvent(event4);
	}
}

DWORD WINAPI waitEvent1(LPVOID v)
{
	while (flag1) {
		WaitForSingleObject(parentEvent1, INFINITE);
		cout << "Получено сообщение 1 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitEvent2(LPVOID v) 
{
	while (flag2) {
		WaitForSingleObject(parentEvent2, INFINITE);
		cout << "Получено сообение 2 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitEvent11(LPVOID v)
{
	while (flag1) {
		WaitForSingleObject(parentEvent1, INFINITE);
		cout << "Получено сообщение 1 от процесса Child\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitEvent22(LPVOID v)
{
	while (flag2) {
		WaitForSingleObject(parentEvent2, INFINITE);
		cout << "Получено сообение 2 от процесса Child\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitParentEndEvent(LPVOID v) 
{
	while (flag3) {
		WaitForSingleObject(parentEndEvent, INFINITE);
		cout << "Процесс Parent завершил работу.\n";
		messageParentEndCount++;
	}
	return 0;
}

DWORD WINAPI waitChildEndEvent(LPVOID v)
{
	while (flag3) {
		WaitForSingleObject(parentEndEvent, INFINITE);
		cout << "Процесс Child завершил работу.\n";
		messageParentEndCount++;
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	//создаём события а б с д
	event1 = CreateEvent(NULL, FALSE, FALSE, "Event1");
	event2 = CreateEvent(NULL, FALSE, FALSE, "Event2");
	event3 = CreateEvent(NULL, FALSE, FALSE, "Event3");
	event4 = CreateEvent(NULL, FALSE, FALSE, "Event4");

	//два события-сообщения которые может прнять родитель по условию + событие для окончания
	parentEvent1 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent1");
	parentEvent2 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent2");
	parentEndEvent = CreateEvent(NULL, FALSE, FALSE, "ParentEndEvent");


	childEvent1 = CreateEvent(NULL, FALSE, FALSE, "ChildEvent1");
	childEvent2 = CreateEvent(NULL, FALSE, FALSE, "ChildEvent2");
	childEndEvent = CreateEvent(NULL, FALSE, FALSE, "ChildEndEvent");

	//семафор для синхронизации родителя и ребёнка
	semaphore = CreateSemaphore(NULL, 3, 3, "Semaphore");

	//запрашиваем колво процессов для каждого
	cout << "Введите кол-во процессов \"parent\" и \"child\":\n";
	int processCountParent;
	int processCountChild;
	cin >> processCountParent;
	cin >> processCountChild;
	int processCountSum = processCountParent + processCountChild;
	system("cls");

	//создаём массив процессов
	ProcessInformation** processInformation;
	processInformation = new ProcessInformation * [processCountSum];

	char lpszAppName1[] = "parent ";
	char lpszAppName2[] = "child ";

	for (int i = 0; i < processCountParent; i++)
	{
		processInformation[i] = new ProcessInformation();
		if (!CreateProcess(NULL, lpszAppName1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i]->info, &processInformation[i]->processInformation)) 
		{
			cout << "Parent not created\n";
		}
	}

	for (int j = processCountChild ; j < processCountSum; j++)
	{

		processInformation[j] = new ProcessInformation();
		if (!CreateProcess(NULL, lpszAppName2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[j]->info, &processInformation[j]->processInformation)) 
		{
			cout << "Parent not created\n";
		}
	}

	//получаем сообщения от процесса родитель
	HANDLE parentThread1;
	DWORD IDThread1;

	HANDLE parentThread2;
	DWORD IDThread2;

	HANDLE parentEndThread;
	DWORD IDThread3;

	parentThread1 = CreateThread(NULL, 0, waitEvent1, (void*)parentEvent1, 0, &IDThread1);
	parentThread2 = CreateThread(NULL, 0, waitEvent2, (void*)parentEvent2, 0, &IDThread2);

	parentEndThread = CreateThread(NULL, 0, waitParentEndEvent, (void*)parentEndEvent, 0, &IDThread3);

	HANDLE childThread1;
	DWORD IDThread11;

	HANDLE childThread2;
	DWORD IDThread22;

	HANDLE childEndThread;
	DWORD IDThread33;

	childThread1 = CreateThread(NULL, 0, waitEvent11, (void*)childEvent1, 0, &IDThread11);
	childThread2 = CreateThread(NULL, 0, waitEvent22, (void*)childEvent2, 0, &IDThread22);

	childEndThread = CreateThread(NULL, 0, waitChildEndEvent, (void*)childEndEvent, 0, &IDThread33);

	for (int i = 0; i < processCountSum; i++) 
	{
		cout << "Введите сообщение (1-4):\n";
		int message;
		cin >> message;

		sendMessage(message);
	}

	if (WaitForSingleObject(parentEndEvent,INFINITE)&& WaitForSingleObject(childEndEvent, INFINITE))
	{
		TerminateThread(parentThread1, 0);
		TerminateThread(parentThread2, 0);
		TerminateThread(parentEndThread, 0);

		CloseHandle(parentThread1);
		CloseHandle(parentThread2);
		CloseHandle(parentEndThread);

		TerminateThread(childThread1, 0);
		TerminateThread(childThread2, 0);
		TerminateThread(childEndThread, 0);

		CloseHandle(childThread1);
		CloseHandle(childThread2);
		CloseHandle(childEndThread);
	}



	system("pause");

	return 0;
}