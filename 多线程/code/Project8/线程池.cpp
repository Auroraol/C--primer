#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <exception>

using namespace std;

class ThreadPool
{
public:
	ThreadPool(size_t size);
	~ThreadPool();
	// 泛型编程
	template <class F, class ...Args>
	auto enqueue(F&& f, Args&& ...args)->future<decltype(f(args...))>;    // future包装

protected:
	queue<function<void()>> tasks;   //任务列表
	vector<thread> workers;          //工作线程
	mutex queue_mutex;
	condition_variable condition;   // 条件变量 一般结合开关使用
	bool stop;  // 开关

};

ThreadPool::ThreadPool(size_t size) : stop(false)
{
	for (int i = 0; i < size; i++)
	{
		workers.emplace_back(

			[this]()
			{
				while (true)
				{
					function<void()> task;   // 包装
					{
						unique_lock<mutex> lock(this->queue_mutex);  // 加锁
						this->condition.wait(lock, [this]()  // 条件变量wait操作
							{
								return this->stop || !this->tasks.empty();
							}
						);

						// 防止死循环
						if (this->stop && this->tasks.empty())  // stop做了改变, 并且不为空
						{
							return;
						}

						// 出队操作
						task = move(this->tasks.front());
						this->tasks.pop();
					}
					task();
				}
			}
		);
	}

}

ThreadPool::~ThreadPool()
{
	// 第一部分 锁的处理
	{

		unique_lock<mutex> lock(queue_mutex);
		stop = true;
	}

	condition.notify_all();

	for (auto& t : workers)
	{
		t.join();
	}
}


// 第二部分 
template<class F, class ...Args>
auto ThreadPool::enqueue(F&& f, Args&& ...args)->future<decltype(f(args...))>
{
	using Type = decltype(f(args...));  // 重命名
	// 打包成无参的
	auto task = make_shared<packaged_task<Type()>>(bind(forward<F>(f), forward<Args>(args)...));
	future<Type> res = task->get_future();
	{
		unique_lock<mutex>(queue_mutex);
		if (stop)
		{
			throw runtime_error("停止....");
		}

		tasks.emplace(
			[task]()
			{
				(*task)();
			}
		);
	}

	// 唤醒线程
	condition.notify_one();

	return res;
}

int func(int i)
{
	printf("线程干活:%d \n", i);
	this_thread::sleep_for(1s);
	printf("线程结束:%d \n", i);
	return i * i;
}


int main(void)
{
	ThreadPool pool(4);
	vector <future<int>> results;

	for (int i = 0; i < 8; i++)
	{
		results.emplace_back(pool.enqueue(func, i));
	}

	for (auto&& t : results)
	{
		printf("结果:%d\n", t.get());   // get()获取值
	}

	return 0;
}