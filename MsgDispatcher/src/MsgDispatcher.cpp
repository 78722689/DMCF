#include "MsgDispatcher.h"

using namespace Dispatcher;

template<typename T, typename MESSAGE>
void DefaultDispatcherThread::start(T* server, void (T::*DISPATCH)(const MESSAGE&))
{
    threadEntry(server, DISPATCH);
}

template<typename T, typename MESSAGE>
void DefaultDispatcherThread::threadEntry(T* server, void (T::*DISPATCH)(const MESSAGE&))
{
    // Thread begin here.
    while (true)
    {
        // TODO: Waiting for messages from IPC interface,. and then dispatch it to related instance.

        MESSAGE *msg;
        (server->*DISPATCH)(*msg);
    }
}

template<class THREAD>
template<typename MESSAGE>
void MsgDispatcher<THREAD>::notifyMessageToSubscribers(const MESSAGE& msg)
{
    RECEIVERS_PAIR_RANGE it = receiver_map_.equal_range(msg.getDestDescriptor());
    for (RECEIVERS_ELEMENT_ITERATOR element = it.first;  element != it.second; ++element)
    {
        element->second->handle(msg);
    }
}
