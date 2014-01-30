#ifndef __SimFTimer_h_
#define __SimFTimer_h_
/**
* A Frame Timer for Simulation. A TImer Counter Down Each Frame.
*/
#define SIMFTIMER_INIT() SimFTimer::Init()
#define SIMFTIMER_SET(id, t) (SimFTimer::getInstance()->setFTimer(id,t))
#define SIMFTIMER_CHECK(id) (SimFTimer::getInstance()->checkFTimer(id))
#define SIMFTIMER_ISACTIVE(id) (SimFTimer::getInstance()->isActive(id))
#define SIMFTIMER_RESET(id) (SimFTimer::getInstance()->resetFTimer(id))
#define SIMFTIMER_RESETALL() (SimFTimer::getInstance()->resetAllFTimer())
#define SIMFTIMER_COUNTDOWN() (SimFTimer::getInstance()->counteDown())
class SimFTimer
{
public:
	enum{
		tDoRecover,
		tNoticeRecovered,
		tImpulseRobot, 
		tScoreRobot,
		NumTimer
	};
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}
		void reset()
		{
			counter = 0;
			active = false;
			timeup = false;
		}
		void setTime(int time)
		{
			counter = time;
			active = true;
			timeup = false;
		}
		int counter;
		bool active;
		bool timeup;
	};
protected:
	SimFTimer();
	static SimFTimer* theSimFTimer;
	Timer timers[NumTimer];
public:
	void setFTimer(int id, int time);
	bool checkFTimer(int id);
	void resetFTimer(int id);
	void resetAllFTimer();
	void counteDown();
	bool isActive(int id)
	{
		if(id < NumTimer)
			return timers[id].active;
		else
			return false;
	}
public:
	static void Init();
	static SimFTimer* getInstance();
};
#endif