@[TOC]
# 前言
链接：
[https://changkun.de/modern-cpp/](https://changkun.de/modern-cpp/)
[https://github.com/changkun/modern-cpp-tutorial](https://github.com/changkun/modern-cpp-tutorial)
![在这里插入图片描述](https://img-blog.csdnimg.cn/16495d2eeef04b0783ba57027c432764.png)
为了方便，我直接用 VS2019 了，即用 Visual C++ 而没有用作者的 clang++ 测试。


# 第 6 章 正则表达式
正则表达式不是 C++ 语言的一部分，给一个GitHub网站，我当时大概过了一遍：
[https://github.com/ziishaned/learn-regex](https://github.com/ziishaned/learn-regex)

传统上我们只能使用 boost 的正则表达式库。 而 C++11 正式将正则表达式的的处理方法纳入标准库的行列，从语言级上提供了标准的支持， 不再依赖第三方。

C++11 提供的正则表达式库操作 std::string 对象， 模式 std::regex (本质是 std::basic_regex)进行初始化， 通过 std::regex_match 进行匹配， 从而产生 std::smatch （本质是 std::match_results 对象）。

用法：
（这段代码我直接copy的原书，详细请看原书，或参考链接：[https://en.cppreference.com/w/cpp/regex](https://en.cppreference.com/w/cpp/regex)）

```cpp
#include <iostream>
#include <string>
#include <regex>

int main() {
    std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt"};
    // 在 C++ 中 \ 会被作为字符串内的转义符，为使 \. 作为正则表达式传递进去生效，需要对 \ 进行二次转义，从而有 \\.
    std::regex txt_regex("[a-z]+\\.txt");
    for (const auto &fname: fnames)
        std::cout << fname << ": " << std::regex_match(fname, txt_regex) << std::endl;
}
```
大佬的回答：
[https://www.zhihu.com/question/23070203/answer/84248248](https://www.zhihu.com/question/23070203/answer/84248248)

# 第 7 章 并行与并发
## 额外可参考的资料
[https://www.bookstack.cn/books/CPP-Concurrency-In-Action-2ed-2019](https://www.bookstack.cn/books/CPP-Concurrency-In-Action-2ed-2019)
[https://zhuanlan.zhihu.com/p/194198073](https://zhuanlan.zhihu.com/p/194198073)

以及关于并行并发我非常认同这个回答：
[https://www.zhihu.com/question/307100151/answer/894486042](https://www.zhihu.com/question/307100151/answer/894486042)

并发描述的是“如何处理”，而并行描述的是“如何执行”，将并发理解为一种解决问题的方法，将大的任务拆解为许许多多小的可以并发的任务是重要的编程思想。

线程支持库文档：
[https://en.cppreference.com/w/cpp/thread](https://en.cppreference.com/w/cpp/thread)

## 并行基础
std::thread 用于创建一个执行的线程实例，使用时需要包含 <thread> 头文件，

一些基本操作：
get_id() 来获取所创建线程的线程 ID，使用 join() 来加入一个线程等等，详细可见：
[http://www.cplusplus.com/reference/thread/thread/?kw=thread](http://www.cplusplus.com/reference/thread/thread/?kw=thread)

对于join上面链接有解释：The function returns when the thread execution has completed.
在线程执行完成时返回。

而对于detach，也有解释：Detaches the thread represented by the object from the calling thread, allowing them to execute independently from each other.After a call to this function, the thread object becomes non-joinable and can be destroyed safely.

detach原本在英文中就有分离的意思，这里就是说将线程与调用它的父线程分离开而独自运行，且之后不能再join，并且可以安全的销毁。

下面我根据链接中的示例改编出一个程序，一次解释join和detach：

```cpp
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <Windows.h>

void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended\n";
}

int main()
{
	std::cout << "Spawning 3 threads...\n";
	std::thread t1(pause_thread, 1);
	std::thread t2(pause_thread, 15);
	std::thread t3(pause_thread, 16);
	std::thread(pause_thread, 10).detach();
	std::cout << "Done spawning threads. Now waiting for them to join:\n";
	Sleep(30000);
	std::cout << "hbh\n";
	t1.join();
	t2.join();
	t3.join();
	std::cout << "All threads joined!\n";

	return 0;
}
```
chrono是一个time library，和regex一样，也是源于boost，后被C++11引入标准库。上面代码就是一个暂停多少秒的函数，而同时我又用了Sleep函数（需要引入头文件Windows.h）去暂停主线程，运行结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c7af1a2d9c564929a1293da3cb86c675.png)
我们再来分析一下，以理解join和detach：首先创建线程的时候它们就在运行了，并且detach是不需要也不能join的；join只是在线程完成时返回，这里我强制等待30000毫秒也就是30秒，大于每个线程的等待时间，因此在线程都完成时main函数仍然在sleep中，最后才执行join，但join这一步什么也没有输出。但是不调用join程序会出错，因为join会清理线程相关的存储部分，这也说明join只能调用一次。detach同样也只能调用一次，可否调用可通过joinable()判断。


## 互斥量与临界区
C++11 引入了 mutex 相关的类，其所有相关的函数都放在 <mutex> 头文件中。
相关链接：[http://www.cplusplus.com/reference/mutex/mutex/?kw=mutex](http://www.cplusplus.com/reference/mutex/mutex/?kw=mutex)

通过实例化 std::mutex 可以创建互斥量， 而通过其成员函数 lock() 可以进行上锁，unlock() 可以进行解锁。 C++11 还为互斥量提供了一个 RAII 语法的模板类 std::lock_guard。 RAII 在不失代码简洁性的同时，很好的保证了代码的异常安全性。在 RAII 用法下，对于临界区的互斥量的创建只需要在作用域的开始部分。

不过在并发编程中，推荐使用 std::unique_lock 以独占所有权的方式管理 mutex 对象上的上锁和解锁的操作。unique_lock 是 相对于 lock_guard 出现的，因此它们功能有些类似但unique_lock更加灵活。 std::unique_lock对象实例化后会直接接管std::mutex。如果你用到了条件变量 std::condition_variable::wait 则必须使用 std::unique_lock 作为参数。

std::lock_guard 不能显式的调用 lock 和 unlock， 而 std::unique_lock 可以在声明后的任意位置调用， 可以缩小锁的作用范围，提供更高的并发度。

用法示例（CriticalSection就是临界区的意思，即访问共享数据的程序片段）：

```cpp
int v = 1;

void critical_section_1(int change_v) {
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    // 执行竞争操作
    v = change_v;

    // 离开此作用域后 mtx 会被释放
}

void critical_section_2(int change_v) {
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    // 执行竞争操作
    v = change_v;
    std::cout << v << std::endl;
    // 将锁进行释放
    lock.unlock();

    // 在此期间，任何人都可以抢夺 v 的持有权

    // 开始另一组竞争操作，再次加锁
    lock.lock();
    v += 1;
    std::cout << v << std::endl;
}
```

引入了unique_lock 、lock_guard ，我们接着就要引入adopt_lock、defer_lock、try_to_lock

先放链接：
[https://www.cplusplus.com/reference/mutex/adopt_lock/?kw=adopt_lock](https://www.cplusplus.com/reference/mutex/adopt_lock/?kw=adopt_lock)

从链接中我们看adopt_lock的解释，知道它仅仅用来消除构造函数的歧义（事实上adopt_lock、defer_lock和 try_to_lock都有写说明：The value is a compile-time constant that carries no state, and is merely used to disambiguate between constructor signatures.），啥意思呢，我们看源码（前面说了在VS2019中）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4a8da5371bb14aa799db5986dc524281.png)
也就是，没有adopt_lock构造函数就帮咱lock了，而带了这个参数我们就要自己提前手动锁定。

至于后二者都是用在unique_lock中而不能在lock_guard中，defer_lock初始化了一个没有lock的mutex，而try_to_lock就是尝试去lock，可以从源码的注释窥见一二：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a42434563a0747ec9b65ea54761870ff.png)

最后再引入读写锁shared_mutex，shared_mutex通过lock_shared，unlock_shared，shared_lock进行读者的锁定与解锁；通过lock，unlock，unique_lock进行写者的锁定与解锁。


## 期物
期物（Future）表现为 std::future，它提供了一个访问异步操作结果的途径，可以联想屏障（barrier），可以作为一种简单的线程同步手段。

参考链接：[https://www.cplusplus.com/reference/future/future/?kw=future](https://www.cplusplus.com/reference/future/future/?kw=future)

这里我再次从原文copy一下示例代码，再简单分析：
```cpp
#include <iostream>
#include <future>
#include <thread>

int main() 
{
    // 将一个返回值为7的 lambda 表达式封装到 task 中
    // std::packaged_task 的模板参数为要封装函数的类型
    std::packaged_task<int()> task([]() {return 7; });
    // 获得 task 的期物
    std::future<int> result = task.get_future(); // 在一个线程中执行 task
    std::thread(std::move(task)).detach();
    std::cout << "waiting...";
    result.wait(); // 在此设置屏障，阻塞到期物的完成
    // 输出执行结果
    std::cout << "done!" << std::endl << "future result is " << result.get() << std::endl;
    return 0;
}
```
这里std::packaged_task，可以用来封装任何可以调用的目标，从而用于实现异步的调用。在封装好要调用的目标后，可以使用 get_future() 来获得一个 std::future 对象，以便之后实施线程同步。

std::future::wait，在前面的参考链接可以看到这样的描述：If the shared state is not yet ready (i.e., the provider has not yet set its value or exception), the function blocks the calling thread and waits until it is ready.

因此这里result.wait()就是设置屏障，等待线程（期物）完成，之后可用get得到期物结果。但是其实示例代码不调用 wait() 也行，因为 std::future::get 中也有描述：If the shared state is not yet ready (i.e., the provider has not yet set its value or exception), the function blocks the calling thread and waits until it is ready. 

也就是说，如果调用get的时候还没有完成期物，那么get函数此时就会阻塞调用线程，知道可以得到期物结果。

于是我修改一下示例代码来验证：

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <Windows.h>

int main() 
{
    // 将一个返回值为7的 lambda 表达式封装到 task 中
    // std::packaged_task 的模板参数为要封装函数的类型
    std::packaged_task<int()> task([]() {Sleep(5000); return 7; });
    // 获得 task 的期物
    std::future<int> result = task.get_future(); // 在一个线程中执行 task
    std::thread(std::move(task)).detach();
    std::cout << "waiting...";
    // result.wait(); // 不调用wait，测试 std::future::get 函数
    // 输出执行结果
    std::cout << "done!" << std::endl << "future result is " << result.get() << std::endl;
    return 0;
}
```
结果就是会在这里等待5秒：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c9a8c4f55b294efeb7cdf5af06c6a5c7.png)
然后再打印结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/73f3743920644863bebe8507ad584476.png)

## 条件变量
条件变量 std::condition_variable 是为了解决死锁而生，当互斥操作不够用而引入的。需要#include<condition_variable>。std::condition_variable的 notify_one() 用于唤醒一个线程； notify_all() 则是通知所有线程。

原文给了一个生产者消费者模型的例子，可以查阅一下。

## 原子操作与内存模型
在汇编层面，我们把单指令的操作称为**原子的（Atomic）**。在Windows中有一套API专门进行一些原子操作，这些API被称为 Interlocked API，这里放一个MSDN链接：
[https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.interlocked?view=net-5.0](https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.interlocked?view=net-5.0)

比如一个著名的例子，++i 代码在很多体系结构上会这样实现：
1. 读取 i 到某个寄存器X
2. X++
3. 将X的内容存储回 i

这样一句代码分为了三个指令，就不会是原子的，多线程不处理好就会出问题。

### 原子操作
这里引入 std::atomic 模板，使得我们实例化一个原子类型，将一个 原子类型读写操作从一组指令，最小化到单个 CPU 指令。例如：`std::atomic<int> counter;`

并为整数或浮点数的原子类型提供了基本的数值成员函数，举例来说， 包括 fetch_add, fetch_sub 等，同时通过重载方便的提供了对应的 +，- 版本。

我们总是可以通过 std::atomic<T>::is_lock_free 来检查该原子类型是否需支持原子操作，毕竟不是所有的类型都能提供原子操作，原子操作的可行性取决于 CPU 的架构以及所实例化的类型结构是否满足该架构对内存对齐 条件的要求。

### 一致性模型
并行执行的多个线程，从某种宏观层面上讨论，可以粗略的视为一种分布式系统。 在分布式系统中，任何通信乃至本地操作都需要消耗一定时间，甚至出现不可靠的通信。因此我们可以通过削弱原子操作的在进程间的同步条件来适当加速程序。

从原理上看，每个线程可以对应为一个集群节点，而线程间的通信也几乎等价于集群节点间的通信。 削弱进程间的同步条件，通常我们会考虑四种不同的一致性模型：线性一致性、顺序一致性、因果一致性、最终一致性。详细描述请看原书第七章7.5。

### 内存顺序
为了追求极致的性能，实现各种强度要求的一致性，C++11 为原子操作定义了六种不同的内存顺序 std::memory_order 的选项，表达了四种多线程间的同步模型：宽松模型、释放/消费模型、释放/获取模型、顺序一致模型。详细描述请看原书第七章7.5。

# 第 8 章 文件系统
文件系统库提供了文件系统、路径、常规文件、目录等等相关组件进行操作的相关功能。和正则表达式库类似，他也是最先由 boost 发起，并最终被合并为 C++ 标准的众多库之一。

这里原书还没有写，就给个链接好了：
[https://en.cppreference.com/w/cpp/filesystem](https://en.cppreference.com/w/cpp/filesystem)


# 第 9 章 其他杂项
## long long int
C99把long long int纳入，但 C++11 才正式把它纳入标准库， 规定了一个 long long int 类型至少具备 64 位的比特数。

## noexcept
C++ 相比于 C 的一大优势就在于 C++ 本身就定义了一套完整的异常处理机制。且C++11 将异常的声明简化为以下两种情况：

- 函数可能抛出任何异常
- 函数不能抛出任何异常

并使用 noexcept 对这两种行为进行限制，使用 noexcept 修饰过的函数如果抛出异常，编译器会使用 std::terminate() 来立即终止程序运行。noexcept 还能够做操作符，用于操作一个表达式，当表达式无异常时，返回 true，否则返回 false。

见我的笔记第11点：
[https://zhuanlan.zhihu.com/p/396032161](https://zhuanlan.zhihu.com/p/396032161)

noexcept 修饰完一个函数之后还能够起到封锁异常扩散的功效，如果内部产生异常，外部也不会触发。

## 字面量
### 原始字符串字面量
C++11 提供了原始字符串字面量的写法，可以在一个字符串前方使用 R 来修饰这个字符串， 同时，将原始字符串使用括号包裹。

比如字符串 C:\File\To\Path ，按传统需要写为：

```cpp
std::string str = "C:\\File\\To\\Path";
```
但是我们现在可以写为：

```cpp
std::string str = R"(C:\File\To\Path)";
```
这样就避免了大量的转义符。注意，这里的 " " 内必须要用小括号包裹。

### 自定义字面量
C++11 引进了自定义字面量的能力，通过重载双引号后缀运算符实现，这里我也直接copy原文代码了，详细需要再看看原文：

```cpp
#include <iostream>
#include <string>

// 字符串字面量自定义必须设置如下的参数列表
std::string operator"" _wow1(const char* wow1, size_t len)
{
    return std::string(wow1) + "hbh";
}

std::string operator"" _wow2(unsigned long long i)
{
    return std::to_string(i) + "hbh";
}

int main()
{
    auto str = "hello_"_wow1;
    auto num = 1_wow2;
    std::cout << str << std::endl;
    std::cout << num << std::endl;
    return 0;
}
```
结果是打印
![在这里插入图片描述](https://img-blog.csdnimg.cn/6bdea1b30099419a83bb77465529c63e.png)
自定义字面量支持四种字面量，详看原文。

## 内存对齐
C++ 11 引入了两个新的关键字 alignof 和 alignas 来支持对内存对齐进行控制。

alignof 关键字能够获得一个与平台相关的 std::size_t 类型的值，用于查询该平台的对齐方式。alignas则是重新修饰某个结构的对齐方式。

参考链接：
[https://en.cppreference.com/w/cpp/language/alignof](https://en.cppreference.com/w/cpp/language/alignof)
[https://en.cppreference.com/w/cpp/language/alignas](https://en.cppreference.com/w/cpp/language/alignas)

示例代码：
```cpp
#include <iostream>
#include <vector>

using namespace std;

#pragma pack(1)
struct Storage
{
    char      a;
    int       b;
    double    c;
    long long d;
};
#pragma pack()

struct VecStorage 
{
    vector<char> m;
};

struct alignas(std::max_align_t) AlignasStorage
{
    char      a;
    int       b;
    double    c;
    long long d;
};

int main() 
{
    std::cout << alignof(Storage) << std::endl;
    std::cout << alignof(VecStorage) << std::endl;
    std::cout << alignof(AlignasStorage) << std::endl;

    return 0;
}
```
如上，输出结果为
![在这里插入图片描述](https://img-blog.csdnimg.cn/d8959b7d37e046e6aa244da0eaa15a62.png)
第一个指定1字节对齐；第二个vector内部有三根指针，指针是4字节，所以结果是4（但是sizeof查看的时候却是16个字节的大小）；第三个 std::max_align_t 在我这个测试中为double（win10，vs2019），所以是8字节。至于以前用过的__attribute__((packed))似乎为GCC特有语法，visual C++好像并不支持。不过一般也用不上，因为字节对齐主要是提高内存的访问效率，强行packed可能会导致出现多个CPU总线周期才能读取到我们想要的数据，降低效率。

## 总结
本节介绍的几个特性是从仍未介绍的现代 C++ 新特性里使用频次较靠前的特性了，noexcept 是最为重要的特性，它的一个功能在于能够阻止异常的扩散传播，有效的让编译器最大限度的优化我们的代码。

# 第 10 章 展望：C++20 简介
这里原书也有很多还未完成的部分，期待作者大大之后补完，一共有：
概念与约束、模块、合约、范围、协程、事物内存。

先放一个参考链接：[https://en.cppreference.com/w/cpp/20](https://en.cppreference.com/w/cpp/20)

## 概念与约束
知乎上有文章：
[https://zhuanlan.zhihu.com/p/107610017](https://zhuanlan.zhihu.com/p/107610017)

这里原书似乎有误，应为sortable而不是Sortable，见链接：
[https://en.cppreference.com/w/cpp/iterator/sortable](https://en.cppreference.com/w/cpp/iterator/sortable)

这里我写了一段代码来测试concept：

```cpp
#include <list>
#include <concepts>
#include <iterator>
#include <vector>
#include <iostream>

using namespace std;

template <typename T>
concept sortable_container = is_same<typename iterator_traits<T>::iterator_category, random_access_iterator_tag>::value;

template<typename T>
requires sortable_container<T>
void hbh_test(T a)
{
    cout << "the container can be sorted by algorithm" << endl;
}

template<typename T>
void hbh_test(T a)
{
    cout << "the container can't be sorted by algorithm" << endl;
}

int main() 
{
    vector<int> hbh_vec;
    hbh_test(hbh_vec.begin());

    list<int> hbh_list;
    hbh_test(hbh_list.begin());

    return 0;
}
```
如上，该代码是想测试一个容器可否被algorithm来sort。我是直接来看它的迭代器类型是否是random_access_iterator_tag，concept 后面需要跟一个bool型，所以我就用stl的is_same，然后用迭代器萃取机去萃取迭代器类型，看他是否和random_access_iterator_tag相等。

于是，满足concept的就会进入

```cpp
template<typename T>
requires sortable_container<T>
void hbh_test(T a)
{
    cout << "the container can be sorted by algorithm" << endl;
}
```
这里使用了 requires sortable_container\<T\>，这些都属于新特性concept的使用。

顺便一提，要是我去掉：

```cpp
template<typename T>
void hbh_test(T a)
{
    cout << "the container can't be sorted by algorithm" << endl;
}
```
那么在`hbh_test(hbh_list.begin());`这一句就会报错，根本就不会让你过编译。我想这也是新特性的用处，提前审查一遍。

## 模块
给一个知乎文章吧：
[https://zhuanlan.zhihu.com/p/350136757](https://zhuanlan.zhihu.com/p/350136757)

# 结语
原书还有一些没写完的内容（第八章、第十章），其他内容都很精炼，以后估计还会来看第二遍。

希望以后能看完这本书：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d9768a14a0304f6b9d2868fea395b406.png)
链接：
[https://www.bookstack.cn/read/CPP-Concurrency-In-Action-2ed-2019/README.md#6hpagg](https://www.bookstack.cn/read/CPP-Concurrency-In-Action-2ed-2019/README.md#6hpagg)

不过听说看英文原文更好，但不管怎样还是辛苦这些作者译者了，站在大佬们的肩膀上是一件幸福的事情。
