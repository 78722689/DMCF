

#ifndef _MSG_DISPATCHER_H_
#define _MSG_DISPATCHER_H_

#include "DispatcherDefinition.h"
#include "IMsgReceiver.h"
#include <map>

namespace Dispatcher
{

 // Dispathcer interface
class IDispatcher
{
public:
    virtual ~IDispatcher()
    {}

    virtual void start() = 0;
    virtual void registerService() = 0;
    virtual  void unregisterService() = 0;

    virtual void subscribeMessages(IMsgReceiver* receiver) = 0;
};

// Dispathcer can work in a special thread if implemented it.
struct DefaultDispatcherThread
{
    template<typename T, typename MESSAGE>
    void start(T* server, void (T::*DISPATCH)(const MESSAGE&));

    // If you specify a thread to control dispatcher, here should be difined with static member as the thread entry.
    template<typename T, typename MESSAGE>
    void threadEntry(T* server, void (T::*DISPATCH)(const MESSAGE&));
};

// Response for dispatching message when received from notification unit.
// If you want the dispatcher to run on your thread, you should define the thread by yourself.
template<class THREAD>
class MsgDispatcher : public IDispatcher
{
    typedef std::multimap<int, IMsgReceiver*>  RECEIVERS_MAP;
    typedef RECEIVERS_MAP::const_iterator RECEIVERS_ELEMENT_ITERATOR;
    typedef std::pair<RECEIVERS_ELEMENT_ITERATOR, RECEIVERS_ELEMENT_ITERATOR> RECEIVERS_CONST_ITERATOR;

public:
    MsgDispatcher()
    {}
    ~MsgDispatcher()
    {}

    inline virtual void start()
    {
        THREAD::start(this, &dispatch);
    }

    // Register your service to dispatcher and notify to other process.
    inline virtual void registerService()
    {}

    // unregister serrvice.
    inline virtual void unregisterService()
    {}

    // Subscribe message from dispatcher.
    inline virtual  void subscribeMessages(int descriptor, IMsgReceiver* receiver)
    {
        receiver_map_.insert(std::make_pair(descriptor, receiver));
    }

    // dispatch message to subscriber if received it.
    inline void dispatch(const IMessage& msg)
    {
        notifyMessageToSubscribers(msg);
    }

 private:   
    template<typename MESSAGE>
    void notifyMessageToSubscribers(const MESSAGE& msg);

private:
    RECEIVERS_MAP receiver_map_; // To storage all the subscribers and the messages.
    
};

}

#endif //  _MSG_DISPATCHER_H_
