#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

// 不安全的队列
#if 0
void insertDate(queue<int>& data, int num)
{
	data.push(num);
}

void testQuque()
{
	queue<int> data;
	for (int i = 0; i < 10; i++)
	{
		thread t(insertDate, ref(data), i);
		t.detach();
		// 用detach() 可能有问题
	}

	this_thread::sleep_for(1s);
	while (!data.empty())
	{
		cout << data.front() << " ";
		data.pop();
	}
	cout << endl;
}
#endif

// 封装一个线程安全队列
template <class T>
class thread_safe_queue
{
protected:
	mutable mutex mut;
	queue<T> data_queue;
	condition_variable data_cond;
public:
	thread_safe_queue()
	{}
	// 拷贝构造
	thread_safe_queue(thread_safe_queue const& other)
	{
		// 加锁
		lock_guard<mutex> lk(other.mut);
		data_queue = other.data_queue;
	}

	// 录数据
	void push(T new_value)
	{
		//加锁 
		lock_guard<mutex> lk(mut);
		data_queue.push(new_value);
		// 数据存储完成后唤醒另一个线程
		data_cond.notify_one();
	}

	// 等待线程存储数据结束后在做一个数据的出队
	void wait_add_pop(T& value)
	{
		unique_lock<mutex> lk(mut);
		// 等待数据不为空
		data_cond.wait(lk, [this](){return !data_queue.empty(); });

		// 当不为空的时候在做出队列的操作
		value = data_queue.front();
		data_queue.pop();
	}

	// 智能指针
	shared_ptr<T> wait_add_pop()
	{
		unique_lock<mutex> lk(mut);
		// 等待数据不为空
		data_cond.wait(lk, [this](){return !data_queue.empty(); });

		// 当不为空的时候在做出队列的操作
		shared_ptr<T> res(make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		// 加锁
		lock_guard<mutex> lk(mut);
		return data_queue.empty();
	}
};

void insertDate(thread_safe_queue<int>& data, int num)
{
	data.push(num);
}

void threadSafeQueue()
{
	thread_safe_queue<int> data;
	for (int i = 0; i < 10; i++)
	{
		thread t(insertDate, ref(data), i);
		t.detach();
		// 用detach() 可能有问题
	}

	this_thread::sleep_for(1s);
	int num = 0;
	while (!data.empty())
	{
		data.wait_add_pop(num);
		cout << num << " ";
	}
	cout << endl;
}

int main(void)
{
	threadSafeQueue();
	return 0;
}
