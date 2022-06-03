#include<iostream>
#include<windows.h>
#include <process.h>
using namespace std;

struct massive
{
	int size;
	int* mas;

	massive(int n, int* m)
	{
		size = n;
		mas = new int[size];
		for (int i = 0; i <size; i++)
		{
			mas[i] = m[i];
		}
	}

	~massive()
	{
		delete mas;
	}
};

DWORD WINAPI worker(LPVOID m)
{
	massive* mas = (massive*)m;
	cout << "Enter new element: " << endl;
	int x;
	cin >> x;
	cout << endl;
	for (int i = 0; i < mas->size; i++)
	{
		if (mas->mas[i] < x)
			cout << mas->mas[i] << " ";
	}
	cout << endl;
	cout << endl;

	return 0;

}

DWORD WINAPI Fibonnaci(LPVOID v)
{
	int num1 = 1;
	int num2 = 1;

	do
	{
		cout << num2 << " ";

		int tmp = num2;
		num2 += num1;
		num1 = tmp;

		Sleep(200);
	} while (num2>0);

	return 0;
}

unsigned __stdcall worker2(void* m)
{
	cout << "Hello. I've created this thread with _beginthreadex dunction. ";
	cout << endl;
	return 0;
}


int main()
{
	int size;
	cout << "Enter mass size: " << endl;
	cin >> size;
	int* mas = new int[size];
	cout << "Enter elements: " << endl;
	for (int i = 0; i < size; i++)
	{
		cin >> mas[i];
		cout << endl;
	}

	for (int i = 0; i < size; i++)
	{
		cout << mas[i]<<" ";
	}
	cout << endl;

	double stoptime;

	cout << "Enter time to stop the thread: " << endl;
	cin >> stoptime;

	HANDLE Worker;
	DWORD IDThread;

	Worker = CreateThread(NULL, 0, worker,(void*)new massive(size, mas), 0, &IDThread);

	SuspendThread(Worker);
//	Sleep(stoptime);
	ResumeThread(Worker);
	WaitForSingleObject(Worker, INFINITE);
	CloseHandle(Worker);
	delete mas;




	/*Параметр dwCreationFlags определяет, в каком состоянии будет создан поток.Если значение этого
	параметра равно 0, то функция потока начинает выполняться сразу после создания потока.Если же значение
		этого параметра равно (предпоследний)CREATE_SUSPENDED, то поток создается в подвешенном состоянии.В дальнейшем
		этот поток можно запустить вызовом функции ResumeThread.*/

	HANDLE Count;
	DWORD IDThread2;

	cout << endl;
	Count = CreateThread(NULL, 0, Fibonnaci, NULL, CREATE_SUSPENDED, &IDThread2);
	

	//SuspendThread(Count);
	ResumeThread(Count);
	cout << endl; cout << endl;


	cout << endl;
	cout << endl;
	HANDLE h1Thread;
	unsigned int threadId;

	h1Thread = (HANDLE)_beginthreadex(NULL, NULL, &worker2, NULL, 0, &threadId);
	CloseHandle(h1Thread);
	cout << endl;
;

	system("pause");

	return 0;
}


