#include <iostream>

using namespace std;

const int N = 10010; // �������󳤶�
int tmp[N]; // merge_sort�õ�tmp����
int q[N];

// ����Ŀ��ź͹鲢���Ǵ�С��������

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

	// ��������Σ����֡��ֶ���֮��˼��
	int mid = l + r >> 1;
	merge_sort(q, l, mid);
	merge_sort(q, mid + 1, r);

	// ���δ�ͷһ��һ������tmp���飬�ȴ�С
	int k = 0, i = l, j = mid + 1;
	while (i <= mid && j <= r)
		if (q[i] <= q[j]) tmp[k++] = q[i++];
		else tmp[k++] = q[j++];

	// �ٰ����λ�û���������tmp��
	while (i <= mid) tmp[k++] = q[i++];
	while (j <= r) tmp[k++] = q[j++];

	// ��tmp��������Ų��q�У��������
	for (i = l, j = 0; i <= r; i++, j++) q[i] = tmp[j];
}

int main()
{
	int tmp_q[5];

	cout << "��������������� " << endl;
	for (int i = 0; i < 5; i++)
	{
		cin >> q[i];
		tmp_q[i] = q[i];
	}

	quick_sort(q, 0, 4);
	cout << "������������ ";
	for (int i = 0; i < 5; i++)
	{
		cout << q[i] << " ";
	}
	puts("");

	memcpy(q, tmp_q, 20);

	merge_sort(q, 0, 4);
	cout << "�鲢�������� ";
	for (int i = 0; i < 5; i++)
	{
		cout << q[i] << " ";
	}
	puts("");

	return 0;
}