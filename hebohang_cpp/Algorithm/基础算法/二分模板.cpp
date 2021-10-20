#include <iostream>

using namespace std;

// ��������ģ��
// 
// ���ֵı��ʣ��������֣���
// ����������һ�����ʣ����������һ��Ϊ����һ������һ�벻���㣬��ô���־���ȥѰ������ı߽�

// ���ӣ���һ�δ�С���������check
bool check(int x)
{
	// ���x�Ƿ�����ĳ������
	// ������ʾ��������x�Ƿ����5
	return x > 5;
}

// ����[l, r]�����ֳ�[l, mid]��[mid + 1, r]ʱʹ�ã�
// ���Ｔ�ҵ�����5�ĵ�һ����������check�ĵ�һ������
int bsearch_1(int l, int r)
{
	while (l < r)
	{
		int mid = l + r >> 1;
		if (check(mid)) r = mid; // check()�ж�mid�Ƿ���������
		else l = mid + 1;
	}
	return l;
}

// ����[l, r]�����ֳ�[l, mid - 1]��[mid, r]ʱʹ�ã�
// ������Ϊÿ�θ��£������������� l = mid�� 
// ��� mid = l + r >> 1 �Ļ�����ô�� r = l + 1ʱ��mid = l����ôÿ�θ��¶����䣬��ѭ������������ mid = l + r + 1 >> 1
// ���Ｔ�ҵ�С�ڵ���5�����һ������������check�����һ������
int bsearch_2(int l, int r)
{
	while (l < r)
	{
		int mid = l + r + 1 >> 1;
		if (!check(mid)) l = mid; // ���С�ڵ���5����������
		else r = mid - 1;
	}
	return l;
}

// ����������
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
	cout << "1~10�е�һ������5����Ϊ��" << x << endl;
	x = bsearch_2(1, 10);
	cout << "1~10�����һ��С�ڵ���5����Ϊ��" << x << endl;
	return 0;
}