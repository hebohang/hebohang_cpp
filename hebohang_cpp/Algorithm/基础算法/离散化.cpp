#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> alls; // 存储所有待离散化的值

// 离散化
// 即将一系列数映射到1、2、3...
void discretization()
{
    sort(alls.begin(), alls.end()); // 将所有值排序
    alls.erase(unique(alls.begin(), alls.end()), alls.end());   // 去掉重复元素
    // unique 它指向的是去重后容器中不重复序列的最后一个元素的下一个元素
}

// 二分求出x对应的离散化的值
int discretization_find(int x) // 找到第一个大于等于x的位置
{
    int l = 0, r = alls.size() - 1;
    while (l < r)
    {
        int mid = l + r >> 1;
        if (alls[mid] >= x) r = mid;
        else l = mid + 1;
    }
    return r + 1; // 映射到1, 2, ...n
}

int main()
{
    alls = { 0, 100, 200, 500, 400, 300, 200, 300, 200, 200 };
    discretization(); // 此时alls应该存 0 100 200 300 400 500，离散化后映射值应该为 1 2 3 4 5 6
    int res = discretization_find(150); // 第一个大于等于150的数，为200，对应离散化为3，所以返回3
    cout << res << endl;

    return 0;
}