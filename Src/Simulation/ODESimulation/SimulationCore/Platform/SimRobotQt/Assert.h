

#ifndef __ASSERT_H__
#define __ASSERT_H__

#ifdef ASSERT
#error ASSERT already defined
#endif

#ifndef NDEBUG

class Assert
{
public:
  static void trace(const char* format, ...);
};

#include <assert.h>
#define ASSERT(e) assert(e)
#define VERIFY(e) ASSERT(e)
#define TRACE(...) Assert::trace(__VA_ARGS__)

#else
#define ASSERT(e)
#define VERIFY(e) e
#define TRACE(...)
#endif

#endif //!__ASSERT_H__
