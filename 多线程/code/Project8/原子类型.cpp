#include <iostream>
#include <atomic>
#include <thread>

/*
	ԭ������
	using atomic_bool = atomic<bool>;
	using atomic_char   = atomic<char>;
	using atomic_schar  = atomic<signed char>;
	using atomic_uchar  = atomic<unsigned char>;
	using atomic_short  = atomic<short>;
	using atomic_ushort = atomic<unsigned short>;
	using atomic_int    = atomic<int>;
	using atomic_uint   = atomic<unsigned int>;
	using atomic_long   = atomic<long>;
	using atomic_ulong  = atomic<unsigned long>;
	using atomic_llong  = atomic<long long>;
	using atomic_ullong = atomic<unsigned long long>;

	// д����
	iNum.store(2,memory_order::memory_order_relaxed);
	// ������(��������)
	iNum.load(memory_order::memory_order_release);
	//�޸�
	iNum.exchange(2, memory_order::memory_order_relaxed);

	// �ڴ�ģ��˳��
	enum memory_order{
		imemory_order_relaxed,        	//��Ч�� //����ִ��˳������֤
		memory_order_release,           //��Ч�� //���߳��У�����ԭ�� д���� �����ڱ�ԭ�����Ͳ���������ſ�ʼ
		memory_order_seq_cst            //��Ч�� //ȫ���Ķ�д����˳������

		memory_order_consume,           //���߳���,����ԭ�Ӳ��������ڱ�ԭ�����Ͳ����������ڿ�ʼ
		memory_order_acquire,           //���߳��У�����ԭ�� ������ �����ڱ�ԭ�����Ͳ���������ſ�ʼ
		memory_order_acq_rel,           //memory_order_release��memory_order_acquire�ۺ�

	}
	ǿ˳��:����˳���ִ��˳����һ����
	��˳��:����ִ��˳��ᱻ���������ʵ�����

	����:
	atomic<int>a;
	atomic<int> b;
	void threadFunc()
	{
		int t = 1;
		a = t;
		b = 1;   //���д��벻����������κ�,ִ�й��̣�b=1���ܱ�����Ĵ���ִ�и����
	}

	ע��:
	������ʹ�� (memory_order_consume  case memory_order_acquire  case memory_order_acq_rel)

*/


using namespace std;

atomic<int> a;
atomic<int> b;

void setValue()
{
	int t = 1;
	a.store(t, memory_order::memory_order_release);   // ��Ժ���д���� //��֤��ǰ˳������ //��ǰ�ĵȼ��� a = t;  //  memory_order_release �����ʾb�Ĳ���������a���������
	b.store(2, memory_order::memory_order_relaxed);   // ����ִ��˳������֤ //b=1
}

void printAtomic()
{
	//cout << a << "\t" << b << endl;
	cout << a.load(memory_order_relaxed) << endl;
	b.exchange(111, memory_order_relaxed);
}

int main(void)
{
	thread t1(setValue);
	thread t2(printAtomic);
	t1.join();
	t2.join();
	cout << b.load(memory_order_relaxed);
	return 0;
}

/*

using namespace std;

atomic<int> a;
atomic<int> b;

void setValue()
{
	int t = 1;
	a.store(t);   // ��Ժ���д���� //��֤��ǰ˳������ //��ǰ�ĵȼ��� a = t;  //  memory_order_release �����ʾb�Ĳ���������a���������
	b.store(2);   // ����ִ��˳������֤ //b=1
}

void printAtomic()
{
	//cout << a << "\t" << b << endl;
	cout << a.load() << endl;
	b.exchange(111);
}

int main(void)
{
	thread t1(setValue);
	thread t2(printAtomic);
	t1.join();
	t2.join();
	cout << b.load();
	return 0;
}


*/