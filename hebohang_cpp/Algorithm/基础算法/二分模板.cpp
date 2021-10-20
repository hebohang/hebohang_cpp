#include <iostream>

using namespace std;

// 整数二分模板
// 
// 二分的本质（整数二分）：
// 整个区间有一个性质，且区间可以一分为二：一半满足一半不满足，那么二分就能去寻找区间的边界

// 例子：对一段从小到大的数来check
bool check(int x)
{
	// 检查x是否满足某种性质
	// 这里做示范，如检查x是否大于5
	return x > 5;
}

// 区间[l, r]被划分成[l, mid]和[mid + 1, r]时使用：
// 这里即找到大于5的第一个数（满足check的第一个数）
int bsearch_1(int l, int r)
{
	while (l < r)
	{
		int mid = l + r >> 1;
		if (check(mid)) r = mid; // check()判断mid是否满足性质
		else l = mid + 1;
	}
	return l;
}

// 区间[l, r]被划分成[l, mid - 1]和[mid, r]时使用：
// 这里因为每次更新，若满足性质则 l = mid， 
// 如果 mid = l + r >> 1 的话，那么若 r = l + 1时，mid = l，那么每次更新都不变，死循环，所以这里 mid = l + r + 1 >> 1
// 这里即找到小于等于5的最后一个数（不满足check的最后一个数）
int bsearch_2(int l, int r)
{
	while (l < r)
	{
		int mid = l + r + 1 >> 1;
		if (!check(mid)) l = mid; // 如果小于等于5，继续查找
		else r = mid - 1;
	}
	return l;
}

// 浮点数二分
double bsearch_3(double l, double r)
{
	const double eps = 1e-6;
	while (r - l > eps)
	{
		double mid = (l + r) / 2;
		if (check(mid)) r = mid;
		else l = mid;
	}
	return l;
}

int main()
{
	int x = bsearch_1(1, 10);
	cout << "1~10中第一个大于5的数为：" << x << endl;
	x = bsearch_2(1, 10);
	cout << "1~10中最后一个小于等于5的数为：" << x << endl;
	return 0;
}