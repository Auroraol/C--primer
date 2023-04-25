#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

/*
int value = 0;

void testFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		value++;
	}
}


int main(void)
{
	thread t1(testFunc);
	thread t2(testFunc);
	t1.join();
	t2.join();

	cout << value << endl;
	return 0;
}

*/


/*
	ԭ�Ӳ���: �Ա����Ķ�д�������ɷָ�
	atomic: ģ����
	atomic<int> foo(0);
	//��������͸�ֵ���㱻ɾ����atomic( const atomic&) = delete;  atomic& operator=( const atomic&) = delete;

*/



int value = 0;
atomic<int> a_value = 0;
atomic <int> t_value = 0;

void testFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		value++;
		a_value++;   // += �Ƿ��ϵĶ�=�ǲ��е�
		//t_value = t_value + 1;  // ������ᱨ��
		if (t_value != 0)
		{
			t_value = 1;
		}

	}
}


int main(void)
{
	thread t1(testFunc);
	thread t2(testFunc);
	t1.join();
	t2.join();
	cout << value << endl;
	cout << a_value << endl;

	return 0;
}