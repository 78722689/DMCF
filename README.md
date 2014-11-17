DMCF
====
Distributed Message Communication Framework
--------------------------------------------
  C++ Library 
  Current Version: v1.0.0 

Introduction
---------------
  A c++ library in Linux to provide the basic communication 
  in Multi-threads, Multi-processes and Multi-Nodes. 

Requirements
------------
  C++11, CMAKE

Building
--------
  1. Create a empty folder with name "build" under the root directory. 
  2. cd build/ 
  3. cmake .. 
  4. make 

Usage
------
    // define message/event id 
    #define evStart_TEST_id 0x01 
    #define evResponse_TEST_id 0x02 
     
    // define message/event 
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

    // An example for show how the event works with 
    // Reactive, and communicates between threads.
    class Client2 : public ReactiveAdapter< Client2 >
    {
    public:
        Client2()
        {
            setThread(threadPool->getThread()->holdWithParameters("Client2"));
            loger << debug 
                  << "Client1 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] "
                  << "setthread";
            registerEventToReactive(&Client2::onStart);
        }
        ~Client2(){}

    public:
        void onStart(evStart* ev)
        {
            loger << debug 
                  << "Client2 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] " 
                  << "received evStart.";
            ev->src_->gen(new evResponse);
        }

    };

    class Client1 : public ReactiveAdapter< Client1 >
    {
    public:
        Client1()
        {
            setThread(threadPool->getThread()->holdWithParameters("Client1"));
            loger << debug 
                  << "Client1 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] "
                  << "setthread";
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
            
            loger << debug 
                  << "Client1 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] " 
                  << "received evStart.";

            client_->gen(new evStart(this));
        }

        void onResponse(evResponse*)
        {
            loger << debug 
                  << "Client1 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] " 
                  << "received evResponse.";
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
            loger << debug 
                  << "Client3 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] "
                  < "setthread";
            registerEventToReactive(&Client3::onResponse);
            client_->gen(new evStart(this));
        }
        Client3(){}

    public:
       void onResponse(evResponse*)
        {
            loger << debug 
                  << "Client3 ThreadID[" 
                  << OS_TOOLS::OSGetCurrentThreadId() 
                  << "] " 
                  << "received evResponse.";
        }

    private:
        Client2* client_;
    };


    /** Example:
    * 1. main start the process with evStart.
    * 2. Client3 notify Client2 start its process with evStart(this).
    * 3. Client2 response a message to Client3 after completed receiving and handling 
    * evStart process.
    * 4. Client 1 start the process with evStart and notify Client2 to start up, soon Client2
    * response response a message to Client 1.
    * Note: Don't send message to a temp client, probably the thread is not handle the 
    * message yet and the temp client has been released.
    **/
    int main()
    {
        threadPool->CreateThreads();

        Client1 client1;
        Client3 client3(client1.client_);
        client1.gen(new evStart);
        threadPool->joinThreads();

        loger << debug << "main exit.........";
        return 0;
    }



