# C++设计模式——“对象创建”模式

[TOC]

通过“对象创建” 模式绕开new，来避免对象创建（new）过程中所导致的紧耦合（依赖具体类），从而支持对象创建的稳定。它是接口抽象之后的第一步工作。



一共分为四个模式，其实非常接近，只是演化过程中有一些不同：

- Factory Method 
- Abstract Factory 
- Prototype 
- Builder



# 工厂方法（Factory Method）

学习设计模式首先要明晰问题：干嘛需要这样一个设计模式。



举个例子，假设本来有一些分割的设计实现，那么一般我们会选择将他们继承一个抽象基类：

```cpp
class ISplitter{
public:
    virtual void split()=0;
    virtual ~ISplitter(){}
};

class BinarySplitter : public ISplitter{
    
};

class TxtSplitter: public ISplitter{
    
};

class PictureSplitter: public ISplitter{
    
};

class VideoSplitter: public ISplitter{
    
};
```

那么我们要是直接写，就会写成这样：

```cpp
class MainForm : public Form
{
	TextBox* txtFilePath;
	TextBox* txtFileNumber;
	ProgressBar* progressBar;

public:
	void Button1_Click(){
		ISplitter* splitter = new BinarySplitter();//依赖具体类
        
        splitter->split();
	}
};
```

比如这里我们需要new一个BinarySplitter，为了面向接口编程，我们会用 ISplitter* splitter而不是 BinarySplitter* splitter，这便是面向接口编程。



面向接口编程最简单的一个表现形式就是对象要声明成抽象基类。因为我们有依赖倒置原则，依赖抽象而不是依赖实现细节，所以需要面向接口编程。



但是后面怎么办呢？



首先 ISplitter* splitter 已经产生一个编译式依赖了：需要ISplitter的存在才能编译通过。但是后面的 new BinarySplitter() 是一个细节依赖（依赖了具体类），这就有了问题：代码中哪怕只出现一处细节依赖，哪怕其他都是抽象依赖，也解决不了问题。因为会打破依赖倒置原则（实现了编译时的细节依赖）。然而毕竟是抽象基类 ISplitter ，所以肯定不能直接new一个ISplitter出来，那么怎么办呢？



因此引出了这四种“对象创建”模式。即最开头的那句话：

通过“对象创建” 模式绕开new，来避免对象创建（new）过程中所导致的紧耦合（依赖具体类），从而支持对象创建的稳定。它是接口抽象之后的第一步工作。



而如果我要是改成这样：

```cpp
//工厂基类
class SplitterFactory{
public:
    virtual ISplitter* CreateSplitter()
    {
        return new BinarySplitter();
    }
};
```

可以看到则还是会回到最初的样子：产生了编译时依赖。这样不好，我们应当弄成运行时依赖：即借助虚函数的办法。



因此为了依赖抽象，我们就可以定义抽象基类了：

```cpp
//抽象类
class ISplitter{
public:
    virtual void split()=0;
    virtual ~ISplitter(){}
};


//工厂基类
class SplitterFactory{
public:
    virtual ISplitter* CreateSplitter()=0;
    virtual ~SplitterFactory(){}
};
```

虚函数就是一种延迟，延迟到运行时。可以从上面看到，这样的工厂基类就绕开了new，因为要new就会面向依赖具体类的矛盾，就会导致紧耦合。



因此最终代码结构长这样（只是示意该设计模式，暂不考虑他们的内存管理，之后的笔记也会是这样）：

抽象基类：

```cpp
//抽象类
class ISplitter{
public:
    virtual void split()=0;
    virtual ~ISplitter(){}
};


//工厂基类
class SplitterFactory{
public:
    virtual ISplitter* CreateSplitter()=0;
    virtual ~SplitterFactory(){}
};
```

具体类和具体工厂：

```cpp
//具体类
class BinarySplitter : public ISplitter{
    
};

class TxtSplitter: public ISplitter{
    
};

class PictureSplitter: public ISplitter{
    
};

class VideoSplitter: public ISplitter{
    
};

//具体工厂
class BinarySplitterFactory: public SplitterFactory{
public:
    virtual ISplitter* CreateSplitter(){
        return new BinarySplitter();
    }
};

class TxtSplitterFactory: public SplitterFactory{
public:
    virtual ISplitter* CreateSplitter(){
        return new TxtSplitter();
    }
};

class PictureSplitterFactory: public SplitterFactory{
public:
    virtual ISplitter* CreateSplitter(){
        return new PictureSplitter();
    }
};

class VideoSplitterFactory: public SplitterFactory{
public:
    virtual ISplitter* CreateSplitter(){
        return new VideoSplitter();
    }
};
```

最终函数：

```cpp
class MainForm : public Form
{
    SplitterFactory* factory; //工厂

public:
    
    MainForm(SplitterFactory*  factory){
        this->factory = factory;
    }
    
	void Button1_Click(){
		ISplitter* splitter = factory->CreateSplitter(); //多态new
        splitter->split();
	}
};
```

把创建交给未来：通过虚函数变成运行时依赖，绕开new所产生的依赖具体类矛盾。



> “把变化当作一只猫，我们只能把它关到笼子里（局部的变化、依赖），而不能让它在整个屋子（代码）里到处乱跑。”



最后回到书本上的定义：

> 定义一个用于创建对象的接口，让子类决定实例化哪一个类。 Factory Method使得一个类的实例化延迟（目的：解耦， 手段：虚函数）到子类。
>
> ——《设计模式》GoF



UML结构图：

![image-20211020162104049](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020162104049.png)

与前面的代码对应：Product就是ISplitter，ConcreteProduct就是那些具体的Splitter，Creator就是抽象的工厂基类SplitterFactory，而ConcreteCreator就是那些具体的工厂类如BinarySplitterFactory、TxtSplitterFactory等。而MainForm只和最上面的Product和Creator依赖。



红色部分是稳定的，蓝色部分是变化的。**看到任何一个设计模式的类图的时候一定要马上看出谁是稳定的谁是变化的。**



### 总结：

- Factory Method模式用于隔离类对象的使用者和具体类型之间的耦合关系。面对一个经常变化的具体类型，紧耦合关系(new)会导 致软件的脆弱。 
- Factory Method模式通过面向对象的手法，将所要创建的具体对象工作延迟到子类，从而实现一种扩展（而非更改）的策略，较好地解决了这种紧耦合关系。 
- Factory Method模式解决“单个对象”的需求变化。缺点在于要求创建方法/参数相同。



# 抽象工厂（Abstract Factory）

动机：

在软件系统中，经常面临着“**一系列相互依赖的对象”的创建工作**；同时，由于需求的变化，往往存在更多系列对象的创建工作。



来看具体例子：

```cpp
class EmployeeDAO{
    
public:
    vector<EmployeeDO> GetEmployees(){
        SqlConnection* connection =
            new SqlConnection();
        connection->ConnectionString("...");

        SqlCommand* command =
            new SqlCommand();
        command->CommandText("...");
        command->SetConnection(connection);

        SqlDataReader* reader = command->ExecuteReader();
        while (reader->Read()){

        }
    }
};
```

假设任务是要写一个数据访问层要创建一系列的对象，假设访问的是Sql数据库。但是有各种各样的数据库，以后可能会有”变化“，因此像上面这样绑死的写new的方式是不合适的。



通过之前的工厂模式的学习，我们很快写出这样的代码（这里写一起便于理解）：

```cpp
//数据库访问有关的基类
class IDBConnection{
    
};
class IDBConnectionFactory{
public:
    virtual IDBConnection* CreateDBConnection()=0;
};


class IDBCommand{
    
};
class IDBCommandFactory{
public:
    virtual IDBCommand* CreateDBCommand()=0;
};


class IDataReader{
    
};
class IDataReaderFactory{
public:
    virtual IDataReader* CreateDataReader()=0;
};
```

基类代码大概像上面这样，那么不同的数据库的支持就会如下面这样继承：

SQL的：

```cpp
//支持SQL Server
class SqlConnection: public IDBConnection{
    
};
class SqlConnectionFactory:public IDBConnectionFactory{
    
};


class SqlCommand: public IDBCommand{
    
};
class SqlCommandFactory:public IDBCommandFactory{
    
};


class SqlDataReader: public IDataReader{
    
};
class SqlDataReaderFactory:public IDataReaderFactory{
    
};
```

Oracle的：

```cpp
//支持Oracle
class OracleConnection: public IDBConnection{
    
};

class OracleCommand: public IDBCommand{
    
};

class OracleDataReader: public IDataReader{
    
};
```

那么最终的上层：

```cpp
class EmployeeDAO{
    IDBConnectionFactory* dbConnectionFactory;
    IDBCommandFactory* dbCommandFactory;
    IDataReaderFactory* dataReaderFactory;
    
    
public:
    vector<EmployeeDO> GetEmployees(){
        IDBConnection* connection =
            dbConnectionFactory->CreateDBConnection();
        connection->ConnectionString("...");

        IDBCommand* command =
            dbCommandFactory->CreateDBCommand();
        command->CommandText("...");
        command->SetConnection(connection); //关联性

        IDBDataReader* reader = command->ExecuteReader(); //关联性
        while (reader->Read()){

        }
    }
};
```

这样只能勉强解决问题，但是Connection、Command、dataReader它们是有关联的：它们要么同是SQL的，要么同是Oracle的，如果Connection是SQL而Command是Oracle的就会报错。所以还是有点问题。



那么这仨都有相关性，那干嘛要用一个工厂呢？高内聚松耦合，因此这里就引出了抽象工厂（Abstract Factory）：

基类与工厂基类：

```cpp
//数据库访问有关的基类
class IDBConnection{
    
};

class IDBCommand{
    
};

class IDataReader{
    
};


class IDBFactory{
public:
    virtual IDBConnection* CreateDBConnection()=0;
    virtual IDBCommand* CreateDBCommand()=0;
    virtual IDataReader* CreateDataReader()=0;
    
};
```

支持Sql的：

```cpp
//支持SQL Server
class SqlConnection: public IDBConnection{
    
};
class SqlCommand: public IDBCommand{
    
};
class SqlDataReader: public IDataReader{
    
};


class SqlDBFactory:public IDBFactory{
public:
    virtual IDBConnection* CreateDBConnection()=0;
    virtual IDBCommand* CreateDBCommand()=0;
    virtual IDataReader* CreateDataReader()=0;
 
};
```

最终的上层：

```cpp
class EmployeeDAO{
    IDBFactory* dbFactory;
    
public:
    vector<EmployeeDO> GetEmployees(){
        IDBConnection* connection =
            dbFactory->CreateDBConnection();
        connection->ConnectionString("...");

        IDBCommand* command =
            dbFactory->CreateDBCommand();
        command->CommandText("...");
        command->SetConnection(connection); //关联性

        IDBDataReader* reader = command->ExecuteReader(); //关联性
        while (reader->Read()){

        }
    }
};
```



最后回到本书定义：

> 提供一个接口，让该接口负责创建一系列“相关或者相互依赖的对象”，无需指定它们具体的类。
>
> ——《设计模式》GoF



同样最后回到UML结构图：

![image-20211020165257731](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020165257731.png)

AbstractFactory是稳定的，这里相当于IDBFactory，里头的两个方法：CreateProductA和CreateProductB就相当于里头的两个函数CreateDBConnection和CreateDBCommand；

AbstractProductA和AbstractProductB就相当于IDBConnection和IDBCommand（当然这里都只显示两个作为例子），ProductA1和ProductB1就相当于SqlConnection和SqlCommand，A2和B2同理换成Oracle的。

而ConcreteFatory1和ConcreteFatory2就相当于SqlDBFactory和OracleDBFactory，CreateProductA和CreateProductB就相当于SqlDBFactory和OracleDBFactory中的CreateDBConnection和CreateDBCommand方法。



图中红色表示稳定的，绿色为第一种变化，蓝色为第二种变化。



同样我们可以看出，工厂方法模式其实就是抽象工厂的一种特例：只不过后者是针对一系列“相关或者相互依赖的对象”罢了。



### 总结

- 如果没有应对“多系列对象构建”的需求变化，则没有必要使用 Abstract Factory模式，这时候使用简单的工厂完全可以。 
- “系列对象”指的是在某一特定系列下的对象之间有相互依赖、 或作用的关系。不同系列的对象之间不能相互依赖。 
- Abstract Factory模式主要在于应对“新系列”的需求变动。其缺点在于难以应对“新对象”的需求变动。



”任何一个模式都有缺点。它稳定的地方就是它的缺点（不能够变化）“。比如这里的缺点就在于难以应对“新对象”的需求变动。

> 因此如果极端地假设任何一个地方都在变化，那么就没有设计模式适用；同样，要是假设任何一个地方都稳定，那干嘛还需要用模式呢？直接写不就完了。



# 原型模式（Prototype）

前两个模式用的非常多，后两个Prototype和Builder模式则相对较少。



Prototype实际上也是Factory Method的一个变体模式。回想一下之前的工厂模式：

```Cpp
//抽象类
class ISplitter{
public:
    virtual void split()=0;
    virtual ~ISplitter(){}
};


//工厂基类
class SplitterFactory{
public:
    virtual ISplitter* CreateSplitter()=0;
    virtual ~SplitterFactory(){}
};
```

这里一个抽象类一个工厂类，但是在原型模式中我们却将它们合二为一：

```cpp
//抽象类
class ISplitter{
public:
    virtual void split()=0;
    virtual ISplitter* clone()=0; //通过克隆自己来创建对象
    
    virtual ~ISplitter(){}

};
```

对于具体类则为：

```cpp
//具体类
class BinarySplitter : public ISplitter{
public:
    virtual ISplitter* clone(){
        return new BinarySplitter(*this); // 通过拷贝构造函数克隆自己，前提是拷贝构造函数必须写正确；这里是深克隆而不是浅克隆
    }
};

class TxtSplitter: public ISplitter{
public:
    virtual ISplitter* clone(){
        return new TxtSplitter(*this);
    }
};

class PictureSplitter: public ISplitter{
public:
    virtual ISplitter* clone(){
        return new PictureSplitter(*this);
    }
};

class VideoSplitter: public ISplitter{
public:
    virtual ISplitter* clone(){
        return new VideoSplitter(*this);
    }
};
```

而真正用的时候则这样：

```cpp
class MainForm : public Form
{
    ISplitter*  prototype;//原型对象

public:
    
    MainForm(ISplitter*  prototype){
        this->prototype=prototype;
    }
    
	void Button1_Click(){

		ISplitter * splitter=
            prototype->clone(); //克隆原型
        
        splitter->split();

	}
};
```



那么问题又来了，干嘛要这样搞呢？



先看老师的PPT：

![image-20211020173312142](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020173312142.png)



继续回到书本定义：

> 使用原型实例指定创建对象的种类，然后通过拷贝这些原型来创建新的对象。
>
> ——《设计模式》GoF



类图如下：

![image-20211020173222538](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020173222538.png)



因此我们可以看出，使用该模式一般是说，对于这样一些”结构复杂的对象“，并且它们已经到达了某一个状态，因此我们就想通过这样一个克隆的方式，去同时把它们的状态给拿出来。



这便是原型的一个灵活性：传入的prototype是什么，那么克隆出来的状态就是啥样的，避免用Factory Method生成出来的东西实现起来（恢复到该状态）过于复杂。



那么什么时候用工厂方法模式什么时候用原型模式呢？区分点就在于：用工厂方法模式创建对象是不是非常简单的几步就能创建出来，还是说要考虑某个对象很复杂的中间状态而又希望保留那个中间状态。如果是后者就用原型。



这里我想了个例子，比如游戏中某个怪物，在对战到一半中它要进行一个招式叫”克隆“，但是在对战中间，该怪物受到了比如一些debuff、身上插满了刀子、手上拿着武器等等，那么我们就不想用工厂模式去先创建基类再去一步步达到这个状态，而是想直接”克隆“出它现在的样子，因此就可以去用原型模式。



### 总结

![image-20211020173855771](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020173855771.png)

> 对于C++可能序列化不太方便，那么直接就拷贝构造函数就很好了，写对拷贝构造函数。



# 构建器（Builder）

动机：

在软件系统中，有时候面临着“一个复杂对象”的创建工作，其通常由各个部分的子对象用一定的算法构成；由于需求的变化，这个复杂对象的各个部分经常面临着剧烈的**变化**，但是将它们组合在 一起的算法却**相对稳定**。

如何应对这种变化？如何提供一种“封装机制”来隔离出“复杂 对象的各个部分”的变化，从而保持系统中的“稳定构建算法”不 随着需求改变而改变？



继续，举个例子：

比如在游戏中要建房子，无论是啥样的房子，总有固定的几个流程：地基、窗户、墙壁、门......但是一些区别就是不同的房子可能天花板长得不一样、门长的不一样之类的。如代码所示：



```
class House{
public:
	void Init(){
		this->BuildPart1();
		
		for (int i = 0; i < 4; i++){
			this->BuildPart2();
		}
		
		bool flag=pHouseBuilder->BuildPart3();
        
        if(flag){
            pHouseBuilder->BuildPart4();
        }
        
        pHouseBuilder->BuildPart5();
	}
	
	virtual ~House(){}
protected:
	virtual void BuildPart1()=0;
    virtual void BuildPart2()=0;
    virtual void BuildPart3()=0;
    virtual void BuildPart4()=0;
    virtual void BuildPart5()=0;
	
};
```

首先那些Init中用到的方法如BuildPart1、BuildPart2等，不能直接放在House类的构造函数中去写，因为那将导致静态绑定：this->BuildPart1()，但BuildPart1还没实现呢，还是virtual void BuildPart1()=0;呢，如此就会报错。（子类的构造函数会先调用父类的构造函数）但是像Java等在构造函数中调用都是动态绑定的。

接着比方说有一个石头房子，去继承并override就行：

```cpp
class StoneHouse: public House{
protected:
    
    virtual void BuildPart1(){
        //pHouse->Part1 = ...;
    }
    virtual void BuildPart2(){
        
    }
    virtual void BuildPart3(){
        
    }
    virtual void BuildPart4(){
        
    }
    virtual void BuildPart5(){
        
    }
    
};
```

其实Builder模式做到这一步已经ok了，石头房、茅草房等都去override那些函数，但是Init是固定的。



但是还有优化的空间，因为构建过程太复杂，可以提取出来作为一个单独的类。



那么我们就可以做一些拆分：

（第一种方法：HouseBuilder专管构建，和House区分开来；

我们这里再进一步，分出HouseDirector，把那些稳定的部分都拿出来。这样HouseDirector就永远是稳定的，想重写的时候重写HouseBuilder）

```cpp
class House{
    //....
};

class HouseBuilder {
public:
    House* GetResult(){
        return pHouse;
    }
    virtual ~HouseBuilder(){}
protected:
    
    House* pHouse;
	virtual void BuildPart1()=0;
    virtual void BuildPart2()=0;
    virtual void BuildPart3()=0;
    virtual void BuildPart4()=0;
    virtual void BuildPart5()=0;
	
};

class StoneHouse: public House{
    
};

class StoneHouseBuilder: public HouseBuilder{
protected:
    
    virtual void BuildPart1(){
        //pHouse->Part1 = ...; //可以访问pHouse
    }
    virtual void BuildPart2(){
        
    }
    virtual void BuildPart3(){
        
    }
    virtual void BuildPart4(){
        
    }
    virtual void BuildPart5(){
        
    }
    
};


class HouseDirector{
    
public:
    HouseBuilder* pHouseBuilder;
    
    HouseDirector(HouseBuilder* pHouseBuilder){
        this->pHouseBuilder=pHouseBuilder;
    }
    
    House* Construct(){
        
        pHouseBuilder->BuildPart1();
        
        for (int i = 0; i < 4; i++){
            pHouseBuilder->BuildPart2();
        }
        
        bool flag=pHouseBuilder->BuildPart3();
        
        if(flag){
            pHouseBuilder->BuildPart4();
        }
        
        pHouseBuilder->BuildPart5();
        
        return pHouseBuilder->GetResult();
    }
};
```

绕来绕去，万变不离其宗的是理解谁是稳定的谁是变化的。

> martin flower曾说：不能有太肥的类。一个类代码行为太多，不太好，需要重构。



回到书本上的模式定义：

> 将一个复杂对象的构建与其表示相分离，使得同样的构建过 程(稳定)可以创建不同的表示(变化)。
>
> ——《设计模式》GoF



回到UML类图：

![image-20211020180819767](C:\Users\51906\AppData\Roaming\Typora\typora-user-images\image-20211020180819767.png)



我们也强调了这只是一种形式。



### 总结

- Builder 模式主要用于“分步骤构建一个复杂的对象”。在这其中 “分步骤”是一个稳定的算法，而复杂对象的各个部分则经常变化。 
- 变化点在哪里，封装哪里—— Builder模式主要在于应对“复杂对 象各个部分”的频繁需求变动。其缺点在于难以应对“分步骤构建 算法”的需求变动。 
- 在Builder模式中，要注意不同语言中构造器内调用虚函数的差别 （C++ vs. C#) 。



> 总能从变化中找到稳定的，那么就可以使用设计模式了。





