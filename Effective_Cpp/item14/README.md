# 在资源管理中心小心coping行为(Think carefully about copying behavior in resource-managing classes)
这里提到了一个互斥量的设计问题，其思想和LevelDB的代码设计不谋而合：
```cpp
// leveldb/util/mutexlock.h
class SCOPED_LOCKABLE MutexLock {
 public:
  explicit MutexLock(port::Mutex* mu) EXCLUSIVE_LOCK_FUNCTION(mu) : mu_(mu) {
    this->mu_->Lock();
  }
  ~MutexLock() UNLOCK_FUNCTION() { this->mu_->Unlock(); }

  MutexLock(const MutexLock&) = delete;
  MutexLock& operator=(const MutexLock&) = delete;

 private:
  port::Mutex* const mu_;
};
```
互斥量的`Unlock`操作被隐藏到析构函数中，这种设计非常值得学习。