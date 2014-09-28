

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
//    typedef std::multimap<>
public:
    MsgDispatcher()
    {}
    ~MsgDispatcher()
    {}

    void start()
    {

    }

    void registerService()
    {}

    void unregisterService()
    {}

    inline void subscribeMessages(int descriptor, IMsgReceiver* receiver)
    {
        receiver_list_.push_back(receiver);
    }

private:
    template<typename T>
    void notifyMessageToSubscriber(const T& msg)
    {}


private:
    std::list<IMsgReceiver*> receiver_list_;
    
};

}

#endif //  _MSG_DISPATCHER_H_