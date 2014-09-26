// To receive the messages what comes from others process or unit.

#ifndef _I_MSG_RECEIVER_
#define _I_MSG_RECEIVER_

namespace Dispatcher
{

class IMsgReceiver
{
public:
    virtual ~IMsgReceiver()
    {}

    virtual void handle(IMessage* msg) = 0;
};

}
#endif // _I_MSG_RECEIVER_
