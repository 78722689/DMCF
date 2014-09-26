// To receive the messages what comes from others process or unit.

#ifndef _MSG_RECEIVER_
#define _MSG_RECEIVER_

#include "DispatcherDefinition.h"

#include <memory>
#include <list>

class IMsgReceiver
{
public:
    virtual ~IMsgReceiver()
    {}

    virtual void handle() = 0;
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

typedef void (SERVER::*OPERATION)(MSG_BASE*);

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

private:
    SERVER server_;
    OPERATION operation_;
};


// All class inherits from this if it need to provide the service of receiving message
template<class SERVER>
class MsgReceiver : public IMsgReceiver
{
    typedef std::list<std::shared_ptr<MsgReceiverHandlerBase> > RECEIVER_HANDLER;

public:
    MsgReceiver()
    {}
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
    
private:
    RECEIVER_HANDLER msg_list_;
};

#endif // _MSG_RECEIVER_