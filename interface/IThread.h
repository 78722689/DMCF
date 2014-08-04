#ifndef _I_Thread_H_
#define _I_Thread_H_

#define DEFAULT_THREAD_STACK_SIZE (5*1024)

typedef enum thread_state_e
{
	EUninitilization=0, // No config any parameters to the thread
	EInitlized, // Configured parameters
	EIdle,	// After reset the thread
	EBusy	// Running
} Thread_State;


// Buffer the tasks of one thread till it is idle.
class IThread
{
public:
	virtual ~IThread()
	{}
	virtual void start() = 0;
	virtual void send(void* data) = 0;
	virtual Thread_State getThreadState() const = 0;
	virtual void setThreadState(Thread_State eState) = 0;
	virtual bool stop() const = 0;
	virtual IThread* holdWithParameters(const char* name,
			void func(void*) = NULL,
			void* param = NULL,
			const long stackSize = DEFAULT_THREAD_STACK_SIZE) = 0;
	virtual void joinThread() const = 0;
};

#endif // _I_Thread_H_