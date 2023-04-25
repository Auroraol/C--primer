#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

/*
int value = 0;

void testFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		value++;
	}
}


int main(void)
{
	thread t1(testFunc);
	thread t2(testFunc);
	t1.join();
	t2.join();

	cout << value << endl;
	return 0;
}

*/


/*
	原子操作: 对变量的读写操作不可分隔
	atomic: 模板类
	atomic<int> foo(0);
	//拷贝构造和赋值运算被删除了atomic( const atomic&) = delete;  atomic& operator=( const atomic&) = delete;

*/



int value = 0;
atomic<int> a_value = 0;
atomic <int> t_value = 0;

void testFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		value++;
		a_value++;   // += 是符合的而=是不行的
		//t_value = t_value + 1;  // 不满足会报错
		if (t_value != 0)
		{
			t_value = 1;
		}

	}
}


int main(void)
{
	thread t1(testFunc);
	thread t2(testFunc);
	t1.join();
	t2.join();
	cout << value << endl;
	cout << a_value << endl;

	return 0;
}