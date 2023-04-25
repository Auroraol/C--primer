#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

// ����ȫ�Ķ���
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
		// ��detach() ����������
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

// ��װһ���̰߳�ȫ����
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
	// ��������
	thread_safe_queue(thread_safe_queue const& other)
	{
		// ����
		lock_guard<mutex> lk(other.mut);
		data_queue = other.data_queue;
	}

	// ¼����
	void push(T new_value)
	{
		//���� 
		lock_guard<mutex> lk(mut);
		data_queue.push(new_value);
		// ���ݴ洢��ɺ�����һ���߳�
		data_cond.notify_one();
	}

	// �ȴ��̴߳洢���ݽ���������һ�����ݵĳ���
	void wait_add_pop(T& value)
	{
		unique_lock<mutex> lk(mut);
		// �ȴ����ݲ�Ϊ��
		data_cond.wait(lk, [this](){return !data_queue.empty(); });

		// ����Ϊ�յ�ʱ�����������еĲ���
		value = data_queue.front();
		data_queue.pop();
	}

	// ����ָ��
	shared_ptr<T> wait_add_pop()
	{
		unique_lock<mutex> lk(mut);
		// �ȴ����ݲ�Ϊ��
		data_cond.wait(lk, [this](){return !data_queue.empty(); });

		// ����Ϊ�յ�ʱ�����������еĲ���
		shared_ptr<T> res(make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		// ����
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
		// ��detach() ����������
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
