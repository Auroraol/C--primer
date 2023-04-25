#include <atomic>  // 原子
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

//template <class T>
class threadsafe_list
{
protected:
	struct Node
	{
		int value;
		Node* next;
		Node(int value, Node* next) : value(value), next(next) {}
	};
	atomic<Node*> list_head;  // 表头(相当于虚拟节点)

public:
	~threadsafe_list()
	{
		Node* cur;
		while (cur = list_head)
		{
			list_head = cur->next;
			delete cur;
		}
	}

	void push_front(int value)
	{
		Node* oldHead = list_head;
		Node* newNode = new Node(value, oldHead);
		while (!list_head.compare_exchange_weak(oldHead, newNode))
		{
			newNode->next = oldHead;
		}
	}

	void printList()
	{
		Node* cur = list_head;
		while (cur)
		{
			cout << cur->value << endl;
			cur = cur->next;

		}
	}
};




int main()
{
	vector<thread> t;
	threadsafe_list test;
	for (int i = 0; i < 5; i++)
	{
		t.push_back(thread(&threadsafe_list::push_front, &test, i));
	}

	for (auto& v : t)
	{
		v.join();
	}

	test.printList();

	return 0;
}