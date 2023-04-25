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
			//No.1 unique_lock加锁过程
			// unique_lock<mutex> unique(mtx);
			//No.2 unique_lock其他参数
			//2.1 adopt_lock 先进行lock操作,不使用的话，程序会调用abort函数终止程序
			// mtx.lock();
			// unique_lock<mutex> unique(mtx, adopt_lock);
			//2.2 defer_lock, 需要自己手动lock，创建是一个没有lock的锁
			//unique_lock<mutex> unique(mtx, defer_lock);
			//unique.lock();
			// 2.3 try_to_lock 调用 owns_lock()判定
			unique_lock<mutex> unique(mtx, try_to_lock);
			cout << "上厕所!" << endl;
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
				cout << "洗澡!" << endl;
				num.pop_back();
			}
			else
			{
				cout << "正在做事" << endl;
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