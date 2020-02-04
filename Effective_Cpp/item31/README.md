# item31 将文件间的编译依存关系降至最低(Minimize compilation dependencies between files)
以leveldb代码为例：
```cpp
// leveldb/db/db_impl.h
...
class MemTable;
class TableCache;
class Version;
class VersionEdit;
class VersionSet;
...
```
在该文件后面不论是定义上述类型的类变量，或是某个函数声明需要用到其中的某个类，都可以保证编译通过，如果不使用这样的class前置声明，就需要include相应的头文件，从而加大了“文件之间的编译依存关系”。