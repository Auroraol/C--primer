#include <iostream>
#include <thread>
#include <windows.h>

using namespace std;

void print()
{
	cout << "�ɻ�\n";
}

void printDate()
{
	Sleep(1000);
	cout << "˯��\n";
}

int main(void)
{
	thread t1(printDate);
	t1.join();
	//t1.detach();
	print();
	return 0;
}