#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std;


/*
condition_variable类
	1.调用wait函数阻塞线程
	2.unique_lock加锁线程
	3.notify_one唤醒线程  notify_all唤醒所有线程
*/

condition_variable cv;
mutex cv_m;  //互斥量
int i = 0;    // 描述的唤醒条件
bool done = false;  //充当开关

void waits()
{
	unique_lock<mutex> lk(cv_m);  // 加锁
	cout << "等待中...\n";
	cv.wait(lk, []{return i == 1; });  //调用wait函数阻塞线程  参数1:锁 参数2:条件
	cout << "运行结束...\n";
	done = true;
}


void singnals()
{
	this_thread::sleep_for(chrono::seconds(2)); // 延时操作
	cout << "不做条件变量唤醒操作" << endl;   // 目的是查看是否有效, 结果是无效的
	cv.notify_one();

	//
	unique_lock<mutex> lk(cv_m);
	i = 1;
	while (!done)
	{
		cout << "条件变量满足唤醒操作" << endl;
		lk.unlock();
		cv.notify_one();
		this_thread::sleep_for(1s);
		lk.lock();
	}
}

void test01()
{
	thread t1(waits);
	thread t2(singnals);
	t1.join();
	t2.join();
}


void waits_thread(int id)
{
	unique_lock<mutex> lk(cv_m);  // 加锁
	cout << id << " 等待中...\n";
	cv.wait(lk, []{return i == 1; });  //调用wait函数阻塞线程  参数1:锁 参数2:条件
	cout << id << " 运行结束...\n";
	done = true;
}

void singnalsAll()
{
	this_thread::sleep_for(1s);
	{

		unique_lock<mutex> lk(cv_m);
		cout << "无条件变量改变的唤醒" << endl;
		cv.notify_all();
	}

	this_thread::sleep_for(1s);
	{
		unique_lock<mutex> lk(cv_m);
		cout << "带条件变量改变的唤醒操作" << endl;
		i = 1;
		cv.notify_all();
	}

}

void test02()
{
	thread t1(waits_thread, 1), t2(waits_thread, 2), t3(waits_thread, 3);
	thread t4(singnalsAll);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
}


int main(void)
{
	//test01();
	test02();
	return 0;
}