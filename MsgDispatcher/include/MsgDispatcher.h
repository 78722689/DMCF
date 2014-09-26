

#ifndef _MSG_DISPATCHER_H_
#define _MSG_DISPATCHER_H_

#include <map>

namespace Dispatcher
{

 // Dispathcer interface
class IDispatcher
{
public:
    virtual ~IDispatcher()
    {}
    
};

// Response for dispatching message when received from notification unit.
class MsgDispatcher : public IDispatcher
{
public:
    MsgDispatcher()
    {}
    ~MsgDispatcher()
    {}

    void startDispatcher()
    {}

    void registerService()
    {}

    void unregisterService()
    {}

private:
    void notifyMessageToSubscriber()
    {}
};

}

#endif //  _MSG_DISPATCHER_H_