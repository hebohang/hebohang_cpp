#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> alls; // �洢���д���ɢ����ֵ

// ��ɢ��
// ����һϵ����ӳ�䵽1��2��3...
void discretization()
{
    sort(alls.begin(), alls.end()); // ������ֵ����
    alls.erase(unique(alls.begin(), alls.end()), alls.end());   // ȥ���ظ�Ԫ��
    // unique ��ָ�����ȥ�غ������в��ظ����е����һ��Ԫ�ص���һ��Ԫ��
}

// �������x��Ӧ����ɢ����ֵ
int discretization_find(int x) // �ҵ���һ�����ڵ���x��λ��
{
    int l = 0, r = alls.size() - 1;
    while (l < r)
    {
        int mid = l + r >> 1;
        if (alls[mid] >= x) r = mid;
        else l = mid + 1;
    }
    return r + 1; // ӳ�䵽1, 2, ...n
}

int main()
{
    alls = { 0, 100, 200, 500, 400, 300, 200, 300, 200, 200 };
    discretization(); // ��ʱallsӦ�ô� 0 100 200 300 400 500����ɢ����ӳ��ֵӦ��Ϊ 1 2 3 4 5 6
    int res = discretization_find(150); // ��һ�����ڵ���150������Ϊ200����Ӧ��ɢ��Ϊ3�����Է���3
    cout << res << endl;

    return 0;
}