# Qt Core

## [元对象系统](https://doc.qt.io/qt-6/zh/metaobjects.html)

实现以下功能

- 对象间通信
- 运行时类型
- 动态属性系统
- 信号和槽

> 对象树不是元对象系统的功能，是由 QObject 类实现的功能

实现原理：

1. 继承 QObject
2. 使用 Q_OBJECT 宏添加额外功能代码
3. MOC 在编译前添加额外代码

> 实现元对象的类，都会拥有一个静态 QMetaObject 对象（类共有），作为使用元对象功能的接口

[QMetaObject](https://doc.qt.io/qt-6/zh/qmetaobject.html)

## [属性系统](https://doc.qt.io/qt-6/zh/properties.html)

基于 `元对象系统`，实现对象属性的动态管理（运行时读取和变更）, 其关联的原生C++成员变量可以通过元对象系统的属性接口访问（可用于QML C++ 集成，导出 C++ 类型）。

[概述 - QML 与 C++ 的集成](https://doc.qt.io/qt-6/zh/qtqml-cppintegration-overview.html)

注意：

- C++ 原生成员变量可以通过用户自定义的 MEMBER 与 READ / WRITE 接口直接访问，属性系统只提供 **`访问接口`**
- 动态添加的属性，并不属于 C++ 原生成员，此处属性系统 **`提供存储`** 和 **`访问接口`**。（其数据存储在属性系统中，用户的使用需要通过 QVariant 类型进行中转）

## [对象树](https://doc.qt.io/qt-6/objecttrees.html)

继承自 QObject 的类型都具有对象树功能，用于管理子对象的内存和生存周期。

- 实例化时指定父对象
- 父对象销毁时，其对象树上的所有子对象都会被销毁

**QML中的父对象**

- 父对象：管理子对象的生存周期

- 可视父对象（parent 属性）：管理对象是否渲染
  
  - 没有显式指定 parent , 默认设置上下文对象为可视父对象（父对象）
  
  - 指定 parent : null ， 该对象不会被渲染显示
  
  - parent 动态更改，该对象的显示位置会发生改变
    
    > 其生存周期始终由父对象管理，可视父对象仅控制 是否显示 和 显示位置

## [信号和槽](https://doc.qt.io/qt-6/zh/signalsandslots.html)

`signal 默认是且只能是 public 的，不需要显式声明为 public`

`slots 可以设置权限，来约束其可以在哪里被连接`

只要不涉及线程间通信和队列信号，信号和槽的参数传递可以直接使用C++原生数据类型

涉及线程间通信，必须使用Qt类型

- Qt 默认类型 （QMetaType）
- [自定义 Qt 类型](https://doc.qt.io/qt-6/zh/custom-types.html)    
  1. Q_DECLARE_METATYPE() 编译时声明，提供元对象支持 : 可被 Qt 自带的功能函数识别， 但不可用于线程间通信
  2. qRegisterMetaType() 模板函数运行时注册，Qt 能自动完成其创建和销毁，可用于线程间的参数传递。

**信号槽连接出现功能缺失**

1. sender / reveiver 指针失效，Qt 会将该 connection 销毁
2. 使用 lambda 表达式引用的已失效参数，会导致崩溃（使用智能指针弥补, 或提供 this 上下文接收者保证数据失效断开连接）
   [自动信号槽连接管理](https://doc.qt.io/qt-6/zh/signalsandslots.html#automatic-connection-management)

**信号槽连接的参数对应**

![](vx_assets/386b10b2-e6d1-40ad-94db-6284be3edc79/580144310845366.png =500x)

- 参数个数不需要相同
- 信号参数个数 ≥ 槽函数参数个数
- 参数类型左对齐

**信号槽连接的数量对应关系**

- 一信号  ->  多槽
- 多信号  ->  一槽
- 信号   ->    信号
  ![](vx_assets/386b10b2-e6d1-40ad-94db-6284be3edc79/281304897239446.png%20=500x)

### [同步信号槽 Qt::DirectConnection](https://doc.qt.io/qt-6/zh/qt.html#ConnectionType-enum)

1. 线程内的信号槽 ： 是直接的函数调用 （可传递C++原生数据类型）
2. 线程间的信号槽：  槽函数会被信号 sender 立即执行，会产生 **竞态条件**，要注意线程间的同步。  （可传递C++原生数据类型）

### [异步信号槽 Qt::QueuedConnection](https://doc.qt.io/qt-6/zh/qt.html#ConnectionType-enum)

依赖于 [QEvent ](https://doc.qt.io/qt-6/zh/qevent.html)  和 [ 事件循环 ](https://doc.qt.io/qt-6/zh/eventsandfilters.html)，即事件循环必须启动后才会派发槽函数的任务。使用队列信号槽连接。

信号传递底层需要使用队列，要求参数必须可序列化, **必须使用 Qt 类型**:
`序列化 -> 入队列 -> 派发 -> 反序列化 -> 槽函数传入`

1. 线程内的信号槽 ：使用事件循环，当前线程必须启动事件循环
2. 线程间的信号槽 ：使用事件循环，信号接收者所在线程必须启动事件循环

## 事件系统
