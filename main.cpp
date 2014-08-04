#include "loger/loger_definition.h"
#include "ThreadPool.h"
#include "event/EventHandler.h"

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
        loger << debug << "Client1 ThreadID[" << CURRENT_THREAD_ID << "] "<< "setthread";
        registerEventToReactive(&Client2::onStart);
    }
    ~Client2(){}

public:
    void onStart(evStart* ev)
    {
        loger << debug << "Client2 ThreadID[" << CURRENT_THREAD_ID << "] " << "received evStart.";
        ev->src_->gen(new evResponse);
    }
};

class Client1 : public ReactiveAdapter< Client1 >
{
public:
    Client1()
    {
        setThread(threadPool->getThread()->holdWithParameters("Client1"));
        loger << debug << "Client1 ThreadID[" << CURRENT_THREAD_ID << "] "<< "setthread";
        registerEventToReactive(&Client1::onStart);
        registerEventToReactive(&Client1::onResponse);

        client_ = new Client2;
    }
    ~Client1(){delete client_;}

public:
    void onStart(evStart*)
    {
        loger << debug << "Client1 ThreadID[" << CURRENT_THREAD_ID << "] " << "received evStart.";
        client_->gen(new evStart(this));
    }

    void onResponse(evResponse*)
    {
        loger << debug << "Client1 ThreadID[" << CURRENT_THREAD_ID << "] " << "received evResponse.";
    }

public:
    Client2* client_;
};

/** Example:
* 1. main start the process with evStart.
* 2. Client1 notify Client2 start its process with evStart(this).
* 3. Client2 response a message to Client1 after completed receiving and handling evStart process.
**/
int main()
{
    threadPool->CreateThreads();

    Client1 client1;
    client1.gen(new evStart);
    threadPool->joinThreads();


    return 0;
}