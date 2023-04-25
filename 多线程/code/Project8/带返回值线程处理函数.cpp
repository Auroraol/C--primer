#include <iostream>
#include <thread>
#include <future>
#include <chrono>

using namespace std;
/*
������ֵ���̴߳������ķ���:
��һ��: ����async: ����һ���첽����,(�����߳�,����ִ���̴߳�����),����future����.
�ڶ���: ͨ��future�����е�get()������ȡ�̴߳���������ֵ. // ע��get()����ֻ�ܻ�ȡһ��
*/

//N0.1 ������ֵ����ͨ�����䵱�̴߳�����
int testThreadOne()
{
	return 1001;
}

//No.2 ������ֵ�����Ա�����䵱�̴߳�����
class MM
{
public:
	int mmthread(int num)
	{
		cout << "MM�����id: " << this_thread::get_id() << endl;
		num *= 2;
		chrono::milliseconds duration(5000);
		this_thread::sleep_for(duration);
		return num;
	}
};

void testAsync()
{
	future<int> result = async(testThreadOne);
	cout << result.get() << endl;

	MM mm;
	auto temp = async(&MM::mmthread, &mm, 1999);
	cout << temp.get() << endl;

	//async��������
	//launch::async    : �����߳�, ִ���̴߳�����(Ĭ��)
	//launch::deferred : �̴߳������ӳٵ����ǵ���wait��get����ʱ��Ż�ִ��.������û�д����̵߳�
	auto testDerred1 = async(launch::deferred, &MM::mmthread, &mm, 1000);
	cout << testDerred1.get() << endl;  //�Ŵ����߳�
	auto testDerred2 = async(launch::async, &MM::mmthread, &mm, 1000);

}

//No.3��thread���������ֵ���̴߳�����
//��һ��: ͨ����ģ��packaged_task��װ�̴߳�����
//�ڶ���: ͨ��packaged_task�Ķ������get_future��ȡfuture������ͨ��get()�����õ����̴߳������ķ���ֵ
void testPackaged_task()
{
	//No.1���������ֵ����ͨ�����䵱�̴߳�����
	packaged_task<int(void)> taskOne(testThreadOne);
	thread testOne(ref(taskOne));
	testOne.join();
	cout << taskOne.get_future().get() << endl;

	//No.2 ���������ֵ�����Ա�����䵱�̴߳�����
	MM mm;
	packaged_task<int(int)> taskTwo(bind(&MM::mmthread, &mm, placeholders::_1)); //placeholders::_1ռλ��
	thread testTwo(ref(taskTwo), 20);
	testTwo.join();
	cout << taskTwo.get_future().get() << endl;

	//No.3���Lambda���ʽ
	packaged_task<int(int)> taskThree([](int num){

		cout << "thread_id: " << this_thread::get_id() << endl;
		num *= 10;
		return num;
		});
	thread testTree(ref(taskThree), 7);
	testTree.join();
	cout << taskThree.get_future().get() << endl;

}


//No.4 promise��ȡ�̴߳���������ֵ
//��һ��: ͨ��promise��ģ�幹������,ͨ������set_value�Ǵ洢������Ҫ���ص�ֵ
//�ڶ���: ͨ��get_future��ȡfuture������ͨ��get()������ȡ�̴߳����е�ֵ
void testPromiseThread(promise<int>& temp, int data)
{
	cout << "thread_id" << this_thread::get_id() << endl;
	data *= 3;
	temp.set_value(data);
}
void testPromise()
{
	promise<int> temp;
	thread testp(testPromiseThread, ref(temp), 18);
	testp.join();
	cout << temp.get_future().get() << endl;
}

int main(void)
{
	//testAsync();
	//testPackaged_task();
	testPromise();
	return 0;
}