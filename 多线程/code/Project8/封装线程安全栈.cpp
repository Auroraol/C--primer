#include <iostream>
#include <thread>
#include <stack>
#include <vector>
#include <mutex>
#include <exception>  // 异常处理头文件


using namespace std;


struct empty_satck : public exception
{
	// 重写what方法
	const char* what() const noexcept
	{
		return "栈为空!!";
	}
};



// 封装线程安全栈

template <class T>
class threadSafe_stack
{
public:
	// 构造函数
	threadSafe_stack() {}
	threadSafe_stack(const threadSafe_stack& other)
	{
		lock_guard<mutex> lock(other.m);   // 互斥量用拷贝构成
		data = other.data;
	}
	// 删除赋值操作
	threadSafe_stack& operator=(const threadSafe_stack&) = delete;
	~threadSafe_stack() {}

	// 万金油函数
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


	// 存数据
	void push(T value)
	{
		lock_guard<mutex> lock(m);
		data.push(move(value));
	}

	// 取数据
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
	mutable mutex m;   //mutable可以修改的互斥量
	stack<T> data;
};


// 封装线程处理函数
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