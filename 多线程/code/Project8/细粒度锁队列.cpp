#include <iostream>
#include <mutex>
#include <thread>
#include <new>
#include <memory>
#include <vector>

using namespace std;

template <class T>
class threadsafe_queue
{
private:
	struct node
	{
		shared_ptr<T> data;
		unique_ptr<node> next;
	};
	//FIFO 先入先出
	mutex head_mutex;
	unique_ptr<node> head; // 表示整个队列的容量(链表)
	mutex tail_mutex;
	node* tail;

	node* get_tail()
	{
		lock_guard<mutex> tail_lock(tail_mutex);
		return tail;
	}
	unique_ptr<node> pop_head()
	{
		lock_guard<mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return nullptr;
		}
		unique_ptr<node> old_head = move(head);  // unique_ptr只能通过move()得到另一个的所有权
		head = move(old_head->next);
		return old_head;
	}
public:
	threadsafe_queue() : head(new node), tail(head.get()) {}
	threadsafe_queue(const threadsafe_queue& other) = delete;
	threadsafe_queue& operator=(const threadsafe_queue& other) = delete;
	// 入队
	void push(T new_value)
	{
		shared_ptr<T> new_data(make_shared<T>(move(new_value)));
		unique_ptr<node>  p(new node);
		node* const new_tail = p.get();
		lock_guard<mutex> tail_lock(tail_mutex);
		tail->data = new_data;
		tail->next = move(p);
		tail = new_tail;
	}
	// 出队
	shared_ptr<T> try_pop()
	{
		unique_ptr<node> old_head = pop_head();
		return old_head ? old_head->data : shared_ptr<T>();
	}
};

void insertData(threadsafe_queue<int>& test, int data)
{
	test.push(data);
}

void deleteData(threadsafe_queue<int>& test)
{
	shared_ptr<int> result = test.try_pop();
	if (result == nullptr)
	{
		printf("queue is empty!! ! \n");
	}
	else
	{
		printf("%d\n", *result);
	}
}

int main(void)
{
	vector<thread> t;
	threadsafe_queue<int> test;
	for (int i = 0; i < 5; i++)
	{
		t.push_back(thread(insertData, ref(test), i));
		t.push_back(thread(deleteData, ref(test)));

	}

	for (auto& v : t)
	{
		v.join();
	}

	return 0;
}