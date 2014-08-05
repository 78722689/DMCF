#include "dmcfos.h"
#include "definition.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

u32 DMCF_OSGetCurrentThread()
{
    return syscall(SYS_gettid);
}
