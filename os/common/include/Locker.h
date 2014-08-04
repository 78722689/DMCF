#ifndef _LOCKER_H_
#define _LOCKER_H_

class Locker
{
public:
    Locker()
    {
    }
    ~Locker()
    {
    }

public:
    void lock() const{};
    void unlock() const{};
};

#endif // _LOCKER_H_