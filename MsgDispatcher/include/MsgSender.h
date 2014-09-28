
// To send the messages to others process or units
// NOTE: you can only send the messages after received the instance of MsgSender from dispatcher, 
// that's meant you must waiting for the process that it is startuped normally and registered to dispatcher sucessfully.

#ifndef _MSG_SENDER_
#define _MSG_SENDER_

#include "DispatcherDefinition.h"
#include "IMsgSender.h"

namespace Dispatcher
{

class MsgSender : public IMsgSender
{
    typedef std::list<IMessage> BUFFERED_MSG;
public:
    MsgSender() : descriptor_(-1)
    {}
    MsgSender(int descriptor) : descriptor_(descriptor)
    {}
    ~MsgSender()
    {}

    // Send messages via IPC interface....
    virtual void send(const IMessage& msg)
    {
        // if no valid descriptor, buffered the sending message to list till received the valid descriptor.
        if (-1 == descriptor_)
        {
            buffered_msg_list_.push_back(msg);
        }
        else // TODO: Send it.
        {
               ; // TODO: Send it with IPC interface.
        }
    }

private:
    int descriptor_;    // Dest descriptor, it's only set after the process startuped and registered to dispatcher sucessfully.
    BUFFERED_MSG buffered_msg_list_;
};

}
#endif // _MSG_SENDER_