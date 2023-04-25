#include <iostream>
#include <thread>
#include <list>
#include <cstdio>
#include <mutex>

using namespace std;

/*

No.1 第一种加锁方法:
mutex类(互斥量)创建mutex类的对象
	1.1通过调用1ock函数进行加锁
	1.2通过调用un1ock函数进行解锁
	注意: lock和unlock必须成对出现

No.2 第二种加锁方法:
	lock_guard 对象实现加锁(相当于自动门)
	lock_guard类的构造函数中调用1ock函数
	lock_gurad类的析构函数调用了unlock函数
*/

class Seaking
{
public:
	void makeFriend()  // 写
	{
		lock_guard<mutex> lgObject(m_mutex);
		for (int i = 0; i < 100000; i++)
		{
			//m_mutex.lock();  // 加锁
			printf("女朋友增加一个%d\n", i);
			mm.push_back(i);
			//m_mutex.unlock(); // 解锁
		}
	}

	void breakUp()   // 读
	{
		lock_guard<mutex> lgObject(m_mutex);
		for (int i = 0; i < 100000; i++)
		{
			if (!mm.empty())
			{
				//m_mutex.lock();  // 加锁
				printf("女朋友减少一个%d\n", mm.back());
				mm.pop_back();
				//m_mutex.unlock(); // 解锁
			}
			else
			{
				printf("单身状态\n");
			}

		}
	}
protected:
	list<int> mm;   //读写
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