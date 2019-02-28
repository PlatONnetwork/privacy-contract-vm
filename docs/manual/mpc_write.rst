MPC合约编写
===============

MPC合约
----------

::

      MPC合约是一种类似C++语言，使用C++的语法，运行在PlatON MPC VM上公链系统上，提供安全的多方计算服务。
      MPC合约提供MPC运算基础类型，这些数据类型运算在PlatON MPC VM上， 使用MPC安全计算协议传输数据、计算结果，参与方计算方在不泄露实际数据前提下，实现安全计算。
	  当前版本为两方计算MPC合约编写规范，三方及多方MPC计算版本不久将推出。

依赖组件
-----------

1. **platon MPC计算组件库**

::

   提供MPC计算相关头文件等，已经内置到编译器中

2. **Google Protobuf库**

::

    提供pb协议打包和解包实现库和头文件， 已经内置到编译器中

3. **plang编译器**

::

    提供MPC合约编译输出wasm智能合约、JAVA数据提供商代理代码等。

MPC基础类型
--------------

::
   
    MPC合约当前仅支持32位、64位整型**MPC**运算类型（**platon::mpc::Integer**），MPC运算类型使用MPC计算协议进行多方安全计算。
    计算双方的角色要分别为Alice、Bob，支持的算术运算包括：**“+, -, *, /, |, &, ^”** 等。


下面以32位整型的MPC加法运算为例：

1. 声明32位有符号整型``MPC变量``

.. code:: cpp
    
    platon::mpc::Integer v1(32, 123, ALICE);//声明Alice MPC变量
    platon::mpc::Integer v2(32, 456, BOB);//声明Bob MPC变量

2. MPC加法运算

.. code:: cpp

    platon::mpc::Integer result = v1 + v2; //进行MPC加法运算

3. 获取结果值

.. code:: cpp

    int plain_result = result.reveal_int32<int>();//Alice和Bob都获得计算结果579

.. Note::


    MPC运算类型变量运算要求Alice和Bob在同类型、同长度的条件下进行。

platon::mpc::Integer
~~~~~~~~~~~~~~~~~~~~~~~~

::

    **platon::mpc::Integer**作为MPC计算的实现对象，提供算术运算、绝对值、位运算等MPC安全计算。

**接口**：

.. code:: cpp

    namespace platon
    {
        namespace mpc
        {
            //构造
            Integer(int32_t length, const std::string& value, uint8_t party = PUBLIC);
            Integer(int32_t length, int32_t value, uint8_t party = PUBLIC);
            
            //运算
            Integer operator<<(int shamt)const;
            Integer operator>>(int shamt)const;
            Integer operator<<(const Integer& shamt)const;
            Integer operator>>(const Integer& shamt)const;

            Integer operator+(const Integer& rhs)const;
            Integer operator-(const Integer& rhs)const;
            Integer operator-()const;
            Integer operator*(const Integer& rhs)const;
            Integer operator/(const Integer& rhs)const;
            Integer operator%(const Integer& rhs)const;
            Integer operator&(const Integer& rhs)const;
            Integer operator|(const Integer& rhs)const;

            Bit& operator[](int index);
            const Bit & operator[](int index) const;
            
            Integer abs() const;
        }
    }


MPC函数定义
--------------

MPC函数定义为：

::
    
    能够参与MPC计算的函数，输入参数为2个以上，参数1对应为Alice，参数2对应为Bob，其他参数为附加参数。

函数定义原型如下：

.. code:: cpp

    return_type function_name(declare_type in1, declare_type in2, ...) 

含义：

::

    **return_type**: 返回类型
    **declare_type**: C++基本类型或Google Protobuf定义的类型
    **in1**: Alice的输入
    **in2**: Bob的输入


下面定义一个MPC函数，并使用用户自定义类型作为参数。

1. 定义用户自定类型Foo（使用Google Protobuf定义）

.. code:: protobuf

    message Foo {
        int32 item1;
        int32 item2;
        string info;
    };

2. 定义函数

::

    这里定义函数实现进行了取绝对值运算，Alice和Bob的输入值在MPC运算过程都不会泄露，最终获得计算更新后的Foo对象。

代码如下：

.. code:: cpp

    /** 
    * 对Alice输入的Foo对象item1，item2字段分别和Bob输入值进行绝对值运算，计算结果返回计算后的Foo对象
    */
    Foo foo_abs(const Foo& in1, int32_t in2, const std::string& extra)
    {
        Foo foo = in1;
        platon::mpc::Integer item1(32, in1.item1(), ALICE);//Alice输入item1
        platon::mpc::Integer item2(32, in1.item2(), ALICE);//Alic输入item2
        platon::mpc::Integer v2(32, in2.item1(), BOB);//Bob输入
        platon::mpc::Integer ret1 = (item1 - v2).abs();//MPC绝对值运算
        platon::mpc::Integer ret2 = (item2 - v2).abs();//MPC绝对值运算
        foo.set_item1(ret1.reveal_int32());//更新item1
        foo.set_item2(ret2.reveal_int32());//更新item2
        
        return foo;
    }


3. 调用MPC运算实例
例如，Alice和Bob分别输入：{1000, 100}， 900进行foo_add MPC计算

Alice调用MPC运算代码如下：

.. code:: cpp

    /** 
    外部构建Alice对象
    alice.set_item1(1000);
    alice.set_item2(100);
    */
    Foo alice;
    int in2 = 0;//Bob输入对本地无意义
    Foo result = foo_add(alice, in2);//参数1的值只有Alice知道，参数2对于Alice无意义，依赖MPC对端的Bob
    ```

Bob调用MPC运算如下：

.. code:: cpp

    //Alice输入参数对本地Bob无意义
    Foo alice;
    int in2 = 900;//外部构建Bob输入
    Foo result = foo_add(alice, in2);//参数1的值只有Bob知道，参数1对于Bob无意义，依赖MPC对端的Alice， 

可以发现Alice和Bob调用同样的代码：``Foo result = foo_add(alice, in2)``， 仅仅输入不一样，并且都只使用自己的输入参与两方计算，另外一个无关输入可以任意（这里默认为0）。


完整实例
------------

**protobuf定义**：

.. code:: protobuf

    message Foo {
        int32 item1;
        int32 item2;
        string info;
    };


**MPC合约源码**:

.. code:: cpp

    #include "foo.pb.h"
    #include "platon_integer.h"

    /** 
    * 对Alice输入的Foo对象item1，item2字段分别和Bob输入值进行绝对值运算，计算结果返回计算后的Foo对象
    */
    Foo foo_abs(const Foo& in1, int32_t in2, const std::string& extra)
    {
        Foo foo = in1;
        platon::mpc::Integer item1(32, in1.item1(), ALICE);//Alice输入item1
        platon::mpc::Integer item2(32, in1.item2(), ALICE);//Alic输入item2
        platon::mpc::Integer v2(32, in2.item1(), BOB);//Bob输入
        platon::mpc::Integer ret1 = (item1 - v2).abs();//MPC绝对值运算
        platon::mpc::Integer ret2 = (item2 - v2).abs();//MPC绝对值运算
        foo.set_item1(ret1.reveal_int32());//更新item1
        foo.set_item2(ret2.reveal_int32());//更新item2
        
        return foo;
    }
