#include <iostream>
#include <vector>

using namespace std;

// ����߾���vector�洢���������ǵ�0��Ԫ�������λ
// ������λ��ֱ��push_back��Ч�ʸ�

// �߾��ȼӷ�
vector<int> add(vector<int>& A, vector<int>& B)
{
	if (A.size() < B.size()) return add(B, A);

	vector<int> C;
	int t = 0;
	for (int i = 0; i < A.size(); i++)
	{
		t += A[i];
		if (i < B.size()) t += B[i];
		C.push_back(t % 10);
		t /= 10;
	}

	if (t) C.push_back(t);
	return C;
}

// �߾��ȼ���
vector<int> sub(vector<int>& A, vector<int>& B)
{
	vector<int> C;
	int t = 0;
	for (int i = 0; i < A.size(); i++)
	{
		t = A[i] - t;
		if (i < B.size()) t -= B[i];
		C.push_back((t + 10) % 10);
		if (t < 0) t = 1;
		else t = 0;
	}

	// ����ǰ��0
	while (C.size() > 1 && C.back() == 0) C.pop_back();
	return C;
}

// �߾��ȳ˷�
vector<int> mul(vector<int>& A, int& b)
{
	vector<int> C;
	for (int i = 0, t = 0; i < A.size() || t; i++)
	{
		if (i < A.size()) t += A[i] * b;
		C.push_back(t % 10);
		t /= 10;
	}

	while (C.size() > 1 && C.back() == 0) C.pop_back();
	return C;
}

// �߾��ȳ��Ե;���
// A / b = C ... r, A >= 0, b > 0
vector<int> div(vector<int>& A, int b, int& r)
{
	vector<int> C;
	r = 0;
	for (int i = A.size() - 1; i >= 0; i--)
	{
		r = r * 10 + A[i];
		C.push_back(r / b);
		r %= b;
	}
	reverse(C.begin(), C.end());
	while (C.size() > 1 && C.back() == 0) C.pop_back();
	return C;
}

int main()
{
	string a = "100";
	string b = "20";
	int bb = 20;

	vector<int> A;
	for (int i = a.size() - 1; i >= 0; i--) A.push_back(a[i] - '0');

	vector<int> B;
	for (int i = b.size() - 1; i >= 0; i--) B.push_back(b[i] - '0');

	cout << "a = 100, b = 20" << endl;
	{
		cout << "�߾��ȼӷ�: " << endl;
		auto C = add(A, B);
		for (int i = C.size() - 1; i >= 0; i--) printf("%d", C[i]);
		puts("");
	}

	{
		cout << "�߾��ȼ���: " << endl;
		auto C = sub(A, B);
		for (int i = C.size() - 1; i >= 0; i--) printf("%d", C[i]);
		puts("");
	}

	{
		cout << "�߾��ȳ˷�: " << endl;
		auto C = mul(A, bb);
		for (int i = C.size() - 1; i >= 0; i--) printf("%d", C[i]);
		puts("");
	}

	{
		cout << "�߾��ȳ���: " << endl;
		int r;
		auto C = div(A, bb, r);
		for (int i = C.size() - 1; i >= 0; i--) printf("%d", C[i]);
		puts("");
		cout << r << endl;
	}
}