# item04: 确定对象被使用前已先被初始化(Make sure that objects are initialized before they're used)
## 为避免“跨编译单元初始化次序”问题，请以local static对象替换non-local static对象
```cpp
static CTest g_obj; // not recommended

inline CTest &getStaticCTestObj() {
    static CTest obj;
    return obj;
}

int main() {
    getStaticCTestObj().foo();
}
```
