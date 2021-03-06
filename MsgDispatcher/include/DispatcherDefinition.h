
#ifndef _DISPATCHER_DEFINITION_
#define _DISPATCHER_DEFINITION_

class IMessage
{
public:
    virtual ~IMessage()
    {}

    virtual int getMsgId () const = 0;
    virtual int getDestDescriptor() const = 0;
    virtual int getSourceDescriptor() const = 0;
};

// Message base for all IPC messages
class MSG_BASE : public IMessage
{
public:
    MSG_BASE() : msgid_(-1)
    {}
    virtual ~MSG_BASE()
    {}

    inline virtual int getMsgId() const {return msgid_;}
    inline virtual int getDestDescriptor() const {return dest_;}    
    inline virtual int getSourceDescriptor() const {return source_;}

protected:
    int msgid_;
    int dest_;
    int source_;
};

// An example message
#define MSG_START_ID 0x888888
class MSG_START : public MSG_BASE
{
public:
    MSG_START()
    {
        msgid_ = MSG_START_ID;
    }
    ~MSG_START()
    {}
};

#endif // _DISPATCHER_DEFINITION_

