
#ifndef _MSG_SENDER_
#define _MSG_SENDER_

#include "DispatcherDefinition.h"

class IMsgSender
{
public:
    virtual ~IMsgReceiver()
    {}

    virtual void handle() = 0;
};


class MsgSender //: public IMsgSender
{
public:
    MsgSender()
    {}
    ~MsgSender()
    {}

    template<typename MESSAGE>
    void registerMsgToReceiver(MSG_BASE* msg)
    {
        //msg_queue_.push_back(*msg);
    }

private:
    //std::vector<MSG_BASE> msg_queue_;
};

#endif // _MSG_SENDER_