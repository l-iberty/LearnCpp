# item05: 了解C\++默默编写并调用了哪些函数(Know what functions C\++ silently writes and calls)

## 1. 关于拷贝构造函数和赋值运算符
### (1) 如何区分?
- 有新对象生成->调用拷贝构造函数
- 已有对象之间的赋值->调用赋值运算符

```cpp
CTest t1;
CTest t2 = t1; // 有新对象生成->调用拷贝构造函数. 相当于CTest t2(t1)
t1 = t2; // 已有对象之间的赋值->调用赋值运算符. 相当于t1.operator=(t2)
```

### (2) 为什么拷贝构造函数必须接收引用类型作为参数?
如果有如下类型的拷贝构造函数:
```cpp
CTest(const CTest t);
```
那么在执行```CTest t2 = t1```时, 就会在传参的过程中无限循环地调用拷贝构造函数: 由于这个拷贝构造函数的参数按值传递, 所以将实参`t1`传递给形参`t`的过程中就需要再次调用拷贝构造函数, 然后就会一直调下去...

代码实例:

```cpp
class CTest {
public:
    CTest(const char *pBuffer, size_t n) {
        m_pBuffer = new char[n];
        memcpy(m_pBuffer, pBuffer, n);
        m_BufferLength = n;
    }

    CTest(const CTest &t) {
        m_pBuffer = new char[t.m_BufferLength];
        memcpy(m_pBuffer, t.m_pBuffer, t.m_BufferLength);
        m_BufferLength = t.m_BufferLength;
    }

    ~CTest() { delete m_pBuffer; }

    CTest &operator=(const CTest &t) {
        if (this == &t) return *this;
        delete m_pBuffer;
        m_pBuffer = new char[t.m_BufferLength];
        memcpy(m_pBuffer, t.m_pBuffer, t.m_BufferLength);
        m_BufferLength = t.m_BufferLength;
        return *this;
    }
    
private:
    char *m_pBuffer;
    size_t m_BufferLength;
};
```

## 2. 对于“内含reference成员”或者“内含const成员”的classes...
C++编译器拒绝生成默认的赋值运算符函数, 你必须自己定义.

该性质对于const成员是显然的, 因为const成员是read-only, 已经初始化就不能再修改.

对于reference成员：对引用变量的修改实则是对它所指向的对象进行修改. 考虑以下代码:

```cpp
class NamedObject {
public:
    NamedObject(std::string &name) : m_NameValue(name) {}

    NamedObject &operator=(const NamedObject &o) {
        if (this == &o) return *this;
        m_NameValue = o.m_NameValue;
        return *this;
    }

private:
    std::string &m_NameValue;
};

int main() {
    std::string newDog("Persephone");
    std::string oldDog("Satch");
    NamedObject p(newDog);
    NamedObject s(oldDog);
    
    p = s;

    std::cout << "newDog: " << newDog << std::endl;
    std::cout << "oldDog: " << oldDog << std::endl;
}
```

`p.m_NameValue`和`s.m_NameValue`分别是`newDog`和`oldDog`两个`string`对象的引用. 执行`p = s`等价于`p.operator=(s)`, 其结果是`p.m_NameValue`指向的`newDog`被修改, 于是得到如下输出:

```
newDog: Satch
oldDog: Satch
```

其中的关键之处在于, 对引用变量的修改实则就是在修改它所指向的对象, 编译器认为擅自添加这样的代码（正如上面自定义的`operator=`所做的那样）是不安全的, 所以拒绝生成默认的赋值运算符函数.