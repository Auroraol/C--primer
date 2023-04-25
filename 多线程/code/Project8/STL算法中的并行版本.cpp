#include <algorithm>  // 算法头文件
#include <numeric>    // 算法头文件
#include <chrono>    // 时间
#include <vector>
#include <random>    // 随机数
#include <iostream>
#include <execution>  // 并发版本  // c++17

/*
	增加参数: sequenced_policy  提供相应并行版算法
	execution::seq          并行算法执行可以不并行化
	execution::par           并行算法执行可以并行化
	execution::par_unseq     并行算法执行的可以并行以及向量化
*/

using namespace std;

void randomData(vector<double>& data, int size)
{
	// 随机数
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(1.0, 100.0);

	for (int i = 0; i < size; i++)
	{
		data.push_back(dist(mt));
	}
}

int main()
{
	vector<double> data;
	randomData(data, 10e6);

	vector<double> test1(data);
	vector<double> test2(data);
	vector<double> test3(data);
	vector<double> test4(data);

	auto stime = chrono::steady_clock::now();
	sort(execution::seq, test1.begin(), test1.end());
	auto etime = chrono::steady_clock::now();
	// 打印时间
	auto duration1 = chrono::duration_cast<chrono::microseconds>(etime - stime).count();
	cout << "execution::seq: " << duration1 << endl;

	// ----------------

	stime = chrono::steady_clock::now();
	sort(execution::par, test2.begin(), test2.end());
	etime = chrono::steady_clock::now();
	// 打印时间
	auto duration2 = chrono::duration_cast<chrono::microseconds>(etime - stime).count();
	cout << "execution::par: " << duration2 << endl;

	// ----------------
	stime = chrono::steady_clock::now();
	sort(execution::par_unseq, test3.begin(), test3.end());
	etime = chrono::steady_clock::now();
	// 打印时间
	auto duration3 = chrono::duration_cast<chrono::microseconds>(etime - stime).count();
	cout << "execution::par_unseq: " << duration3 << endl;

	// 普通版----------------
	stime = chrono::steady_clock::now();
	sort(execution::par_unseq, test4.begin(), test4.end());
	etime = chrono::steady_clock::now();
	// 打印时间
	auto duration4 = chrono::duration_cast<chrono::microseconds>(etime - stime).count();
	cout << "sort: " << duration4 << endl;



	return 0;
}