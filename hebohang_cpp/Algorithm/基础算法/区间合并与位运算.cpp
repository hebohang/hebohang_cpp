#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// 位运算：
// 求n的第k位数字: n >> k & 1
// 返回n的最后一位1：lowbit(n) = n & -n, 如果n为0则返回0
// 
// GCC提供的内建函数： __builtin_popcount(n),可以精确计算n表示成二进制时有多少个1
// 不用GCC，自己手写如下：
// 
// 计算n的二进制表示中1的个数：
int count_1_num(int n)
{
	int res = 0;
	while ((n & -n) != 0)
	{
		n -= n & -n;
		res++;
	}
	return res;
}

// 区间合并
// 比如区间 [1, 3] [2, 4]，合并之后就是 [1, 4]
// 比如区间 [1, 2] [3, 4]，合并之后就不变，仍然为2个区间
void merge(vector<pair<int, int>>& segs)
{
	if (segs.size() == 0) return;

	vector<pair<int, int>> res;

	// pair 排序默认按第一个比大小，从小到大排
	sort(segs.begin(), segs.end());

	int st = segs[0].first, ed = segs[0].second;
	for (auto seg : segs)
	{
		// 如果当前的ed比当前区间的左边界要小，则说明当前的st和ed构成一个区间（后续不会再合并）
		if (ed < seg.first)
		{
			res.push_back({ st, ed });
			st = seg.first, ed = seg.second;
		}
		else ed = max(ed, seg.second);
	}

	res.push_back({ st, ed });

	segs = res;
}

int main()
{
	cout << "输入一个整数n，接下来输入n个区间[l, r]" << endl;
	int n;
	cin >> n;
	vector<pair<int, int>> segs;
	for (int i = 0; i < n; i++)
	{
		int l, r;
		cin >> l >> r;
		segs.push_back({ l, r });
	}

	merge(segs);

	cout << segs.size() << endl;

	cin.get();
}