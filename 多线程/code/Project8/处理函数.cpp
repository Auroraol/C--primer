#include <iostream>
#include <thread>
#include <ctime>   // 时间函数
#include <chrono>  // c++新标准的时间函数
#include <iomanip>  //io流格式控制

using namespace std;


// 第一个 获取线程id
void threadfun()
{
	//this_thread 命名空间
	cout << "子线程id = " << this_thread::get_id() << endl;
}

void test01()
{
	cout << "主线程id = " << this_thread::get_id() << endl;
	thread t1(threadfun);
	t1.join();
}


//第二个 延时函数 sleep_for //新标准的函数
void threadSleep_for()
{
	cout << "Sleep_for 子线程id = " << this_thread::get_id() << " 启动" << endl;
	this_thread::sleep_for(2s);  //2秒
	this_thread::sleep_for(chrono::seconds(2)); //2秒
	cout << "Sleep_for 子线程id = " << this_thread::get_id() << " 结束" << endl;
}

void test02()
{
	thread t1(threadSleep_for);
	t1.join();
}


// 第三个 让线程放弃执行,让操作系统调用另一个线程执行 yield
void threadYield(chrono::microseconds duration)  // 毫秒
{
	cout << "yield 子线程id = " << this_thread::get_id() << " 启动" << endl;
	auto startTime = chrono::high_resolution_clock::now();
	auto endTime = startTime + duration;
	do
	{
		this_thread::yield();
		cout << 1 << endl;

	} while (chrono::high_resolution_clock::now() < endTime);
	cout << "yield 子线程id = " << this_thread::get_id() << " 结束" << endl;
}

void test03()
{
	thread t1(threadYield, chrono::microseconds(200));
	t1.join();
}

//第四个 sleep_until
void threadSlee_until()
{
	//this_thread: :sleep_until(chrono: : system_clock::from_time_t(100)) ;
	time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm* ptm = new tm;
	localtime_s(ptm, &t); //获取本地时间
	if (ptm != nullptr)
	{
		this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm)));
	}
	cout << put_time(ptm, "%X") << endl;
}


int main(void)
{
	test01();
	test02();
	test03();
	threadSlee_until();

	return 0;
}