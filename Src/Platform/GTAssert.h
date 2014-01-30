/**
* @file Platform/GTAssert.h
*
* Inclusion of platform dependend definitions for low level debugging.
*
* @author <a href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</a>
*/

#ifndef GTASSERT_INCLUDED





#ifdef WIN32
#include "Win32/GTAssert.h"
#define GTASSERT_INCLUDED
#else
#include "linux/GTAssert.h"
#define GTASSERT_INCLUDED
#endif




#ifndef GTASSERT_INCLUDED
#error "Unknown platform or target"
#endif

#endif
