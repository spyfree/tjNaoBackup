#ifndef __Comm_H_
#define __Comm_H_
#ifdef _WIN32
#define PROCESS_WIDE_STORAGE __declspec(thread)
#define PROCESS_WIDE_STORAGE_STATIC PROCESS_WIDE_STORAGE static
#else
#define PROCESS_WIDE_STORAGE __thread
#define PROCESS_WIDE_STORAGE_STATIC static PROCESS_WIDE_STORAGE
#endif

#define ASSERT(c) if(!(c)) {__asm{int 3};}

#endif