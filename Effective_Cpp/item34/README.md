# item34 区分接口继承和实现继承
基类可以为纯虚函数提供一个缺省实现，其声明部分表现的是接口(派生类必须实现这个接口)，其定义部分则表现出缺省行为。leveldb有一个抽象基类`DB`和它的实现类`DBImpl`，`DB`声明了几个纯虚函数并给出了缺省实现，`DBImpl`在实现那几个接口时直接调用了`DB`的缺省实现：
```cpp
class LEVELDB_EXPORT DB {
 public:
  ...
  virtual Status Put(const WriteOptions& options, const Slice& key,
                     const Slice& value) = 0;
  ...
};

class DBImpl : public DB {
 public:
  ...
  // Implementations of the DB interface
  Status Put(const WriteOptions&, const Slice& key,
             const Slice& value) override;
  ...
};

Status DBImpl::Put(const WriteOptions& o, const Slice& key, const Slice& val) {
  return DB::Put(o, key, val);
}

// Default implementations of convenience methods that subclasses of DB
// can call if they wish
Status DB::Put(const WriteOptions& opt, const Slice& key, const Slice& value) {
  WriteBatch batch;
  batch.Put(key, value);
  return Write(opt, &batch);
}

```
所以说，我们可以为纯虚函数提供定义，作为其缺省实现。