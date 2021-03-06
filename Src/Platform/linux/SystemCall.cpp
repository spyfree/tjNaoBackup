/**
* @file  Platform/linux/SystemCall.cpp
* @brief static class for system calls, Linux implementation
* @attention this is the Linux implementation
*
* @author <A href=mailto:brunn@sim.informatik.tu-darmstadt.de>Ronnie Brunn</A>
* @author <A href=mailto:martin@martin-loetzsch.de>Martin L�tzsch</A>
* @author <A href=mailto:risler@sim.informatik.tu-darmstadt.de>Max Risler</A>
* @author <a href=mailto:dueffert@informatik.hu-berlin.de>Uwe Dffert</a>
*/

#include "Platform/SystemCall.h"
#include "Platform/GTAssert.h"
#include <sys/timeb.h>
#include <unistd.h>
#include <string>
using namespace std;

unsigned SystemCall::getCurrentSystemTime()
{
  return getRealSystemTime();
}

unsigned SystemCall::getRealSystemTime()
{
  static unsigned base = 0;
  timeb sysTime;
  ftime(&sysTime);
  unsigned time = unsigned(sysTime.time * 1000 + sysTime.millitm);
  if(!base)
    base = time - 10000; // avoid time == 0, because it is often used as a marker
  return time - base;
}

const char* SystemCall::getHostName()
{
  static char buf[100];
  VERIFY(!gethostname(buf, sizeof(buf)));
  return buf;
}



void SystemCall::sleep(unsigned long ms)
{
  usleep(1000 * ms);
}


