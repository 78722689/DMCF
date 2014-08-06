#include "loger/loger_definition.h"
#include "ThreadPool.h"
#include "event/EventHandler.h"

#include "Guard.h"
#include "Locker.h"
#include "dmcfos.h"

Loger loger("main");

/**** example events**************************************/
#define evStart_TEST_id 0x01
#define evResponse_TEST_id 0x02
class evStart: public Event{
public:
    evStart() : src_(NULL)
    {
        id_ = evStart_TEST_id; 
    }

    evStart(IReactive* src) : src_(src)
    {
        id_ = evStart_TEST_id; 
    }
    ~evStart(){}

public:
    IReactive* src_;
};
class evResponse: public Event{
public:
    evResponse()
    {
        id_ = evResponse_TEST_id; 
    }
    ~evResponse(){}
};

// An example for event how it works with Reactive, and communicates between threads.
class Client2 : public ReactiveAdapter< Client2 >
{
public:
    Client2()
    {
        setThread(threadPool->getThread()->holdWithParameters("Client2"));
        loger << debug << "Client1 ThreadID[" << DMCF_OSGetCurrentThread() << "] "<< "setthread";
        registerEventToReactive(&Client2::onStart);
    }
    ~Client2(){}

public:
    void onStart(evStart* ev)
    {
        //Guard<Locker> guard(lock_);
        
        loger << debug << "Client2 ThreadID[" << DMCF_OSGetCurrentThread() << "] " << "received evStart.";
        ev->src_->gen(new evResponse);
    }

private:
	//Locker lock_;
};

class Client1 : public ReactiveAdapter< Client1 >
{
public:
    Client1()
    {
        setThread(threadPool->getThread()->holdWithParameters("Client1"));
        loger << debug << "Client1 ThreadID[" << DMCF_OSGetCurrentThread() << "] "<< "setthread";
        registerEventToReactive(&Client1::onStart);
        registerEventToReactive(&Client1::onResponse);

        client_ = new Client2;
    }
    ~Client1()
    {
        delete client_;
    }

public:
    void onStart(evStart*)
    {
        //Guard<Locker> guard(lock_);
        
        loger << debug << "Client1 ThreadID[" << DMCF_OSGetCurrentThread() << "] " << "received evStart.";

        client_->gen(new evStart(this));
    }

    void onResponse(evResponse*)
    {
        loger << debug << "Client1 ThreadID[" << DMCF_OSGetCurrentThread() << "] " << "received evResponse.";
    }

public:
    Client2* client_;
    //Locker lock_;
};

class Client3 : public ReactiveAdapter< Client3 >
{
public:
    Client3(Client2* client): client_(client)
    {
        setThread(threadPool->getThread()->holdWithParameters("Client3"));
        loger << debug << "Client3 ThreadID[" << DMCF_OSGetCurrentThread() << "] "<< "setthread";
        registerEventToReactive(&Client3::onResponse);
        client_->gen(new evStart(this));
    }
    Client3(){}

public:
   void onResponse(evResponse*)
    {
        loger << debug << "Client3 ThreadID[" << DMCF_OSGetCurrentThread() << "] " << "received evResponse.";
    }

private:
    Client2* client_;
	//Locker lock_;
};

/** Example:
* 1. main start the process with evStart.
* 2. Client1 notify Client2 start its process with evStart(this).
* 3. Client2 response a message to Client1 after completed receiving and handling evStart process.
* Note: Don't send message to a temp client, probably the thread is not handle the message yet and the temp client has been released.
**/
int main()
{
    threadPool->CreateThreads();

    Client1 client1;
   Client3 client3(client1.client_);
    client1.gen(new evStart);
    threadPool->joinThreads();

    return 0;
}