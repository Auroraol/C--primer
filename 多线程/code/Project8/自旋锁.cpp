#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

/*
	atomic_flag:������
		<1>.test_and_set() :  ���û�б����þ�����һ�£���������˷���true
		<2>.clear() :         ������,����һ�ε���test_and_set()����false
	����ԭ������ : is_lock_free()�ж��Ƿ�����

	ע������:��������������
	 1. ���������ص�����Դ������ֻ�ܽ���˯��״̬��
	 2. ������������������ߵ����ߣ�������һ��ѭ��������ȴ������ͷš�


*/

atomic_flag lock = ATOMIC_FLAG_INIT;   // ATOMIC_FLAG_INIT ������һ�� {}

void func1(int n)
{
	while (lock.test_and_set(memory_order_acquire))
	{
		printf("�ȴ���....%d\n", n);
	}
	printf("�߳����:%d \n", n);

}

void func2(int n)
{
	printf("�߳̿�ʼ����:%d \n", n);
	this_thread::sleep_for(10ms);
	lock.clear();                 // �����func1���ѭ���Ų���������.�����Ȼ��ȥ������һ���߳�
	printf("�߳����н���:%d\n", n);
}

int main(void)
{
	lock.test_and_set();
	thread t1(func1, 1);
	thread t2(func2, 2);
	t1.join();
	t2.join();

	return 0;
}