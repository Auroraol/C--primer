#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>
#include <list>

using namespace std;

//No.1 通过容器创建多个线程
void print(int index)
{
	//cout << "线程序号: " << index << endl;  // cout是重载输出有点乱,不好区分.换成printf()
	printf("线程序号: %d\n", index);
}

void testThread()
{
	vector<thread*> test;
	// 创建多个线程
	for (int i = 0; i < 10; i++)
	{
		test.push_back(new thread(print, i));
	}

	//汇合线程
	for (auto v : test)
	{
		v->join();
	}
	cout << "主线程\n";
}

//No.2数据共享问题分析
//2.1 只读数据:稳定安全，不需要特殊处理，直接读取即可
vector<int> g_data = {1,2,3};
void printTest(int num)
{
	printf("子线程序号:%d \n", num);
	for (auto v : g_data)
	{
		printf("%d\t", v);
	}
	cout << endl;
}

void visiteDate()
{
	vector<thread*> test;
	// 创建多个线程
	for (int i = 0; i < 10; i++)
	{
		test.push_back(new thread(printTest, i));
	}

	//汇合线程
	for (auto v : test)
	{
		v->join();
	}
	cout << "主线程\n";
}

//2.2 有读有写: 需要特殊处理,让程序只对数据进行单一操作
class Seaking
{
public:
	void makeFriend()  // 写
	{
		for (int i = 0; i < 100000; i++)
		{
			cout << "女朋友增加一个\n";
			mm.push_back(i);
		}
	}

	void breakUp()   // 读
	{

		for (int i = 0; i < 100000; i++)
		{
			if (!mm.empty())
			{
				cout << "女朋友减少一个\n";
				mm.pop_back();
			}
			else
			{
				cout << "单身状态\n";
			}

		}
	}
protected:
	list<int> mm;   //读写
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