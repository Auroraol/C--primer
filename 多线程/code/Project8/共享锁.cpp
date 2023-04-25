#include <iostream>
#include <thread>
#include <list>
#include <cstdio>
#include <mutex>

using namespace std;

/*

No.1 ��һ�ּ�������:
mutex��(������)����mutex��Ķ���
	1.1ͨ������1ock�������м���
	1.2ͨ������un1ock�������н���
	ע��: lock��unlock����ɶԳ���

No.2 �ڶ��ּ�������:
	lock_guard ����ʵ�ּ���(�൱���Զ���)
	lock_guard��Ĺ��캯���е���1ock����
	lock_gurad�����������������unlock����
*/

class Seaking
{
public:
	void makeFriend()  // д
	{
		lock_guard<mutex> lgObject(m_mutex);
		for (int i = 0; i < 100000; i++)
		{
			//m_mutex.lock();  // ����
			printf("Ů��������һ��%d\n", i);
			mm.push_back(i);
			//m_mutex.unlock(); // ����
		}
	}

	void breakUp()   // ��
	{
		lock_guard<mutex> lgObject(m_mutex);
		for (int i = 0; i < 100000; i++)
		{
			if (!mm.empty())
			{
				//m_mutex.lock();  // ����
				printf("Ů���Ѽ���һ��%d\n", mm.back());
				mm.pop_back();
				//m_mutex.unlock(); // ����
			}
			else
			{
				printf("����״̬\n");
			}

		}
	}
protected:
	list<int> mm;   //��д
	mutex m_mutex;
};


int main(void)
{
	Seaking man;
	thread t1(&Seaking::makeFriend, &man);
	thread t2(&Seaking::breakUp, &man);
	t1.join();
	t2.join();
	return 0;
}