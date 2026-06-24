# Introduciton to Qt Widgets 
[YouTube video Link](https://www.youtube.com/playlist?list=PL6CJYn40gN6iFcTyItvnE5nOmJR8qk_7o)


## Moudle 1 
![](vx_images/527827381053015.png =500x)

### part 01

- Qt Widgets - > desktop application

- Qt QML -> embedded device, mobile device

>basicly based on what‘s your application looks like. 



![](vx_images/436033212646177.png =700x)


![](vx_images/130311015067127.png =700x)


![](vx_images/332132317168082.png =700x) 

### part 02

[qmake-tutorial.html](https://doc.qt.io/qt-6/qmake-tutorial.html)

[cmake-get-started.html](https://doc.qt.io/qt-6/cmake-get-started.html)

[KDToolBox](https://github.com/KDABLabs/KDToolBox)

### part 03
Qt helper

shortcuts:
- F1 -> help documentation
- F2 -> go to defination

### part 04

QObject
![](vx_images/402162282638676.png =800x)


![](vx_images/383141140564.png =800x)

- 并非所有 Qt 类是派生自 QObject
- 并非所有 Qt 对象需要使用对象树管理生存周期和内存
- 并非所有 QObject 类的派生类都是可视类型（for example : QFile)
![](vx_images/251895801375510.png =800x)
![](vx_images/414383829082920.png =300x)

### part 05

 **QWidget**
 
![](vx_images/218283962285147.png =800x)

![](vx_images/597237708375561.png =800x)

Layout 布局管理器，不参控件生命周期管理；
`widget->setLayout(layout); ` 布局管理器中的所有控件会被加入 widget 的子对象树中。

### part 06

**signals and slots**

![](vx_images/484975070088047.png =500x)

三种信号槽连接方式
- 槽函数指针
![](vx_images/493774030764903.png =800x)

对于存在函数重载情况
[signalsandslots.html#connecting-to-overloaded-signals-and-slots](https://doc.qt.io/qt-6/signalsandslots.html#connecting-to-overloaded-signals-and-slots)

- SIGNAL() / SLOT()  宏 (Qt4)
- 函数对象（函数包装器，lambda 表达式）



### part 07

**SIGNAL() / SLOT()  宏 (Qt4)**

![](vx_images/181969480691024.png =800x)

- 能够显示指定重载函数的参数类型
- 其底层是基于字符串形式的函数签名，由 moc 在编译前生成，其功能绑定发生在运行时（根据函数签名字符串遍历）；
- 没有类型检查

### part 08

**信号连接 Lambda 表达式 / 函数包装器 /  独立函数指针 `【无接收者】`**

![](vx_images/223031665717160.png =800x)

lambda 是一个匿名函数，不需要信号的接收者。但需要保证 lambda 表达式捕获的上下文环境的有效性。

信号槽连接的自动生命周期管理，sender 或 receiver 销毁后，连接会自动被释放。lambda 表达式没有接收者，存在上下文失效的风险.（[signalsandslots.html#automatic-connection-management](https://doc.qt.io/qt-6/signalsandslots.html#automatic-connection-management)）。

[code example](https://github.com/KDABLabs/kdabtv/blob/master/Programming-With-Qt-Widgets/objects/ex-connect-function-pointers/main.cpp#L24)

**信号槽连接规则**
- 参数关系
![](vx_images/580564807594421.png =800x)

- 信号参数个数 ≥ 槽函数参数个数
- 参数从` 右  ->  左 `匹配

- 连接关系
![](vx_images/280526872273221.png =800x)


**信号槽 的建立与销毁**
![](vx_images/477144395394357.png =800x)

![](vx_images/208215260618629.png =800x)

对于无接收者的情况（使用lambda 、函数包装器、独立函数指针）


### part 09

**自定义信号、槽函数**

- 自定义槽函数
    ![](vx_images/590316934114091.png =500x)
    - 槽函数就是成员函数，可以设置权限，并且可以被外部或其他成员函数调用
    - 任意成员函数也可以作为槽函数(无须slot)，连接信号槽（Qt5）; `不能兼容 Designer, 其只识别 slot`

- 自定义信号
![](vx_images/42768645783409.png =800x)

- 信号默认是，也只能是public，任何情况都不应给信号添加权限；
- 信号槽连接的参数传递是`强制值传递`

- 信号槽连接传递的参数类型
    - 单线程的直接连接 / 多线程的直接连接 ： 可直接使用内置类型和自定义数据类型
    - 单线程的队列连接 / 多线程队列连接： 必须使用 `Q_DECLARE_METATYPE + qRegisterMetaType`注册自定义类型（需要使用事件循环，事件派发任务队列需要序列化和反序列化）

> 直接连接，槽函数都是在是在发送者所在线程执行的；多线程直接连接，存在竞态条件的风险。


### part 10


`emit signal`  : ` emit` 宏实际上没有任何意义，只是方便阅读

### part 11

**QMetaObject**
存储 QObject 以及 Q_OBJECT 提供功能的一些信息。

[GammaRay wiki](https://github.com/KDAB/GammaRay/wiki) 运行时手动修改 Qt 属性

### part 12

**Event System**

[The Event System](https://doc.qt.io/qt-6/eventsandfilters.html)

![](vx_images/490155689708854.png =700x)
  
- widget 绘图
![](vx_images/373893360188021.png =500x)


### part 13 

**Qt Designer**

[Qt Widgets Designer Manual](https://doc.qt.io/qt-6/qtdesigner-manual.html)
1. 创建带UI文件的项目
![](vx_images/101866860351674.png =500x)

2. 创建带UI文件的窗口类
![](vx_images/107927355504806.png =500x)

类内组合一个用于窗口绘制的代理类 Ui::ClassName 对象
![](vx_images/381628170774793.png =500x)

经过 UIC 把 UI 文件转译成 C++ 类文件，其内部代理了界面绘制动作

![](vx_images/156047597819142.png =500x)

窗口控件本体在构造函数内将绘制动作委托给 UI 类的 setupUi 方法
![](vx_images/143058645008789.png =500x)