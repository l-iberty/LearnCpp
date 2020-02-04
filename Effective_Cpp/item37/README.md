# item37 绝不重新定义继承而来的缺省参数值
“静态类型”是指被声明时所采用的类型，“动态类型”是指程序运行时所指对象的类型，例如：
```cpp
Shape *ps;                 // 静态类型为Shape*
Shape *pc = new Circle;    // 静态类型为Shape*, 动态类型为Circle*
Shape *pr = new Rectangle; // 静态类型为Shape*, 动态类型为Rectangle*
```

虚函数是动态绑定的，但是如果有缺省参数值，则缺省参数值是静态绑定的。示例代码：
```cpp
class Shape {
public:
	enum Color{ Red=0, Green, Blue };

	virtual void draw(Color c = Red) {
		std::cout << "draw a shape with color_" << c << "\n";
	}
};

class Circle :public Shape {
public:
	virtual void draw(Color c = Green) override {
		std::cout << "draw a circle with color_" << c << "\n";
	}
};

class Rectangle :public Shape {
public:
	virtual void draw(Color c = Blue) override {
		std::cout << "draw a rectangle with color_" << c << "\n";
	}
};

int main() {
	Shape *ps = new Shape;
	Shape *pc = new Circle;
	Shape *pr = new Rectangle;

	ps->draw(); // 调用Shape::draw(Shape::Red)
	pc->draw(); // 调用Circle::draw(Shape::Red)而不是Circle::draw(Shape::Green)
	pr->draw(); // 调用Rectangle::draw(Shape::Red)而不是Rectangle::draw(Shape::Blue)
}
```