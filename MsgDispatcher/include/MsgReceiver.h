// To receive the messages what comes from others process or units.

#ifndef _MSG_RECEIVER_
#define _MSG_RECEIVER_

#include "DispatcherDefinition.h"
#include "IMsgReceiver.h"
#include "MsgDispatcher.h"

#include <memory>
#include <list>
#include <algorithm>

namespace Dispatcher
{

struct HandlerCaller
{
    HandlerCaller(IMessage* msg) : msg_(msg)
    {}

    template<typename T>
    void operator ()(T& element) const
    {
        if (element.getMsgId() == msg_->getMsgId())
        {
            element.callOperation(msg_);
        }
    }

    IMessage* msg_;
};

struct MsgReceiverHandlerBase
{
public:
    virtual ~MsgReceiverHandlerBase()
    {}

    virtual int getMsgId() const = 0;
};

template<class SERVER, class MESSAGE>
class MsgReceiverHandler : public MsgReceiverHandlerBase
{

typedef void (SERVER::*OPERATION)(MESSAGE*);

public:
    MsgReceiverHandler(SERVER* server, OPERATION operation)
        : server_(server), operation_(operation)
    {}
    ~MsgReceiverHandler()
    {}
    
    inline int getMsgId() const
    {
        MESSAGE msg;    
        return msg.getMsgId();
    }

    inline void callOperation(MESSAGE* msg)
    {
        (server_->*operation_)(msg);
    }
private:
    SERVER *server_;
    OPERATION operation_;
};


// All class inherits from this if it need to provide the service of receiving message
template<class SERVER>
class MsgReceiver : public IMsgReceiver
{
    typedef std::list<std::shared_ptr<MsgReceiverHandlerBase> > RECEIVER_HANDLER;

public:
    MsgReceiver(int descriptor, IDispatcher* dispatcher) : descriptor_(descriptor), dispatcher_(dispatcher)
    {
        subscribeToDispatcher();
    }
    ~MsgReceiver()
    {}

    template<typename MESSAGE>
    void registerMsgToReceiver(void (*HANDLER)(MESSAGE*))
    {
        msg_list_.push_back(makeMsgReceiverHandler(HANDLER));
    }

protected:
    template<typename MESSAGE>
    std::shared_ptr<MsgReceiverHandlerBase> makeMsgReceiverHandler(void (*HANDLER)(MESSAGE*))
    {
        return std::make_shared<MsgReceiverHandler<SERVER, MESSAGE> >(static_cast<SERVER>(this), HANDLER);
    }

    void subscribeToDispatcher()
    {
        if (NULL != dispatcher_)
        {
            dispatcher_->subscribeMessages(descriptor_, this);
        }
    }

    virtual void handle(const IMessage& msg) 
    {
        std::for_each(msg_list_.begin(), msg_list_.end(), HandlerCaller(msg));
    }
private:
    int descriptor_;
    IDispatcher* dispatcher_;
    RECEIVER_HANDLER msg_list_;
   
};

}
#endif // _MSG_RECEIVER_
