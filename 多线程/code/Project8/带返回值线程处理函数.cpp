#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std;
/*
带返回值的线程处理函数的方法:
第一步: 采用async: 启动一个异步任务,(创建线程,并且执行线程处理函数),返回future对象.
第二步: 通过future对象中的get()方法获取线程处理函数返回值. // 注意get()方法只能获取一次
*/

//N0.1 带返回值的普通函数充当线程处理函数
int testThreadOne()
{
	return 1001;
}

//No.2 带返回值的类成员函数充当线程处理函数
class MM
{
public:
	int mmthread(int num)
	{
		cout << "MM对象的id: " << this_thread::get_id() << endl;
		num *= 2;
		chrono::milliseconds duration(5000);
		this_thread::sleep_for(duration);
		return num;
	}
};

void testAsync()
{
	future<int> result = async(testThreadOne);
	cout << result.get() << endl;

	MM mm;
	auto temp = async(&MM::mmthread, &mm, 1999);
	cout << temp.get() << endl;

	//async其他参数
	//launch::async    : 创建线程, 执行线程处理函数(默认)
	//launch::deferred : 线程处理函数延迟到我们调用wait和get方法时候才会执行.本质是没有创建线程的
	auto testDerred1 = async(launch::deferred, &MM::mmthread, &mm, 1000);
	cout << testDerred1.get() << endl;  //才创建线程
	auto testDerred2 = async(launch::async, &MM::mmthread, &mm, 1000);

}

//No.3用thread处理带返回值的线程处理函数
//第一步: 通过类模板packaged_task包装线程处理函数
//第二步: 通过packaged_task的对象调用get_future获取future对象，再通过get()方法得到子线程处理函数的返回值
void testPackaged_task()
{
	//No.1打包带返回值的普通函数充当线程处理函数
	packaged_task<int(void)> taskOne(testThreadOne);
	thread testOne(ref(taskOne));
	testOne.join();
	cout << taskOne.get_future().get() << endl;

	//No.2 打包带返回值的类成员函数充当线程处理函数
	MM mm;
	packaged_task<int(int)> taskTwo(bind(&MM::mmthread, &mm, placeholders::_1)); //placeholders::_1占位符
	thread testTwo(ref(taskTwo), 20);
	testTwo.join();
	cout << taskTwo.get_future().get() << endl;

	//No.3打包Lambda表达式
	packaged_task<int(int)> taskThree([](int num){

		cout << "thread_id: " << this_thread::get_id() << endl;
		num *= 10;
		return num;
		});
	thread testTree(ref(taskThree), 7);
	testTree.join();
	cout << taskThree.get_future().get() << endl;

}


//No.4 promise获取线程处理函数返回值
//第一步: 通过promise类模板构建对象,通过调用set_value是存储函数需要返回的值
//第二步: 通过get_future获取future对象，再通过get()方法获取线程处理中的值
void testPromiseThread(promise<int>& temp, int data)
{
	cout << "thread_id" << this_thread::get_id() << endl;
	data *= 3;
	temp.set_value(data);
}
void testPromise()
{
	promise<int> temp;
	thread testp(testPromiseThread, ref(temp), 18);
	testp.join();
	cout << temp.get_future().get() << endl;
}

int main(void)
{
	//testAsync();
	//testPackaged_task();
	testPromise();
	return 0;
}