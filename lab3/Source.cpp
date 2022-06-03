#include <windows.h>
#include <iostream>

using namespace std;

int number;
volatile char x;
volatile char* arr;
volatile int size_;


CRITICAL_SECTION cs;
HANDLE countElementEvent;

int findSymbolCount(char* a, char symbol, int size_)
{
	int count = 0;

	for (int i = 0; i < size_; i++)
	{
		if (symbol == a[i])
		{
			count++;
		}
	}

	return count;
}

DWORD WINAPI work(LPVOID v)
{
	EnterCriticalSection(&cs);
	cout << "Введите задержку:\n";
	int t;
	cin >> t;

	char* newArr = new char[size_];
	char* tmpArr = new char[size_];

	for (int i = 0; i < size_; i++)
	{
		tmpArr[i] = arr[i];
	}

	int tmp = findSymbolCount(tmpArr, x, size_);
	for (int i = 0; i < tmp; i++)
	{
		newArr[i] = x;
		Sleep(t);
	}

	for (int i = 0; i < size_; i++)
	{
		if (arr[i] != x)
		{
			newArr[tmp++] = arr[i];
			Sleep(t);
		}
	}

	cout << "Итоговый массив:" << endl;
	for (int i = 0; i < size_; i++)
	{
		arr[i] = newArr[i];
		cout << arr[i] << " ";
	}
	cout << endl;

	LeaveCriticalSection(&cs);
	delete tmpArr;
	delete newArr;
	return 0;
}

DWORD WINAPI countElement(LPVOID v)
{
	EnterCriticalSection(&cs);
	int c = 0;
	for (int i = 0; i < size_ && arr[i] == x; i++)
	{
		c++;
	}
	number = c;
	LeaveCriticalSection(&cs);
	SetEvent(countElementEvent);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Введите размер массива:\n";
	int s;
	cin >> s;
	size_ = s;

	arr = new char[s];
	cout << "Введите " << size_ << " элементов:\n";
	for (int i = 0; i < size_; i++)
	{
		char a;
		cin >> a;
		arr[i] = a;
	}

	system("cls");

	cout << "Размерность массива: " << size_ << endl;
	cout << "Элементы массива:" << endl;
	for (int i = 0; i < size_; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;

	//инициализируем критическую секцию и используем при работе с х так как он 
	//используется в нескольких потоках
	InitializeCriticalSection(&cs);
	
	EnterCriticalSection(&cs);
	cout << "Введите символ Х:\n";
	char ch;
	cin >> ch;
	x = ch;
	LeaveCriticalSection(&cs);

	HANDLE workThread;
	DWORD IDThread1;

	workThread = CreateThread(NULL, 0, work, NULL, CREATE_SUSPENDED, &IDThread1);
	ResumeThread(workThread);

	

	HANDLE countElementThread;
	DWORD IDThread2;

	//создаём событие, которое переход в сигнальное состояние в потоке countelement
	countElementEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	countElementThread = CreateThread(NULL, 0, countElement, NULL, CREATE_SUSPENDED, &IDThread2);
	ResumeThread(countElementThread);
	//ждём сигнала события
	WaitForSingleObject(countElementEvent, INFINITE);

	cout << "Кол-во элементов =" << x << " равно :\n" << number << endl;
	CloseHandle(workThread);
	CloseHandle(countElementThread);
	CloseHandle(countElementEvent);
	delete arr;

	system("pause");
	return 0;
}