#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

volatile bool flag = true;

void getMessage(int num) 
{
	switch (num) {
	case 1:
		cout << "Получено сообщение 1 от Boss\n";
		break;
	case 2:
		cout << "Получено сообщение 2 от Boss\n";
		break;
	case 3:
		cout << "Получено сообщение 3 от Boss\n";
		break;
	case 4:
		cout << "Получено сообщение 4 от Boss\n";
		break;

	}
}

void sendMessage(int type, HANDLE childEvent1, HANDLE childEvent2)
{
	switch (type)
	{
	case 1:
		SetEvent(childEvent1);
		break;
	case 2:
		SetEvent(childEvent2);
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	cout << "Child process\n";

	HANDLE* handleArray = new HANDLE[7];
	handleArray[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event1");
	handleArray[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event2");
	handleArray[2] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event3");
	handleArray[3] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event4");

	HANDLE childEvent1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ChildEvent1");
	HANDLE childEvent2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ChildEvent2");
	HANDLE childEndEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ChildEndEvent");

	HANDLE semaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Semaphore");

	cout << "Не активен\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	cout << "Child process\n";
	cout << "Активен\n";

	int pos = WaitForMultipleObjects(4, handleArray, FALSE, INFINITE);
	getMessage(pos + 1);

	cout << "Введите сообщение (1-2):\n";
	int message;
	cin >> message;

	sendMessage(message, childEvent1, childEvent2);

	SetEvent(childEndEvent);

	ReleaseSemaphore(semaphore, 1, NULL);
	system("pause");
	return 1;
}