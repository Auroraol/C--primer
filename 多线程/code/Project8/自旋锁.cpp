#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

/*
	atomic_flag:无锁的
		<1>.test_and_set() :  如果没有被设置就设置一下，如果设置了返回true
		<2>.clear() :         清除标记,让下一次调用test_and_set()返回false
	其他原子类型 : is_lock_free()判断是否无锁

	注意区分:互斥锁和自旋锁
	 1. 互斥锁的特点是资源申请者只能进入睡眠状态。
	 2. 自旋锁不会引起调用者的休眠，调用者一致循环在哪里，等待锁的释放。


*/

atomic_flag lock = ATOMIC_FLAG_INIT;   // ATOMIC_FLAG_INIT 就是有一个 {}

void func1(int n)
{
	while (lock.test_and_set(memory_order_acquire))
	{
		printf("等待中....%d\n", n);
	}
	printf("线程完成:%d \n", n);

}

void func2(int n)
{
	printf("线程开始启动:%d \n", n);
	this_thread::sleep_for(10ms);
	lock.clear();                 // 清除后func1里的循环才不满足条件.完成自然的去调用另一个线程
	printf("线程运行结束:%d\n", n);
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