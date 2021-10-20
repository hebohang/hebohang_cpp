#include <iostream>

using namespace std;

const int N = 10010; // 数组的最大长度
int tmp[N]; // merge_sort用到tmp数组
int q[N];

// 这里的快排和归并都是从小到大排序

void quick_sort(int q[], int l, int r)
{
	if (l >= r) return;

	int i = l - 1, j = r + 1, x = q[l + r >> 1];
	while (i < j)
	{
		do i++; while (q[i] < x);
		do j--; while (q[j] > x);
		if (i < j) swap(q[i], q[j]);
	}
	quick_sort(q, l, j);
	quick_sort(q, j + 1, r);
}

void merge_sort(int q[], int l, int r)
{
	if (l >= r) return;

	// 排序好两段，二分、分而治之的思想
	int mid = l + r >> 1;
	merge_sort(q, l, mid);
	merge_sort(q, mid + 1, r);

	// 两段从头一个一个塞入tmp数组，比大小
	int k = 0, i = l, j = mid + 1;
	while (i <= mid && j <= r)
		if (q[i] <= q[j]) tmp[k++] = q[i++];
		else tmp[k++] = q[j++];

	// 再把两段还没塞入的塞入tmp中
	while (i <= mid) tmp[k++] = q[i++];
	while (j <= r) tmp[k++] = q[j++];

	// 把tmp数组存的数挪到q中，完成排序
	for (i = l, j = 0; i <= r; i++, j++) q[i] = tmp[j];
}

int main()
{
	int tmp_q[5];

	cout << "请输入五个整数： " << endl;
	for (int i = 0; i < 5; i++)
	{
		cin >> q[i];
		tmp_q[i] = q[i];
	}

	quick_sort(q, 0, 4);
	cout << "快速排序结果： ";
	for (int i = 0; i < 5; i++)
	{
		cout << q[i] << " ";
	}
	puts("");

	memcpy(q, tmp_q, 20);

	merge_sort(q, 0, 4);
	cout << "归并排序结果： ";
	for (int i = 0; i < 5; i++)
	{
		cout << q[i] << " ";
	}
	puts("");

	return 0;
}