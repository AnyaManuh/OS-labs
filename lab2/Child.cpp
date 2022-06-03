#include <windows.h>
#include <iostream>

using namespace std;
int findSymbolCount(char* arr, char symbol, int size)
{
	int count = 0;

	for (int i = 0; i < size; i++)
	{
		if (symbol == arr[i])
		{
			count++;
		}
	}

	return count;
}

int Main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	int size = atoi(argv[1]);
	char symbol = *argv[2];
	char* arr = argv[3];
	char* newArr = new char[size];

	int tmp = findSymbolCount(arr, symbol, size);
	for (int i = 0; i < size; i++)
	{
		newArr[i] = symbol;
	}

	for (int i = 0; i < size; i++)
	{
		if (arr[i] != symbol)
		{
			newArr[tmp++] = arr[i];
		}
	}

	for (int i = 0; i < size; i++)
	{
		cout << newArr[i] << " ";
	}
	cout << endl;
	system("pause");

	return 1;
}