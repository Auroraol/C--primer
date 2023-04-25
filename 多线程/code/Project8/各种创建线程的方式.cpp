#include <iostream>
#include <thread>

using namespace std;

//��һ�����,�޲���ͨ�����䵱�̴߳�����
void print()
{
	cout << "�޲���ͨ�����䵱�̴߳�����\n";
}

void test01()
{
	thread t1(print);
	t1.join();
}

//�ڶ������,����lamba���ʽ�䵱�̴߳�����
void test02()
{
	thread t1([](){ cout << "amba���ʽ�䵱�̴߳�����\n"; });
	t1.join();
}

//������,�����������߳�(��ͨ������ֻ��ָ�뵱������)
//3.1 ����ͨ����
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

//3.1.1 ������
void printRefrence(int& num)
{
	num = 1001;
	cout << "���߳�" << num << endl;
}

void test04()
{
	int num = 0;
	thread t1(printRefrence, ref(num));   // ref: ��װ������Ϊ���ݵ�ֵ
	t1.join();
	cout << "������" << num << endl;
}

//3.1.2 ָ�뵱������
void printPtr(unique_ptr<int> ptr)
{
	cout << "����ָ��:" << ptr.get() << endl;
}

void test05()
{
	unique_ptr<int> ptr(new int(100));
	cout << ptr.get() << endl;

	thread t1(printPtr, move(ptr));    // move
	t1.join();

	cout << "test05:" << ptr.get() << endl;   // �Ѿ�û������Ȩ
}


// ������,ͨ�����еĳ�Ա����ȥʵ��
// 4.1 �º�������ʽ:�����ķ�ʽ����
class function
{
public:
	void operator()()
	{
		cout << "����()\n";
	}
};

void test06()
{
	//��һ��,����
	function object;
	thread t1(object);
	t1.join();
	//�ڶ���,��������
	thread t2((function()));
	t2.join();
}

//4.2 ��ͨ���еĳ�Ա����
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
	thread t1(&MM::print, m, ref(id));  // ������ʽ��̫һ��
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