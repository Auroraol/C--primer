#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <list>

using namespace std;

class DoSomeThing
{
public:
	void wc()
	{
		for (int i = 0; i < 10000; i++)
		{
			//No.1 unique_lock��������
			// unique_lock<mutex> unique(mtx);
			//No.2 unique_lock��������
			//2.1 adopt_lock �Ƚ���lock����,��ʹ�õĻ�����������abort������ֹ����
			// mtx.lock();
			// unique_lock<mutex> unique(mtx, adopt_lock);
			//2.2 defer_lock, ��Ҫ�Լ��ֶ�lock��������һ��û��lock����
			//unique_lock<mutex> unique(mtx, defer_lock);
			//unique.lock();
			// 2.3 try_to_lock ���� owns_lock()�ж�
			unique_lock<mutex> unique(mtx, try_to_lock);
			cout << "�ϲ���!" << endl;
			if (unique.owns_lock())
			{
				num.push_back(i);
			}
		}
	}

	void bath()
	{
		for (int i = 0; i < 10000; i++)
		{
			if (!num.empty())
			{
				unique_lock<mutex>  unique(mtx);
				cout << "ϴ��!" << endl;
				num.pop_back();
			}
			else
			{
				cout << "��������" << endl;
			}
		}
	}

protected:
	list<int> num;
	mutex mtx;
};


int main(void)
{
	DoSomeThing people;
	thread t1(&DoSomeThing::wc, &people);
	thread t2(&DoSomeThing::bath, &people);
	t1.join();
	t2.join();
	return 0;
}