# item08 别让异常逃离析构函数(Prevent exceptions from leaving destructors)
考虑一下代码：
```cpp
class Widget {
public:
	...
	~Widget() {...} // 假设这个析构函数可能抛出一个异常
}
void doSomething() {
	std::vector<Widget> v;
	...                     // v在这里被销毁
}
```
当`vector v`被销毁，内含的所有`Widget`也会被销毁。如果在析构第一个对象期间有异常被抛出，那么剩余的对象将不会再被析构，从而发生资源泄漏. 示例代码：
```cpp
class Widget {
public:
	Widget() = default;
	~Widget() {
		std::cout << "Destructor invoked.\n";
		throw 0;
	}
};
void doSomething() {
	std::vector<Widget> v(10);
}

int main() {
	try {
		doSomething();
	}
	catch (...) {
		// Handle exception here.
	}
}
```
运行结果中只包含一条输出`Destructor invoked.\n`，即只有第一个`Widget`对象被析构。