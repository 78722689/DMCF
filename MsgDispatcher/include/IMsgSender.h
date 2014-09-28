
#ifndef _I_MSG_SENDER_
#define  _I_MSG_SENDER_

namespace Dispatcher
{
    
class IMsgSender
{
public:
    virtual ~IMsgReceiver()
    {}

    virtual void send(const IMessage& msg) = 0;
};

}

#endif