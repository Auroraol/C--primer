#include <iostream>
#include <thread>
#include <stack>
#include <vector>
#include <mutex>
#include <exception>  // �쳣����ͷ�ļ�


using namespace std;


struct empty_satck : public exception
{
	// ��дwhat����
	const char* what() const noexcept
	{
		return "ջΪ��!!";
	}
};



// ��װ�̰߳�ȫջ

template <class T>
class threadSafe_stack
{
public:
	// ���캯��
	threadSafe_stack() {}
	threadSafe_stack(const threadSafe_stack& other)
	{
		lock_guard<mutex> lock(other.m);   // �������ÿ�������
		data = other.data;
	}
	// ɾ����ֵ����
	threadSafe_stack& operator=(const threadSafe_stack&) = delete;
	~threadSafe_stack() {}

	// ����ͺ���
	bool empty() const
	{
		lock_guard<mutex> lock(m);
		return data.empty();
	}

	size_t size() const
	{
		lock_guard<mutex> lock(m);
		return data.size();
	}


	// ������
	void push(T value)
	{
		lock_guard<mutex> lock(m);
		data.push(move(value));
	}

	// ȡ����
	void pop(T& value)
	{
		lock_guard<mutex> lock(m);
		if (data.empty())
		{
			throw empty_satck();
		}
		value = move(data.top());
		data.pop();
	}

private:
	mutable mutex m;   //mutable�����޸ĵĻ�����
	stack<T> data;
};


// ��װ�̴߳�����
void insertData(threadSafe_stack<int>& stack, int value)
{
	stack.push(value);
}

void deleteData(threadSafe_stack < int> stack)
{
	try
	{
		int value = 0;
		stack.pop(value);
		printf("data: %d\n", value);

	} catch (empty_satck re)
	{
		printf("%s\n", re.what());
	}
}

int main(void)
{
	vector<thread> t;
	threadSafe_stack<int> test;
	for (int i = 0; i < 5; i++)
	{
		t.push_back(thread(insertData, ref(test), i));
		t.push_back(thread(deleteData, ref(test)));
	}
	for (auto& v : t)
	{
		v.join();
	}

	cout << "size: " << test.size() << endl;
	return 0;
}