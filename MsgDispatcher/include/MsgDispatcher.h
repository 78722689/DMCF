

#ifndef _MSG_DISPATCHER_H_
#define _MSG_DISPATCHER_H_

#include "DispatcherDefinition.h"
#include "IMsgReceiver.h"
#include <list>

namespace Dispatcher
{

 // Dispathcer interface
class IDispatcher
{
public:
    virtual ~IDispatcher()
    {}

    virtual void subscribeMessages(IMsgReceiver* receiver) = 0;
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

    void subscribeMessages(IMsgReceiver* receiver)
    {
        receiver_list_.push_back(receiver);
    }

private:
    void notifyMessageToSubscriber()
    {}


private:
    std::list<IMsgReceiver*> receiver_list_;
    
};

}

#endif //  _MSG_DISPATCHER_H_