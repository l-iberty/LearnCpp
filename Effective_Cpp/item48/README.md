# item48 认识模板元编程(TMP, Template metaprogramming)
例1：计算阶乘
```cpp
template<int n>
struct Factorial {
	enum { value = n*Factorial<n - 1>::value };
};

template<>
struct Factorial<0> {
	enum { value = 1 };
};

int main(int argc, char const *argv[])
{
	int x = Factorial<5>::value; // 5!在编译期被计算出来
}
```
TMP的阶乘计算是通过“递归模板实例化”实现的，因为是在编译期完成的，所以程序拥有极高的执行效率。

例2：斐波那契数
```cpp
template<int n>
struct Fib {
	enum { value = Fib<n - 1>::value + Fib<n - 2>::value };
};
template<>
struct Fib<0> {
	enum{ value = 0 };
};
template<>
struct Fib<1> {
	enum{ value = 1 };
};

int main() {
	int x = Fib<10>::value;
}
```