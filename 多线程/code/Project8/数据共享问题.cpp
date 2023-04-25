#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>
#include <list>

using namespace std;

//No.1 ͨ��������������߳�
void print(int index)
{
	//cout << "�߳����: " << index << endl;  // cout����������е���,��������.����printf()
	printf("�߳����: %d\n", index);
}

void testThread()
{
	vector<thread*> test;
	// ��������߳�
	for (int i = 0; i < 10; i++)
	{
		test.push_back(new thread(print, i));
	}

	//����߳�
	for (auto v : test)
	{
		v->join();
	}
	cout << "���߳�\n";
}

//No.2���ݹ����������
//2.1 ֻ������:�ȶ���ȫ������Ҫ���⴦��ֱ�Ӷ�ȡ����
vector<int> g_data = {1,2,3};
void printTest(int num)
{
	printf("���߳����:%d \n", num);
	for (auto v : g_data)
	{
		printf("%d\t", v);
	}
	cout << endl;
}

void visiteDate()
{
	vector<thread*> test;
	// ��������߳�
	for (int i = 0; i < 10; i++)
	{
		test.push_back(new thread(printTest, i));
	}

	//����߳�
	for (auto v : test)
	{
		v->join();
	}
	cout << "���߳�\n";
}

//2.2 �ж���д: ��Ҫ���⴦��,�ó���ֻ�����ݽ��е�һ����
class Seaking
{
public:
	void makeFriend()  // д
	{
		for (int i = 0; i < 100000; i++)
		{
			cout << "Ů��������һ��\n";
			mm.push_back(i);
		}
	}

	void breakUp()   // ��
	{

		for (int i = 0; i < 100000; i++)
		{
			if (!mm.empty())
			{
				cout << "Ů���Ѽ���һ��\n";
				mm.pop_back();
			}
			else
			{
				cout << "����״̬\n";
			}

		}
	}
protected:
	list<int> mm;   //��д
};

void testSeaking()
{
	Seaking seaman;
	thread t1(&Seaking::makeFriend, seaman);
	thread t2(&Seaking::breakUp, seaman);
	t1.join();
	t2.join();
}



int main(void)
{
	//testThread();
	//visiteDate();
	testSeaking();
	return 0;
}