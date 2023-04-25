#include <iostream>
#include <thread>
#include <deque>
#include <condition_variable>
#include <mutex>

using namespace std;

const int KProduceItems = 20;  // ��Ʒ����
const int KPrositorySize = 5;   // �ֿ��С

// �ֿ���
template <class T>
class Repository
{
public:
	deque<T> items_buff;	//�洢��Ʒ

	mutex mtx;				//�����ߺ������ߵĻ�����
	mutex produce_mutex;	//��������������
	mutex consume_mutex;	//���Ѽ���������

	condition_variable repo_not_full;     //�ֿⲻ��״̬��������
	condition_variable repo_not_empty;    //�ֿⲻ�յ�����

	size_t produce_item_count;  // �����߼���
	size_t consume_item_count;  // �����߼���

	size_t produce_position;     //  �����߸���
	size_t consume_position;     //  �����߸���

	Repository()
	{
		produce_item_count = 0;
		consume_item_count = 0;

		produce_position = 0;
		consume_position = 0;
	}

};


// ������
template <class T>
class Factory
{

protected:
	Repository<T> repo;   // �����ֿ����
	//��Ʒ�ŵ��ֿ�
	void ProductItem(Repository<T>& repo, T item)
	{
		unique_lock<mutex> lock(repo.mtx);  // ����
		repo.items_buff.push_back(item);

		if (repo.produce_position == KPrositorySize)
		{
			repo.produce_position = 0;   // �����Ժ�������=0
		}
		// ���յ�ʱ���ѱ���߳�
		repo.repo_not_empty.notify_all();
		lock.unlock(); //���Բ���
	}

	//�Ӵ���ȡ��Ʒ
	T ConsuneItem(Repository<T>& repo)
	{
		unique_lock<mutex> lock(repo.mtx);  // ���� 
		while (repo.items_buff.empty())
		{
			cout << "�޻�Դ,�ȴ�..." << endl;
			repo.repo_not_empty.wait(lock);   // �վ͵ȴ�
		}
		// �л�Դ֮�����
		T data = repo.items_buff.front();   // ȡ������
		repo.items_buff.pop_front();
		// �ж��Ƿ���
		if (repo.consume_position == KPrositorySize)
		{
			repo.consume_position = 0;  // �����Ժ�������=0
		}
		// ������ʱ�����߳�
		repo.repo_not_full.notify_all();
		lock.unlock();
		return data;
	}


public:
	//�����߲���
	void ProduceTask()
	{
		bool ready_to_exit = false;  // ��������
		while (true)
		{
			unique_lock<mutex> lock(repo.produce_mutex);  //����
			//�߳̽�������
			if (repo.produce_item_count < KProduceItems)
			{
				repo.produce_item_count++;

				//������Ʒ�Ĵ����
				this_thread::sleep_for(1s);  // ʵ�ʿ�������
				T item = repo.produce_item_count;
				//cout << "������id: " << this_thread::get_id << "��Դ���: [" << item << "]" << endl;
				printf("������id:%d\t��Դ���:[%d]\n", this_thread::get_id(), item);

				ProductItem(repo, item);
			}
			else
			{
				ready_to_exit = true;
			}
			lock.unlock();

			if (ready_to_exit)  // ��������
			{
				break;
			}

		}

	}

	// �����߲���
	void consumeTask()
	{
		bool ready_to_exit = false;  // ��������
		while (true)
		{
			unique_lock<mutex> lock(repo.consume_mutex);
			if (repo.consume_item_count < KProduceItems)
			{
				T item = ConsuneItem(repo);
				//������Ʒ����
				this_thread::sleep_for(1s);
				//cout << "������id: " << this_thread::get_id() << "��Դ���: [" << item << "]" << endl;
				printf("������id:%d\t��Դ���:[%d]\n", this_thread::get_id(), item);
				repo.consume_item_count++;
			}
			else
			{
				ready_to_exit = true;
			}
			lock.unlock();

			if (ready_to_exit)  // ��������
			{
				break;
			}
		}
	}

};


int main(void)
{
	cout << "�������߳�id: " << this_thread::get_id() << endl;

	Factory<int>myFactory;
	thread produce1(&Factory<int>::ProduceTask, &myFactory);
	thread produce2(&Factory<int>::ProduceTask, &myFactory);

	thread consumer1(&Factory<int>::consumeTask, &myFactory);
	thread consumer2(&Factory<int>::consumeTask, &myFactory);
	thread consumer3(&Factory<int>::consumeTask, &myFactory);
	produce1.join();
	produce2.join();
	consumer1.join();
	consumer2.join();
	consumer3.join();

	return 0;
}