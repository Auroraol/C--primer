#include <iostream>
#include <thread>

using namespace std;

//第一种情况,无参普通函数充当线程处理函数
void print()
{
	cout << "无参普通函数充当线程处理函数\n";
}

void test01()
{
	thread t1(print);
	t1.join();
}

//第二种情况,采用lamba表达式充当线程处理函数
void test02()
{
	thread t1([](){ cout << "amba表达式充当线程处理函数\n"; });
	t1.join();
}

//第三种,带参数创建线程(普通参数和只能指针当作参数)
//3.1 传普通参数
void printDate(int num)
{
	cout << "id:" << num << endl;
}


void test03()
{
	int num = 1;
	thread t1(printDate, num);
	t1.join();
}

//3.1.1 传引用
void printRefrence(int& num)
{
	num = 1001;
	cout << "子线程" << num << endl;
}

void test04()
{
	int num = 0;
	thread t1(printRefrence, ref(num));   // ref: 包装引用作为传递的值
	t1.join();
	cout << "主进程" << num << endl;
}

//3.1.2 指针当作参数
void printPtr(unique_ptr<int> ptr)
{
	cout << "智能指针:" << ptr.get() << endl;
}

void test05()
{
	unique_ptr<int> ptr(new int(100));
	cout << ptr.get() << endl;

	thread t1(printPtr, move(ptr));    // move
	t1.join();

	cout << "test05:" << ptr.get() << endl;   // 已经没有所有权
}


// 第四种,通过类中的成员函数去实现
// 4.1 仿函数的形式:类名的方式调用
class function
{
public:
	void operator()()
	{
		cout << "重载()\n";
	}
};

void test06()
{
	//第一种,对象
	function object;
	thread t1(object);
	t1.join();
	//第二种,匿名对象
	thread t2((function()));
	t2.join();
}

//4.2 普通类中的成员函数
class MM
{
public:
	void print(int& num)
	{
		cout << "id" << num << endl;
	}
	MM();
	~MM();

private:

};

MM::MM()
{}

MM::~MM()
{}

void test07()
{
	MM m;
	int id = 1;
	thread t1(&MM::print, m, ref(id));  // 创建方式不太一样
	t1.join();
}


int main(void)
{
	test01();
	test02();
	test03();
	test04();
	test05();
	test06();
	test07();
	return 0;
}