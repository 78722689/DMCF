#ifndef _GUARD_H_
#define _GUARD_H_

template<class LOCKTYPE>
class Guard
{
public:
    explicit Guard(const LOCKTYPE& lock) : lock_(lock)
    {
        lock.lock();
    }
    ~Guard()
    {
        lock.unlock();
    }
    
private:
    const LOCKTYPE& lock_;
};

#endif // _GUARD_H_