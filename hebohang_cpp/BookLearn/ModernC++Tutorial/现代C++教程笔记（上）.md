@[TOC]
# 前言
链接：
[https://changkun.de/modern-cpp/](https://changkun.de/modern-cpp/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/16495d2eeef04b0783ba57027c432764.png)
为了方便，我直接用 VS2019 了，即用 Visual C++ 而没有用作者的 clang++ 测试。

# 第 1 章 迈向现代 C++
C++ 不是 C 的一个超集：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cf2c91c057d9475bac6107a36c632573.png)
在编写 C++ 时，也应该尽可能的避免使用诸如 void* 之类的程序风格。而在不得不使用 C 时，应该注意使用 extern "C" 这种特性，将 C 语言的代码与 C++代码进行分离编译，再统一链接这种做法。

C++特性在GCC/Clang等编译器中的支持情况：
[https://en.cppreference.com/w/cpp/compiler_support](https://en.cppreference.com/w/cpp/compiler_support)

# 第 2 章 语言可用性的强化
## constexpr：
![在这里插入图片描述](https://img-blog.csdnimg.cn/188c7fb27904463a89c984ddc3af0940.png)
C++ 标准中数组的长度必须是一个常量表达式，而对于 len_1 而言，这是一个 const 常数，而不是一个常量表达式，因此（即便这种行为在大部分编译器中都支持，但是）它是一个非法的行为

**C++11 提供了 constexpr 让用户显式的声明函数或对象构造函数在编译期会成为常量表达式。**
比如下图，需要改为 constexpr int len_foo 方可不报错：
![在这里插入图片描述](https://img-blog.csdnimg.cn/eb6767bd2ee341d4b51b4dab264266f6.png)

这个关键字明确的告诉编译器应该去验证 len_foo 在编译期就应该是一个常量表达式。此外，constexpr 修饰的函数可以使用递归，且从 C++14 开始，constexpr 函数可以在内部使用局部变量、循环和分支等简单语句。

## if/switch 变量声明强化
在传统 C++ 中，终没有办法在 if 和 switch 语句中声明一个临时的变量。C++17 消除了这一限制，使得我们可以在 if（或 switch）中完成这一操作。

比如如下代码：
```cpp
#include <iostream>

using namespace std;

int main()
{
	if (int a = 0; a != 3)
	{
		cout << "hbh" << endl;
	}

	return 0;
}
```
报错：
错误	C2429	语言功能 "if/switch 中的 init-statement" 需要编译器标志 "/std:c++17"	

VS2019中在这改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/07d61eb6bed24451a32ba565f6c154a2.png)
改成 C++17 以后就能正常运行了。


## 初始化列表
见我的笔记第五点——initializer_list：
[https://zhuanlan.zhihu.com/p/396032161](https://zhuanlan.zhihu.com/p/396032161)

## 结构化绑定
C++11 新增了 std::tuple 容器用于构造一个元组。我们可以使用 std::tie 对元组进行拆包，但我们依然必须非常清楚这个元组包含多少个对象，各个对象是什么类型，非常麻烦。

C++17 完善了这一设定，给出了 结构化绑定。

测试如下：

```cpp
#include <iostream>
#include <tuple>

using namespace std;

int main()
{
	tuple<int, double, string> my_tuple(1, 2.3, "hbh");

	int my_int;
	double my_double;
	string my_string;

	cout << get<2>(my_tuple) << endl;

	tie(my_int, my_double, my_string) = my_tuple;
	cout << my_string << endl;

	auto [x, y, z] = my_tuple;
	cout << z << endl;

	return 0;
}
```

可以看到，我们可以用get访问，或是tie拆包，但是用结构化绑定的方法，直接 auto [x, y, z] = my_tuple; 实在是方便。上述代码打印出来是3个 hbh 

关于tie：
[http://www.cplusplus.com/reference/tuple/tie/?kw=tie](http://www.cplusplus.com/reference/tuple/tie/?kw=tie)

## 类型推导
auto不用多说，decltype可见我的笔记：
[https://zhuanlan.zhihu.com/p/396032161](https://zhuanlan.zhihu.com/p/396032161)

std::is_same<T, U> 用于判断 T 和 U 这两个类型是否相等。

注意：typename 和 class 在模板参数列表中没有区别，在 typename 这个关键字出现之前，都是使用 class 来定义模板参数的。但在模板中定义有嵌套依赖类型的变量时，需要用 typename 消除歧义

尾返回类型（trailing return type）：
从 C++14 开始是可以直接让普通函数具备返回值推导，因此下面的写法变得合法：

```cpp
template<typename T, typename U>
auto add3(T x, U y){
    return x + y;
}
```

同时 C++14 还开始提供 decltype(auto) 。decltype(auto) 主要用于对转发函数或封装的返回类型进行推导，它使我们无需显式的指定 decltype 的参数表达式。

## if constexpr
C++17 将 constexpr 这个关键字引入到 if 语句中，允许在代码中声明常量表达式的判断条件。

本质上是通过 C++11 引入的 constexpr 关键字，它将表达式或函数编译为常量结果。把这一特性引入到条件判断中去，让代码在编译时就完成分支判断，从而让程序效率更高。

## 面向对象
### 委托构造和继承构造

```cpp
#include <iostream>
class Base {
public:
    int value1;
    int value2;
    Base() {
        value1 = 1;
    }
    Base(int value) : Base() { // 委托 Base() 构造函数
        value2 = value;
    }
};
class Subclass : public Base {
public:
    using Base::Base; // 继承构造
};
int main() {
    Subclass s(3);
    std::cout << s.value1 << std::endl;
    std::cout << s.value2 << std::endl;
}
```
上述代码的结果为：Subclass 通过继承构造调用了 Base(int value)，使得 value2 为 3，而
Base(int value) 又通过委托构造调用了 Base() ，使得 value1 为 1，所以打印结果为 1 3

注意（源于[https://www.runoob.com/w3cnote/cpp11-inheritance-constructor.html](https://www.runoob.com/w3cnote/cpp11-inheritance-constructor.html)）：

1.对于继承构造函数来说，參数的默认值是不会被继承的，并且，默认值会 导致基类产生多个构造函数版本号（即參数从后一直往前面减。直到包括无參构造函数，当然假设是默认复制构造函数也包括在内），这些函数版本号都会被派生类继承。

2.继承构造函数中的冲突处理：当派生类拥有多个基类时，多个基类中的部分构造函数可能导致派生类中的继承构造函数的函数名。

3.假设基类的构造函数被声明为私有构造函数或者派生类是从基类虚继承的，那么就不能在派生类中声明继承构造函数。

4.假设一旦使用了继承构造函数，编译器就不会为派生类生成默认构造函数。这样，我们得注意继承构造函数无參版本号是不是有须要。（比如这里在 Subclass 中新增成员 int b，结果仍然不会变，编译器没有为派生类生成默认构造函数）

### 显式虚函数重载
C++11 引入了 override 和 final 这两个关键字，前者将显式的告知编译器进行重载，编译器将检查基函数是否存在这样的虚函数，否则将无法通过编译；后者则是为了防止类被继续继承以及终止虚函数继续重载引入的。

于我的笔记第11点：
[https://zhuanlan.zhihu.com/p/396032161](https://zhuanlan.zhihu.com/p/396032161)

同时下一节显示禁用默认函数也可见我的笔记，不再记录。

### 强类型枚举
C++11 引入了枚举类（enumeration class），并使用 enum class 的语法进行声明：

```cpp
enum class new_enum : unsigned int {
    value1,
    value2,
    value3 = 100,
    value4 = 100
};
```

再比如示例代码：

```cpp
#include <iostream>

using namespace std;

template<typename T>
std::ostream& operator<<(
    typename std::enable_if<std::is_enum<T>::value,
    std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

enum class a
{
    hbh1,
    hbh2
};

enum class b
{
    hbh1,
    hbh2
};

int main() {
    cout << a::hbh2 << endl;

    return 0;
}
```
上述代码可以正常打印出值1，但是要是改写成：enum a 和 enum b，即不用枚举类，则会报错：

错误	C2365	“hbh1”: 重定义；以前的定义是“枚举数”	

可见：在传统 C++中，枚举类型并非类型安全，枚举类型会被视作整数，则会让两种完全不同的枚举类型可以进行直接的比较（虽然编译器给出了检查，但并非所有），甚至同一个命名空间中的不同枚举类型的枚举值名字不能相同，这通常不是我们希望看到的结果。

# 第 3 章 语言运行期的强化
## Lambda
于我的笔记第13点：
[https://zhuanlan.zhihu.com/p/396032161](https://zhuanlan.zhihu.com/p/396032161)

## std::function
C++11 std::function 是一种通用、多态的函数封装， 它的实例可以对任何可以调用的目标实体进行存储、复制和调用操作， 它也是对 C++ 中现有的可调用实体的一种类型安全的包裹（相对来说，函数指针的调用不是类型安全的）， 换句话说，就是函数的容器。当我们有了函数的容器之后便能够更加方便的将函数、函数指针作为对象进行处理。

原书的两个示例代码写的很好了，建议看原书。

## std::bind 和 std::placeholder
std::bind 是用来绑定函数调用的参数的，std::placeholder 是作为占位符，这在侯捷老师的 STL 源码剖析 中有详细分析。

## 右值引用
见我的笔记：
[https://zhuanlan.zhihu.com/p/397154220](https://zhuanlan.zhihu.com/p/397154220)

补充：

C++11 中为了引入强大的右值引用，将右值的概念进行了进一步的划分，分为：纯右值、将亡值。

**纯右值(prvalue, pure rvalue)**，纯粹的右值，要么是纯粹的字面量，例如 10, true； 要么是求值结果相当于字面量或匿名临时对象，例如 1+2。非引用返回的临时变量、运算表达式产生的临时变量、 原始字面量、Lambda 表达式都属于纯右值。

（需要注意的是，字符串字面量只有在类中才是右值，当其位于普通函数中是左值。）

**将亡值(xvalue, expiring value)**，是 C++11 为了引入右值引用而提出的概念（因此在传统 C++ 中， 纯右值和右值是同一个概念），也就是即将被销毁、却能够被移动的值。

**引用坍缩（折叠）规则**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0dbbd72f028145aa8b24ba91dcf8b6d1.png)
上表翻译一下就是：

- X& &、X& &&、X&& &都折叠成X&

- X&& &&折叠为X&&

参考：
[https://www.jianshu.com/p/b90d1091a4ff?utm_campaign=haruki&utm_content=note&utm_medium=reader_share&utm_source=weixin](https://www.jianshu.com/p/b90d1091a4ff?utm_campaign=haruki&utm_content=note&utm_medium=reader_share&utm_source=weixin)

# 第 4 章 容器
这一章讲了std::array、std::forward_list、无序容器、tuple。 由于我看过 STL源码剖析，大部分都学过，这一章只做关于tuple的一些补充。

补充：

关于元组的使用有三个核心的函数：
- std::make_tuple: 构造元组
- std::get: 获得元组某个位置的值
- std::tie: 元组拆包

## tuple运行期索引——通过std::variant<> 
std::get<> 依赖一个编译期的常量，所以下面的方式是不合法的：

```cpp
int index = 1;
std::get<index>(t);
```

解决方法是使用 C++ 17 引入的 std::variant<> ，提供给 variant<> 的类型模板参数 可以让一个 variant<> 从而容纳提供的几种类型的变量（在其他语言，例如 Python/JavaScript 等，表现为动态类型）

## tuple合并与遍历
合并两个元组，这可以通过 std::tuple_cat 来实现：

```cpp
auto new_tuple = std::tuple_cat(get_student(1), std::move(t));
```

而通过for循环遍历一个元组，首先需要知道它的长度，之后就可以通过之前讲的运行期索引取遍历了。

那么长度我们可以用两种方法：

```cpp
#include <functional>
#include <iostream>
#include <tuple>

using namespace std;

using TupleType = tuple<int, double>;

template <typename T>
auto tuple_len(T& tpl) 
{
    return std::tuple_size<T>::value;
}

int main() {
    TupleType a(1, 1.2);
    
    cout << tuple_len(a) << endl;
    cout << std::tuple_size<TupleType>::value << endl;

    return 0;
}
```
一种是写了一个函数模板tuple_len，另一种干脆直接using TupleType = tuple<int, double>; 不过明显第一种更具通用性。

## tuple小结
std::tuple 虽然有效，但是标准库提供的功能有限，没办法满足运行期索引和迭代的需求，好在我们还有其他的方法可以自行实现。

# 第 5 章 智能指针与内存管理
以下摘抄书原文：

通常的做法是对于一个对象而言，我们在构造函数的时候申请空间，而在析构函数（在离开作用域时调用）的时候释放空间， 也就是我们常说的 RAII 资源获取即初始化技术。

但是对于需要将对象在自由存储上分配的需求，在传统 C++ 里我们只好使用 new 和 delete 去 『记得』对资源进行释放。而 C++11 引入了智能指针的概念，使用了引用计数的想法，让程序员不再需要关心手动释放内存。 这些智能指针就包括 std::shared_ptr/std::unique_ptr/std::weak_ptr，使用它们需要包含头文件 \<memory\> 。

**注意：引用计数不是垃圾回收，引用计数能够尽快收回不再被使用的对象，同时在回收的过程中也不会造成长时间的等待， 更能够清晰明确的表明资源的生命周期。**

这一章节同样在我的笔记：
[https://zhuanlan.zhihu.com/p/397154220](https://zhuanlan.zhihu.com/p/397154220)
中有涉及，故这里仅作补充。最好还是去看看原文。

## std::shared_ptr
std::make_shared 能够用来消除显式的使用 new，所以std::make_shared 会分配创建传入参数中的对象， 并返回这个对象类型的std::shared_ptr指针。

std::shared_ptr 可以通过 get() 方法来获取原始指针，通过 reset() 来减少一个引用计数， 并通过use_count()来查看一个对象的引用计数。
```cpp
auto pointer = std::make_shared<int>(10);
auto pointer2 = pointer; // 引用计数+1
auto pointer3 = pointer; // 引用计数+1
int *p = pointer.get(); // 这样不会增加引用计数
```

## std::unique_ptr
书上说 C++11 没有提供 std::make_unique，但是各家编译器似乎自己实现了。

我们自行实现一个 make_unique 可如下：

```cpp
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args ) {
  return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
```

最近在看D3D12龙书，里面的示例代码就经常会用到make_unique，我在VS2019下查看源码如下（头文件memory中）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7dc1e31ff76d4af99aaa74af631cd799.png)

## std::weak_ptr 
引出弱指针主要是为了解决 std::shared_ptr 循环包含问题（水平有限，不知道还有别的用途没有）：

std::weak_ptr是一种弱引用（相比较而言 std::shared_ptr 就是一种强引用）。弱引用不会引起引用计数增加。

std::weak_ptr 没有 * 运算符和 -> 运算符，所以不能够对资源进行操作，它的唯一作用就是用于检查 std::shared_ptr 是否存在，其 expired() 方法能在资源未被释放时，会返回 false，否则返回 true。

## 总结
智能指针这种技术并不新奇，在很多语言中都是一种常见的技术，现代 C++ 将这项技术引进，在一定程度上消除了 new/delete 的滥用，是一种更加成熟的编程范式。


















