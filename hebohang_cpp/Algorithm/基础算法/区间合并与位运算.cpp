#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// λ���㣺
// ��n�ĵ�kλ����: n >> k & 1
// ����n�����һλ1��lowbit(n) = n & -n, ���nΪ0�򷵻�0
// 
// GCC�ṩ���ڽ������� __builtin_popcount(n),���Ծ�ȷ����n��ʾ�ɶ�����ʱ�ж��ٸ�1
// ����GCC���Լ���д���£�
// 
// ����n�Ķ����Ʊ�ʾ��1�ĸ�����
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

// ����ϲ�
// �������� [1, 3] [2, 4]���ϲ�֮����� [1, 4]
// �������� [1, 2] [3, 4]���ϲ�֮��Ͳ��䣬��ȻΪ2������
void merge(vector<pair<int, int>>& segs)
{
	if (segs.size() == 0) return;

	vector<pair<int, int>> res;

	// pair ����Ĭ�ϰ���һ���ȴ�С����С������
	sort(segs.begin(), segs.end());

	int st = segs[0].first, ed = segs[0].second;
	for (auto seg : segs)
	{
		// �����ǰ��ed�ȵ�ǰ�������߽�ҪС����˵����ǰ��st��ed����һ�����䣨���������ٺϲ���
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
	cout << "����һ������n������������n������[l, r]" << endl;
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