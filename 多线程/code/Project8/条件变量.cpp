#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std;


/*
condition_variable��
	1.����wait���������߳�
	2.unique_lock�����߳�
	3.notify_one�����߳�  notify_all���������߳�
*/

condition_variable cv;
mutex cv_m;  //������
int i = 0;    // �����Ļ�������
bool done = false;  //�䵱����

void waits()
{
	unique_lock<mutex> lk(cv_m);  // ����
	cout << "�ȴ���...\n";
	cv.wait(lk, []{return i == 1; });  //����wait���������߳�  ����1:�� ����2:����
	cout << "���н���...\n";
	done = true;
}


void singnals()
{
	this_thread::sleep_for(chrono::seconds(2)); // ��ʱ����
	cout << "���������������Ѳ���" << endl;   // Ŀ���ǲ鿴�Ƿ���Ч, �������Ч��
	cv.notify_one();

	//
	unique_lock<mutex> lk(cv_m);
	i = 1;
	while (!done)
	{
		cout << "�����������㻽�Ѳ���" << endl;
		lk.unlock();
		cv.notify_one();
		this_thread::sleep_for(1s);
		lk.lock();
	}
}

void test01()
{
	thread t1(waits);
	thread t2(singnals);
	t1.join();
	t2.join();
}


void waits_thread(int id)
{
	unique_lock<mutex> lk(cv_m);  // ����
	cout << id << " �ȴ���...\n";
	cv.wait(lk, []{return i == 1; });  //����wait���������߳�  ����1:�� ����2:����
	cout << id << " ���н���...\n";
	done = true;
}

void singnalsAll()
{
	this_thread::sleep_for(1s);
	{

		unique_lock<mutex> lk(cv_m);
		cout << "�����������ı�Ļ���" << endl;
		cv.notify_all();
	}

	this_thread::sleep_for(1s);
	{
		unique_lock<mutex> lk(cv_m);
		cout << "�����������ı�Ļ��Ѳ���" << endl;
		i = 1;
		cv.notify_all();
	}

}

void test02()
{
	thread t1(waits_thread, 1), t2(waits_thread, 2), t3(waits_thread, 3);
	thread t4(singnalsAll);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
}


int main(void)
{
	//test01();
	test02();
	return 0;
}