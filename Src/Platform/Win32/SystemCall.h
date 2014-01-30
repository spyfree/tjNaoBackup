#ifndef __SystemCall_h_
#define __SystemCall_h_

#define PROCESS_WIDE_STORAGE __declspec(thread)
#define PROCESS_WIDE_STORAGE_STATIC PROCESS_WIDE_STORAGE static
/**
* static class for system calls
* @attention the implementation is system specific!
*/
class SystemCall
{
public:
	/** returns the current system time in milliseconds*/
	static unsigned getCurrentSystemTime();

	/** returns the real system time in milliseconds (never the simulated one)*/
	static unsigned getRealSystemTime();

	/** returns the time since aTime*/
	inline static int getTimeSince(unsigned aTime) 
	{
		return (int) (getCurrentSystemTime() - aTime);
	}

	/** returns the real time since aTime*/
	inline static int getRealTimeSince(unsigned aTime) 
	{
		return (int) (getRealSystemTime() - aTime);
	} 
	/** Sleeps for some milliseconds.
	* \param ms The amout of milliseconds.
	*/
	static void sleep(unsigned int ms);
};
#endif