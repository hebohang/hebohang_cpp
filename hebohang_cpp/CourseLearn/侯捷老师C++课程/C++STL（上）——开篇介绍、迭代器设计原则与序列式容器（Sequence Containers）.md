@[TOC]

## 前言
本文为侯捷老师STL课程的一个笔记分享。
课件来源：
https://github.com/19PDP/Bilibili-plus/blob/master/C%2B%2B-STL-HouJie/slide/Slide.pdf
其中让我大受震撼的一个是侯老师所讲的迭代器萃取机（对应目录的12），一个是deque的G2.9与G4.9版本的内部实现（对应目录的15和16），值得去研究。

## 1. 开篇介绍
![在这里插入图片描述](https://img-blog.csdnimg.cn/17cb8aa99cee470db6fa85be616b144b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
STL主要用了模板。在课程前需要正确了解模板的知识。
4个level：
![在这里插入图片描述](https://img-blog.csdnimg.cn/96eb35c9bfef4f3cb1fd1c9daf4cc9d2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/18ee6001d35341db93e80ee9c7a7e754.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
侯老师个人解释：标准库 > STL（六大部件）
旧式headers内的组件不被封装在namespace "std"中

## 2. STL体系结构基础介绍

![在这里插入图片描述](https://img-blog.csdnimg.cn/4de46412b5f940dd82ba74dcafe33b77.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
分配器支持容器，容器是一个模板类，有一些模板函数——算法algorithm（注：在面向对象中，数据和一些算法函数都在一个类里头，但是这里很特别，数据在类里，算法在别的地方，和OO有所不同，是模板编程），算法要处理容器数据，算法和数据之间的桥梁就是迭代器（一种泛化的指针），除外还有仿函数Functor和适配器Adapter，适配器adapter用来做一些转换（像变压器）。
一共六大部件。

![在这里插入图片描述](https://img-blog.csdnimg.cn/1d848e9e61b844f5bb5b218eaf64117f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以不写分配器，容器会默认分配器。分配器也是一个模板，告诉它每次分配的是什么东西。
bind2nd：绑定第二个参数。这里就是求出所有大于等于40的数的个数（用了count_if，带一个条件，原来是小于40，然后再用not1变成是大于等于40的意思）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/c2618c97bd4247c68a3b061b79e41d1e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
所有的容器在设计上（表现它的头尾）都要实现“前闭后开”区间（begin和end），即最后一个元素的下一个元素（即end）不属于容器本身：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a46d0bdb66264381be0da60f38e9d030.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/46568d35a53f4c629ae175c2a990a127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 3. 容器的分类与各种测试
容器的分类：
序列式容器（Sequence Containers）
关联式容器（Associative Containers）：有key和value，方便查找（用key来找东西就非常快）
不定序的容器Unordered Containers（也是一种关联式容器），底部是hash table
![在这里插入图片描述](https://img-blog.csdnimg.cn/b07c5f087b914e64aa7782d32116b7f1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
Set和Map是红黑树实现的。事实上，标准库并没有规定set和map应该用什么实现，但是由于红黑树实现很好所以各家编译器都用红黑树来实现。
Set的key就是value、value就是key。
而Multiset和Multimap表示key可以重复。
目前hash table做的最好的做法是separate chain（链地址法），当然这里一条链子不能太长，所以还会有进一步的细节处理。
![在这里插入图片描述](https://img-blog.csdnimg.cn/87d189d75d9e42059e76e58b559d4fd7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
### 测试array
用qsort，比较方法用上图写的辅助函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/05fca767ac8c4b89bf8f08a2674dbbe5.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
clock()：milli-seconds毫秒

### 测试vector：
![在这里插入图片描述](https://img-blog.csdnimg.cn/99164aaa7aa8432db90555fc59f57896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

find前面的 :: 不加也找得到，是全局的函数find，加了显示的说明是全局的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/38454b4f395b43a0b0135bdd48294207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
### 测试list：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fae00d5e8bb14164a50e10bd55393f93.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
list和forward_list内部自己有一个sort，区别于标准库的sort

### 测试forward_list：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7102cdb49a4c439e9363896084fb8dc9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
注意，单向链表，只有push_front

GNU C++还有非标准库的slist，与forward_list差不多：
![在这里插入图片描述](https://img-blog.csdnimg.cn/02f1744f606b41d4a0df5977823268b2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

### 测试deque：
![在这里插入图片描述](https://img-blog.csdnimg.cn/86fc81c6769946e189a7df062b9b60c4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
两边都可以扩充。由上图右边一段一段地构成，实际是分段连续，但是让操作者使用上感觉是整个连续的。比如通过重载了操作符++
每次扩充一个buffer

![在这里插入图片描述](https://img-blog.csdnimg.cn/797c7b53ff444c928ac9a2042d7ae9bf.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
而其实stack和queue内部都是用deque去实现的。


### 测试stack：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bc34e4bef6114114a82d89101ecc0960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

### 测试queue：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cbf7551fbcdf4c6e868ca7b024a284e5.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

由于stack和queue是由deque实现的，所以从技术上来讲不算容器，叫adapter，不过也无所谓
由于queue有先进先出的性质，所以不会提供iterator的操作（不会提供函数让你得到iterator），否则就会破坏内部结构。stack同理。

### 测试multiset：
![在这里插入图片描述](https://img-blog.csdnimg.cn/07db1523d98848de8b7cd5e7e8e2769c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
底层：红黑树
安插元素：insert
里面有一个全局的find，标准库的；有一个自己的find，multiset的find
可以从上图看到时间。自己的find非常快。因为是关联式容器。
而由于是multiset，所以size仍然是100万


### 测试multimap：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8077ac2a7424452fa93a09f4aa1de670.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
红黑树，每一个元素有key和value

### 测试unordered_multiset：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0e7a58fc6140407c916ea077eacbfa4d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
同样有比较标准库自己的find和容器自带的find，也可以看出容器自带的find快的多。

### 测试unordered_multimap：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f6ea8c5d78fe48dca8adf4bff8e6b36f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

### 测试set：
![在这里插入图片描述](https://img-blog.csdnimg.cn/63acb7949939433f8a91ae8576617a10.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以看到，这里rand的范围是0~32767，所以最终得到的set的size是32768，区别于multiset的1000000

### 测试map：
![在这里插入图片描述](https://img-blog.csdnimg.cn/18a929a3f14644b1a49da7123f86f8d3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以对比multimap的注释，不可以用中括号的形式。但是这里的插入就可以：
710行：c[i] = string(buf); 
第i个元素放右边的东西，i就是key，string(buf)就是value
因为这里key就是i不会重复，所以map的size仍然是1000000

### 测试unordered_set和unordered_map：
和前面讲的的差不多：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7462052a41e74ce0a4917759bca87f20.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/ca0f131bda7d42be9382b8a18b42e602.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
其实早期的这些名称：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f7f2d2a317df4f7f97ee55079de244d2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
现在也能用，只是引用的头文件有变化。

## 4. 分配器之测试

![在这里插入图片描述](https://img-blog.csdnimg.cn/fbde65bd5cb2461a9fe9f885ef2c631b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
GNU C++的测试：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4848cf1a7a52416bbfaf11c070e9d255.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/a7d0fa8b8f0446b7a611ecbbe8d26792.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
上图右边示范直接使用分配器，当然一般我们用不到。只是示范一下。
这里直接用分配器负担很重，因为每次deallocate还需要还当初分配的单元数，所以用的时候应该直接针对容器而不应该去直接使用分配器，小的单元内存不如用new/free（因为不需要记住分配了多少字节，直接free指针就行了）

## 5. 源代码之分布
![在这里插入图片描述](https://img-blog.csdnimg.cn/7c5c8744c0c64ab7a99a8d33e5f335ce.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/5c7e06e2f44349bd9538d3a7ee7203c0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/7e7c44ccf4264f34beb7c32db20a05da.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
## 6. OOP vs GP
![在这里插入图片描述](https://img-blog.csdnimg.cn/d4cc185b408f45599b8ebbdff82fee0a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/6b3ea385ecb04a8d864fbec32d3f9c44.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以从上图看到全局的sort函数的两种形式（algorithm中）
![在这里插入图片描述](https://img-blog.csdnimg.cn/9f904d8a73e84f8b9fea753afe187813.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
那么为什么list要用OOP单独放一个sort操作呢？再回到下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8d7f3db658dd405ab1efdfe2179113ce.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
因为链表不能通过iterator+5之类的操作到下一个迭代器去，链表在内存里并不是一个连续空间，迭代器不能跳来跳去，只能一个一个前进或是后退，而从上图右侧标准库的sort算法可以看到first + (last - first) / 2，这样的操作是链表这样的不能支持的。所以要自带sort

![在这里插入图片描述](https://img-blog.csdnimg.cn/de318fac57fd43a2b5bc6f10357cfd40.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 7. 基础：操作符重载与模板
![在这里插入图片描述](https://img-blog.csdnimg.cn/a5d22af77ce64668a61323a8d08979f8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/5d2aabdaa105449e9b2c7f23905ade77.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/ac274a73f1b642919a4c720768e5526a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/3a8fb79a061044cfac3825459396bcdb.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/0bae77f8671d4783ad3e8d3a2e291329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以看到类模板没有这样的信息，无法实参推导。函数模板则略有不同。

这里没有讲到成员模板，PPT也放一下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3f71b38038964ca5948806e7d6cd90f9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)


类模板还有泛化与特化，可以观察一下写法：

![在这里插入图片描述](https://img-blog.csdnimg.cn/bc673ff40b6a4c4b8e6d01dc622083df.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/b4e54dbe2da34673b6da2d248e55f807.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
只要看到template<>就应该是要去特化。

![在这里插入图片描述](https://img-blog.csdnimg.cn/10d6fcefaf5d40a0b6c03abf707ce0e1.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

类模板又衍生出来偏特化：
![在这里插入图片描述](https://img-blog.csdnimg.cn/aa68182e614245d88719ad266705a279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 8. 分配器
一般不会用它，但是非常重要。
![在这里插入图片描述](https://img-blog.csdnimg.cn/596bcfef592f4530a6b72828a46d603a.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
所有的分配动作，最后一层一层下去都会到malloc中。这个函数再根据不同的操作系统去调用各自对应的API（system api）去拿到真正的内存。
所以标准层面上最终都会跑到malloc中去。
这里还有这样一个函数 operator new，和new的关系可以看之前的笔记（面向对象下）。
而malloc分配的内存如上图右侧，细节将在内存管理中讲述。那么也可以看到对于小块内存开销的比例就更大。

面向对象（下）笔记：
![在这里插入图片描述](https://img-blog.csdnimg.cn/11b477be40114e8fa97bebbedd1a2e21.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
VC6例子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/974fd40930a8485087c7d34d60eae617.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/4a854d9d83b242738cbc6c4e4d7f802c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
分配器最重要的函数就是allocate和deallocate，而allocate则会调用operator new，而operator new最终则会调用malloc。deallocate则是调用operator delete，而operator delete则最终调用free，与malloc配合。
示范中（allocator<int>()），一个typename加()就会形成一个临时对象（object）。
当然，不鼓励直接用分配器。

BC5例子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/23d363254b434c41a38afe3d801e7fbf.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/f2de526b513e4b46995def1679aaca05.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
G2.9的例子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ea03fd1338ea45e882e033e81952a6de.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
结论：VC、BC、GNU C的分配器都一样，最终会使用malloc和free来实现，最终会有malloc的额外开销。如果区块小，这个开销所占比例大，就不太能接受。
而上图右侧说明说使用的时候是另一个分配器的设计。
接着往下看：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d58282cf6566436b9abaf7b9f7a98c54.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
那么这个真正的分配器设计alloc长什么样呢？
它的运行情况如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/700f6091d1f24daca0ae687269d51652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

一个自然的想法就是想去避免malloc的一些额外开销，那么这些额外开销究竟在干什么呢？
回到这张图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/5c30b6fed7ba4634b1bd34970af08968.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
size上下两端习惯叫cookie，记录着整块的大小。这样free才知道回收多大内存。cookie从上图也可以看出，它上下都有。（size的上下）
但是容器的元素大小是固定的。比如说一百万个元素，一个元素是8个字节，前面却要记录一百万次这个元素是8字节，完全没有必要。所以在容器的应用情况下可以不需要cookie，所以GNU C就是从这个情况下着手。

![在这里插入图片描述](https://img-blog.csdnimg.cn/4066cd6e4e9f424ba638e18c293d33ff.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
比如上图，第0号链表负责的是8个字节的大小，第7号就是负责8 * 8 = 64个字节的大小，依次类推，最后一个15号，负责的就是16 * 8个字节的大小。
所有的容器需要内存的时候都来往这个分配器要内存。而容器的元素大小因此就会调整到8的倍数，比如元素大小是50字节就会调整到56字节，那么此时就是第6号链表（7 * 8 = 56），然后就会看这号链表里面有没有内存块，如果没有就会调用malloc去向操作系统要一大块去切割，然后用单向链表去存储。所以这样切出来的一大块就会不带cookie，就没有这样的额外开销。

那么比如有一个容器，放一百万个元素，这样做，由于这一百万个元素都不带cookie，于是就可以省下800万个字节的开销。这可不是小数目。当然这是一个直观的想象，实际上每一次调malloc要一大块内存的时候都会带有一个cookie，那么按照我的理解，如果一次malloc想要分配100万个元素的空间，那么就只会带一个8字节的cookie，这当然可以忽略不计。
这是GNU C尤其是alloc的分配器所表现出来的好处。
不过也有缺陷，需要在后面的内存管理章节去讲。

不过经过比较这一种分配器（alloc）是最好的。
上面是2.9版的，那么对于新版的情况，这里谈G4.9，却不再是原先的alloc：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4f73ce8799a24a00b8acc6dda05e30da.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
实现如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/212c6cdd400746aabadfa53760ab5284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里发现到4.9版的情况又回到了最初的设计。那么为什么不用那个2.9版的alloc呢？还无从得知。侯老师也不明白。
当然，原先的alloc还在使用（侯老师看了源代码，确信设计是差不多的）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b7ffabc44e1f4ddebe752cdd01cb58b4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 9. 容器之间的关系与分类
![在这里插入图片描述](https://img-blog.csdnimg.cn/d323101b17484695a6dce0afd217d3fd.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/3ed99dcf373f4b29a2c6180cda46d6ae.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
早期的时候，标准库很少有继承关系，更多的是复合。
这里上图缩排进去，如rb_tree红黑树缩排进去，表示接下来的set、map、multiset、multimap里头都有一个rb_tree。同理有heap（容器的heap）和priority_queue里头有一个vector，stack和queue里头有一个deque。
平常一个class A要用class B的东西，一个是继承，A继承B，一个是复合，A里头拥有一个B，这样A都可以使用B 的东西。

## 10. 深度探索list（上）
![在这里插入图片描述](https://img-blog.csdnimg.cn/d1df639d9149420f982e56d57e7eff30.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以看到G2.9版本一个list内部就一个指针，指向一个节点，所以是4字节（2.9版本，早期版本）。

链表是一个非连续空间，所以不希望一个iterator是单纯的一个指针，否则++就不会按链表去走。所以希望iterator足够聪明，++的时候会去看next指针指向list下一个节点。所以iterator是一个“聪明的指针”，所以会设计成一个class
再来看用法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9d98381adbb541fc88f398002b72a68e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
由于要模拟指针，所以要去实现++、--等操作，且每个iterator的实现类至少要有这五个typedef，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bfacbc937cf14d96877e74a6edc8e013.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
而这里有两个++，其实就是前置型的++和后置的++。C++的语言规格说，前++（即++i）没有参数，而后++有参数（即i++，要把原来的东西记起来，然后++完再传回原来的东西），但是这个参数无意义，只是为了区分。
![在这里插入图片描述](https://img-blog.csdnimg.cn/40e3d57dd5cc4c458b569c2f715d27f4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
所以通过重载++就实现了iterator的++操作，指向list的下一个节点。注意，这里的实现的node是iterator类中的node，注意区分。
而这里尤其注意后++的实现，遇到第一个 “ * ”以为会去看重载的 “ * ”，但是不然，由于先遇到重载的赋值号 = ，所以不会去看重载的星号（如上图），即不会调用operator *。依次类推 2 和 3 。
源代码简短，但是分析需要一定的功夫。
这里的“偶像”，要向谁看齐，应该是要向整数看齐。整数可以怎么做就应该要怎么做。比如上图左下方的操作。C++的整数不允许后++两次，要向它看齐，所以这里的后++传回来的不是引用（reference），而前++是可以两次的，所以传回来的是引用（reference）。就是这个原因。
list的迭代器iterator有很多的操作符重载，这里拿++举例。

![在这里插入图片描述](https://img-blog.csdnimg.cn/ebdca359af484017ba9877b63e561331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
## 11. 深度探索list（下）
再回到这张图（G2.9版本的设计）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2a20846eabb1408e801308ee8ae1d964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里的list_iterator的模板参数有三个，且在list_node中的pointer居然是void*，这就免不了要多次转型，很浪费，按理不是一个好的写法。这些都在新版中有了改进。
这里来做一个对比：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6a76e1b52eb3485399f97c6c3beb2f34.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
2.9版的3个（模板）参数变为了1个参数就够了，传入后再用typedef去定义。当然是只传一个参数好，有进步。
且4.9版节点的设计分成了两块，同样是两根指针加一个data，且这里把void*改成了指向节点自己，把那些不够好的地方都进行了修正。

G2.9版设计相对于4.9就简单多了，而4.9非常复杂，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0d74b6f46b0e4e49b6d9d9ab2da61d4b.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

我们回到2.9版，便于理解：
![在这里插入图片描述](https://img-blog.csdnimg.cn/300eaed515cd4b908981153a33a85a02.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
上图的灰色节点就是之前我们讲过的，一定要实现“前闭后开”区间，灰色的便是那个“开”的end

4.9版也是如此，如下图淡蓝色部分：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c79fdda5598e46618c5b4b0867fad520.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里的list大小是8，与2.9版不同，而list自己本身大小是0，继承的父类里面复合着_List_impl，这里面继续看复合着_List_node_base，里面有两个指针，所以大小是4 * 2 = 8字节。
而2.9版则是list自己指向一个节点__list_node，而节点再包含data、prev、next成员

![在这里插入图片描述](https://img-blog.csdnimg.cn/74308dbf3a354429a2d6dfefaac2d8dd.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
之前说了链表的iterator里头有一堆typedef，而里头有五个typedef是必须的，这就牵扯到traits了。

## 12. 迭代器的设计原则和iterator Traits的作用与设计
traits，英文释义为特征，特性，特点。
所谓traits就是一种人为制造的萃取机。一种机器。希望丢给它什么东西就萃取出你想要的特征。

参考：
https://www.cnblogs.com/mangoyuan/p/6446046.html

traits，又被叫做特性萃取技术，说得简单点就是提取“被传进的对象”对应的返回类型，让同一个接口实现对应的功能。因为STL的算法和容器是分离的，两者通过迭代器链接。算法的实现并不知道自己被传进来什么。萃取器相当于在接口和实现之间加一层封装，来隐藏一些细节并协助调用合适的方法，这需要一些技巧（例如，偏特化）。

标准库有好几种traits，这里谈谈针对iterator的traits（还有type traits、character traits、pointer traits等等）。即萃取出iterator的特性，
首先来看看iterator遵循的原则：
![在这里插入图片描述](https://img-blog.csdnimg.cn/214c6aed992b45629f9710f0146b237f.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
上图1处的想要萃取处的iterator_category分类，所谓的iterator的分类就是它的移动性质，有的只能++，有的还能--，有的还可以跳着走（一次+很多、-很多）。
2和3处的difference_type和value_type，value_type就是容器的元素的type，difference_type就是两个iterator之间的距离应该用difference_type去表现。
所以算法（比如这里的1、2、3）问iterator，然后回答出这三个问题。而上图右下角所述，C++标准库一共设计出了五种：category、difference（距离）、value_type、reference、pointer，而最后这两种从未在标准库中被使用过。
这五种type被称为迭代器的associated_type，相关的类型。迭代器必须定义出来以便回答算法的提问：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bb85de0747894f9c815c8349b81fe5c8.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
如上图，value_type就是链表的元素类型 T，category就是表现链表迭代器是双向类型的bidirectional_iterator_tag
而指针也算一种iterator，是一种退化的iterator，而它不是class形式的，无法回答这五个问题，就要加一个中间层萃取机，萃取机就需要进行区分。

补，之前所述的list的设计：
![在这里插入图片描述](https://img-blog.csdnimg.cn/11345939cad94e65b41359aaf7388df3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20e404f568a0489fa88d1eb76afbc549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/376261991c674ae8945541e329256646.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
算法想知道I的五种相关类型，就来问traits，traits就转问 I （如上图）
这里通过了模板的偏特化来进行区分。如上图的1、2、3.
而这里iterator若是const int* ，其萃取出来的value_type应该是int而非const int，如上图的3与右下角的解释。

![在这里插入图片描述](https://img-blog.csdnimg.cn/d957056b8c264b3bbe535f92cf2fb432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/9d4f17daaaaa4ec4929751cab1ccf619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 13. 深度探索vector

![在这里插入图片描述](https://img-blog.csdnimg.cn/f139c959bca34cfa8346a9e3bc27588e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
事实上没有什么东西能原地扩充，因为不知道后面的空间是否使用。vector也是如此，每次会去找一个可以扩充的空间。
最终没找到两倍大的空间，这个容器就失败（失效）了。
如上图右侧实现，vector类自身就是三根指针（start、finish、end_of_storage），所以其本身大小是12个字节。

![在这里插入图片描述](https://img-blog.csdnimg.cn/2739aab12a124d52986622840fab1e10.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
从下图也可以看出两倍增长（特殊情况：当old_size为0的时候成长为1）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/209c931f75fb4aeaa802c103fdf663d2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
既然vector内部是连续的，按理说指针就能当作迭代器。G2.9版也确实是这样：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c84dd9d4887b47c984b31e8a6d299885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
而G4.9版就设计的很复杂了（侯老师个人不太欣赏设计的这么复杂）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4728b6a6361c4f45afbd20876cb5ee57.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
同样通过继承复合关系去看vector的大小，发现是3根指针12个字节。
这里public继承侯老师认为不太理想，因为public继承有一种“是一种”的意思，这里_Vector_impl怎么会是一种分配器呢？只是想使用分配器里的函数，应该用private继承才合理...

![在这里插入图片描述](https://img-blog.csdnimg.cn/88ce1581252b4f73a4c78f2c9c098790.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/8853b685dd4c4df8b7a1326467b77258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 14. 深度探索array、forward_list
array本来很熟悉了，C++的数组，但是标准库又把它封装成了一个类，一个容器，就必须提供迭代器，然后迭代器就必须提供之前所述的五个信息，以便算法后续调用。
如果没有这么包装的话，array就会在六大部件之外，就不能享受算法、仿函数等的功能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/1dddea3426f24004be5604053a9d32e9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
可以看到，其他元素是可扩充的，但是array要使用时必须指定大小。
array没有ctor和dtor（构造和析构）
array是一个连续空间，只要是连续空间就可以使用指针当迭代器，不必那么复杂。

同样，4.9版又变得非常复杂了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fdd4a879d32049a3a82a50c3773208fe.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/c17e73bc19624e5caec0a67d1b217207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

## 15. 深度探索deque、queue、stack（上）
deque：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fdfb33820107451192188b3c0f5a44e0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
一个vector（图中的map，源代码是用vector实现的，也被叫做控制中心）内放置指向各个缓冲区（buffer，也有人叫节点）的指针，需要扩充的时候就新分配一个缓冲区，然后把指针放入vector中。

再看上图的迭代器的设计，这个迭代器的node就指向控制中心对应的位置，first和last就指向缓冲区的边界位置，用作标志。所以当iterator每次++或者--的时候，就会通过first和last去判断是否走到边界，然后若已经到达边界要跳到下一个buffer的时候，就通过node回到控制中心去找到对应的位置。
第一个cur就是当前指向的元素。

几乎所有的容器都提供begin和end，对应上图中begin就对应start，end就对应finish。

再来看看对应的源码（侯老师讲解主要是以G2.9版本讲）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/15f26412fe1c495d99d5ad0ef51f7150.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
从上图可以看到，一个deque有两个迭代器，下面讲了一个是16字节，然后这里的map_size 4字节，map 在这里定义是一个T**，4字节，所以一共40字节。
这里的模板参数BufSiz就是每个buffer容纳的元素个数。所以G2.9版允许我们指定buffer的大小，但是新版是不允许的。

从下图可以看到，一个迭代器内部四个指针，大小4 * 4 = 16字节：
![在这里插入图片描述](https://img-blog.csdnimg.cn/9188bf3069f64ce28a47960051c37f1e.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
从上图我们接着看到迭代器定义的这五个类型。
分类是random_access_iterator_tag，制造出deque是连续的这样一种假象。所以这样分类是合理的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/15be217158ce417fa13064f45f0062c2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里insert有个考量，毕竟deque是可以两端扩充的，所以要是插入的时候前端元素比较少，那就应该往前推而不是往后推（效率高）。所以这里可以看到如果insert是头就push_front，是尾端就是push_back，其他就是insert_aux辅助函数。

![在这里插入图片描述](https://img-blog.csdnimg.cn/dff4064ff6b04b85b15451af2daf1b83.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
对于辅助函数的实现就会去看安插点之前的元素和之后的元素个数对比了。很精妙。

## 16. 深度探索deque、queue、stack（下）

![在这里插入图片描述](https://img-blog.csdnimg.cn/2b1b9f0c835746729562cd4ecf1c38e3.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
前面讲了前闭后开的概念。这里也可以看到，back()中会取finish，然后再--操作。
这里size也很清晰了。迭代器直接重载操作符“-”就行了，然后去看两端buffer所用的大小，还有中间有几个buffer就可以计算了。比如上图两端是3+0，中间3个buffer就是3*8 = 24.

所以我们也能看到，模拟连续空间都是迭代器的功劳：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a5a2890cab6d45409ec4415703592de0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里看迭代器内部重载“-”的实现，即两个迭代器之间的距离（多少个元素在其间）。

这里++的操作要去看cur是否到达了迭代器边界，没到达边界就简单的一p，到达边界就要跳转到下一个缓冲区去了（先退回控制中心，再找到下一个换冲区）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d9c8f3a7ce724c248f5e9bf4c21ca995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
当然--操作也类似。
通过这里迭代器的重载++和--操作，还有+=和+等操作等等，让使用者感受不到其实空间并不连续：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b40cd5c3f2954e33bdb75b0dd4dae1e0.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/932b946fff954f218150aea330ea13a7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里还有一个很有意思的实现，可以看到上图的operator-=实际上用了+=去实现。
而只要连续空间（或是假象）就应该提供【】这样的操作符（如上图）。

同样，再来看看4.9版本，又变得复杂了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/a0f762f381dd47d79ff35fdee507af66.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
每个容器都设计成这样四个class。本身、base、impl、allocator，本身继承base，base内复合impl，impl继承分配器。
而新版本把buffer_size这个参数拿掉了，取而代之的如上图右下角的注解，自动根据元素类型去判断buffer应该多大。

![在这里插入图片描述](https://img-blog.csdnimg.cn/aae1ec7dc26042f696aeb6c84ed2b0d9.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
这里还有一个聪明的考量，就是控制中心是一个vector，而这里每次扩充成两倍的时候，搬移元素不是搬移到新的控制中心vector的前面，而是搬移到中间（中段），这样便于两端扩充（非常聪明）。

我们再来看看stack和queue：
![在这里插入图片描述](https://img-blog.csdnimg.cn/16dff07d4af9476fa4be9d3f1d71a4ae.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
其实就是内含一个deque，然后封锁住一些功能。
因为这样的实现，所以有时候也不会把queue叫做一个容器，转而叫做一个适配器。

stack也是同样的道理：
![在这里插入图片描述](https://img-blog.csdnimg.cn/d499786c33d4469880cee6923a28dd92.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/89d607fb3fd1426f8a7d4ba8c4fa22f7.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
很有意思的一件事，如上图，stack和queue还能选择list作为底部支撑（如前面的stack和queue的实现，只要能提供那些如back、push_back之类的实现函数就能作为底部支撑），而这里默认的底部支撑是deque，按道理应该deque实现起来更快，但是没有去做进一步分析实验了。
还有一件事就是，这两者都不允许遍历，不提供迭代器。因为一个先进先出一个先进后出是全世界公认的，所以要拿元素只能从头端拿或尾端拿。

接着往下看，还有一些有意思的事：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b73e21919a3c4ad58c51e059d2a29a62.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/8706dd6107b54253b6e07d1bda677c8d.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NzY1MjAwNQ==,size_16,color_FFFFFF,t_70)
底部支撑的考量就是要调用转调用是否底部支撑能提供的到。



































