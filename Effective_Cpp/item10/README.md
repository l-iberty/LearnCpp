# item10 令`operator=`返回一个*reference to *this*
示例代码：
```cpp
class Object {
public:
	Object() :m_pBuffer(nullptr), m_n(0) {}
	Object(const char *s, int n) {
		m_pBuffer = new char[n];
		m_n = n;
		memcpy(m_pBuffer, s, n);
	}
	~Object() {
		if (m_pBuffer != nullptr) {
			delete m_pBuffer;
		}
	}
	Object& operator=(const Object& rhs) {
		if (this == &rhs) return *this;

		this->m_pBuffer = new char[rhs.m_n];
		this->m_n = rhs.m_n;
		memcpy(this->m_pBuffer, rhs.m_pBuffer, rhs.m_n);

		return *this;
	}
	void printBuffer() {
		std::string str(m_pBuffer, m_n);
		printf("addr of buffer: %p\n", m_pBuffer);
		printf("content of buffer: %s\n", str.c_str());
	}
private:
	char *m_pBuffer;
	int m_n;
};

int main() {
	std::string str("hello,c++");
	Object obj1(str.data(), str.size());
	Object obj2;

	obj2 = obj1;
	//obj2.operator=(obj1);

	std::cout << "**** obj1 ****\n";
	obj1.printBuffer();
	std::cout << "**** obj2 ****\n";
	obj2.printBuffer();
}
```

不要把`obj2=obj1`看成赋值操作，这就是一个简单的函数调用`obj2.operator=(obj1)`，所以`this`就是`obj2`，`rhs`就是`obj1`.

另外，`if (this == &rhs) return *this;`是为了处理“把自己赋值给自己”的情况，比如`obj1=obj1`，即`obj1.operator=(obj1)`，此时不需要进行任何的资源拷贝。