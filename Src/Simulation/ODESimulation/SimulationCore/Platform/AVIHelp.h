#ifndef __AVIHelp_INCLUDE_

#ifdef WIN32
#include "Win32/AVIHelp.h"
#define __AVIHelp_INCLUDE_
#else
//#include "Linux/AVIHelp.h"
#define __AVIHelp_INCLUDE_
#endif
#ifndef __AVIHelp_INCLUDE_
#error "Unknown platform or target"
#endif
#endif