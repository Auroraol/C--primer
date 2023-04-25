#include <iostream>
#include <thread>
#include <deque>
#include <condition_variable>
#include <mutex>

using namespace std;

const int KProduceItems = 20;  // 产品个数
const int KPrositorySize = 5;   // 仓库大小

// 仓库类
template <class T>
class Repository
{
public:
	deque<T> items_buff;	//存储产品

	mutex mtx;				//生产者和消费者的互斥量
	mutex produce_mutex;	//生产计数互斥量
	mutex consume_mutex;	//消费计数互斥量

	condition_variable repo_not_full;     //仓库不满状态条件变量
	condition_variable repo_not_empty;    //仓库不空的条件

	size_t produce_item_count;  // 生产者记数
	size_t consume_item_count;  // 消费者记数

	size_t produce_position;     //  生成者个数
	size_t consume_position;     //  消费者个数

	Repository()
	{
		produce_item_count = 0;
		consume_item_count = 0;

		produce_position = 0;
		consume_position = 0;
	}

};


// 工厂类
template <class T>
class Factory
{

protected:
	Repository<T> repo;   // 创建仓库对象
	//产品放到仓库
	void ProductItem(Repository<T>& repo, T item)
	{
		unique_lock<mutex> lock(repo.mtx);  // 加锁
		repo.items_buff.push_back(item);

		if (repo.produce_position == KPrositorySize)
		{
			repo.produce_position = 0;   // 满了以后生产者=0
		}
		// 不空的时候唤醒别的线程
		repo.repo_not_empty.notify_all();
		lock.unlock(); //可以不做
	}

	//从创库取产品
	T ConsuneItem(Repository<T>& repo)
	{
		unique_lock<mutex> lock(repo.mtx);  // 加锁 
		while (repo.items_buff.empty())
		{
			cout << "无货源,等待..." << endl;
			repo.repo_not_empty.wait(lock);   // 空就等待
		}
		// 有货源之后进行
		T data = repo.items_buff.front();   // 取出数据
		repo.items_buff.pop_front();
		// 判断是否不满
		if (repo.consume_position == KPrositorySize)
		{
			repo.consume_position = 0;  // 满了以后消费者=0
		}
		// 不满的时候唤醒线程
		repo.repo_not_full.notify_all();
		lock.unlock();
		return data;
	}


public:
	//生产者操作
	void ProduceTask()
	{
		bool ready_to_exit = false;  // 条件变量
		while (true)
		{
			unique_lock<mutex> lock(repo.produce_mutex);  //加锁
			//线程结束条件
			if (repo.produce_item_count < KProduceItems)
			{
				repo.produce_item_count++;

				//生产产品的代码块
				this_thread::sleep_for(1s);  // 实际开发不用
				T item = repo.produce_item_count;
				//cout << "生产者id: " << this_thread::get_id << "货源编号: [" << item << "]" << endl;
				printf("生产者id:%d\t货源编号:[%d]\n", this_thread::get_id(), item);

				ProductItem(repo, item);
			}
			else
			{
				ready_to_exit = true;
			}
			lock.unlock();

			if (ready_to_exit)  // 跳出生产
			{
				break;
			}

		}

	}

	// 消费者操作
	void consumeTask()
	{
		bool ready_to_exit = false;  // 条件变量
		while (true)
		{
			unique_lock<mutex> lock(repo.consume_mutex);
			if (repo.consume_item_count < KProduceItems)
			{
				T item = ConsuneItem(repo);
				//消费商品代码
				this_thread::sleep_for(1s);
				//cout << "消费者id: " << this_thread::get_id() << "货源编号: [" << item << "]" << endl;
				printf("消费者id:%d\t货源编号:[%d]\n", this_thread::get_id(), item);
				repo.consume_item_count++;
			}
			else
			{
				ready_to_exit = true;
			}
			lock.unlock();

			if (ready_to_exit)  // 跳出生产
			{
				break;
			}
		}
	}

};


int main(void)
{
	cout << "主函数线程id: " << this_thread::get_id() << endl;

	Factory<int>myFactory;
	thread produce1(&Factory<int>::ProduceTask, &myFactory);
	thread produce2(&Factory<int>::ProduceTask, &myFactory);

	thread consumer1(&Factory<int>::consumeTask, &myFactory);
	thread consumer2(&Factory<int>::consumeTask, &myFactory);
	thread consumer3(&Factory<int>::consumeTask, &myFactory);
	produce1.join();
	produce2.join();
	consumer1.join();
	consumer2.join();
	consumer3.join();

	return 0;
}