#include <iostream>
#include <thread>
#include <ctime>   // ʱ�亯��
#include <chrono>  // c++�±�׼��ʱ�亯��
#include <iomanip>  //io����ʽ����

using namespace std;


// ��һ�� ��ȡ�߳�id
void threadfun()
{
	//this_thread �����ռ�
	cout << "���߳�id = " << this_thread::get_id() << endl;
}

void test01()
{
	cout << "���߳�id = " << this_thread::get_id() << endl;
	thread t1(threadfun);
	t1.join();
}


//�ڶ��� ��ʱ���� sleep_for //�±�׼�ĺ���
void threadSleep_for()
{
	cout << "Sleep_for ���߳�id = " << this_thread::get_id() << " ����" << endl;
	this_thread::sleep_for(2s);  //2��
	this_thread::sleep_for(chrono::seconds(2)); //2��
	cout << "Sleep_for ���߳�id = " << this_thread::get_id() << " ����" << endl;
}

void test02()
{
	thread t1(threadSleep_for);
	t1.join();
}


// ������ ���̷߳���ִ��,�ò���ϵͳ������һ���߳�ִ�� yield
void threadYield(chrono::microseconds duration)  // ����
{
	cout << "yield ���߳�id = " << this_thread::get_id() << " ����" << endl;
	auto startTime = chrono::high_resolution_clock::now();
	auto endTime = startTime + duration;
	do
	{
		this_thread::yield();
		cout << 1 << endl;

	} while (chrono::high_resolution_clock::now() < endTime);
	cout << "yield ���߳�id = " << this_thread::get_id() << " ����" << endl;
}

void test03()
{
	thread t1(threadYield, chrono::microseconds(200));
	t1.join();
}

//���ĸ� sleep_until
void threadSlee_until()
{
	//this_thread: :sleep_until(chrono: : system_clock::from_time_t(100)) ;
	time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm* ptm = new tm;
	localtime_s(ptm, &t); //��ȡ����ʱ��
	if (ptm != nullptr)
	{
		this_thread::sleep_until(chrono::system_clock::from_time_t(mktime(ptm)));
	}
	cout << put_time(ptm, "%X") << endl;
}


int main(void)
{
	test01();
	test02();
	test03();
	threadSlee_until();

	return 0;
}