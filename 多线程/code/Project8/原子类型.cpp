#include <iostream>
#include <atomic>
#include <thread>

/*
	原子类型
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

	// 写操作
	iNum.store(2,memory_order::memory_order_relaxed);
	// 读操作(加载数据)
	iNum.load(memory_order::memory_order_release);
	//修改
	iNum.exchange(2, memory_order::memory_order_relaxed);

	// 内存模型顺序
	enum memory_order{
		imemory_order_relaxed,        	//有效的 //对于执行顺序不做保证
		memory_order_release,           //有效的 //本线程中，后续原子 写操作 必须在本原子类型操作结束后才开始
		memory_order_seq_cst            //有效的 //全部的读写按照顺序来做

		memory_order_consume,           //本线程中,后续原子操作必须在本原子类型操作结束后在开始
		memory_order_acquire,           //本线程中，后续原子 读操作 必须在本原子类型操作结束后才开始
		memory_order_acq_rel,           //memory_order_release和memory_order_acquire综合

	}
	强顺序:代码顺序和执行顺序是一样的
	弱顺序:代码执行顺序会被处理器做适当调整

	比如:
	atomic<int>a;
	atomic<int> b;
	void threadFunc()
	{
		int t = 1;
		a = t;
		b = 1;   //这行代码不依赖上面的任何,执行过程，b=1可能比上面的代码执行更早点
	}

	注意:
	并不能使用 (memory_order_consume  case memory_order_acquire  case memory_order_acq_rel)

*/


using namespace std;

atomic<int> a;
atomic<int> b;

void setValue()
{
	int t = 1;
	a.store(t, memory_order::memory_order_release);   // 针对后续写操作 //保证当前顺序优先 //当前的等价于 a = t;  //  memory_order_release 这里表示b的操作必续在a操作后进行
	b.store(2, memory_order::memory_order_relaxed);   // 对于执行顺序不做保证 //b=1
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
	a.store(t);   // 针对后续写操作 //保证当前顺序优先 //当前的等价于 a = t;  //  memory_order_release 这里表示b的操作必续在a操作后进行
	b.store(2);   // 对于执行顺序不做保证 //b=1
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