#ifndef __Thread_H_
#define __Thread_H_
#include <string>
#include <map>
#include <pthread.h>
#include <iostream>
//#include "SmartPtr.h"
#include "Module/Common/SmartPtr.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#define DEBUG_THREAD
struct MutexDeleter
{
	void operator() (pthread_mutex_t *mutex) { pthread_mutex_destroy(mutex); }
};
/**
* Default Mutex wrapper class, default mutex attribute setting
*/
class Mutex
{
public:
	Mutex();
	Mutex(SmartPtr<pthread_mutex_t> _mutex);
	~Mutex(){}
public:
	inline int lock() {return pthread_mutex_lock(mutex.get()); }
	inline int tryLock() { return pthread_mutex_trylock(mutex.get()); }
	inline int unlock() { return pthread_mutex_unlock(mutex.get()); }
	inline SmartPtr<pthread_mutex_t> getMutex() { return mutex; }
	inline pthread_mutex_t* getptheadMutex() {return mutex.get();}
private:
	SmartPtr<pthread_mutex_t> mutex;
//	pthread_attr_t attr;
};
/**
* Default Condition wrapper class, default Condition attribute setting
*/
class Condition
{
public:
	Condition() { pthread_cond_init(&cond, NULL); }
	~Condition() { pthread_cond_destroy(&cond);  }
	inline int wait(SmartPtr<Mutex> _mutex) { return pthread_cond_wait(&cond, _mutex->getptheadMutex()); }
	inline int broadcast() {return pthread_cond_broadcast(&cond); }
	inline int signal() { return pthread_cond_signal(&cond);  }
private:
	pthread_cond_t cond;
};

class Event
{
public:
	Event(std::string _name);
	Event(std::string _name, SmartPtr<Mutex> _mutex);
	~Event();
public:
	void wait();
	void signal();
	/**
	* Query for signalled, and clear signalled flag;
	* @return has signalled
	*/
	bool poll();
public:
	std::string name;
private:
	SmartPtr<Mutex> mutex;
	Condition cond;
	bool signalled;
};

class Synchronizer
{
public:
	Synchronizer();
	virtual ~Synchronizer();
public:
	 //Register a new event with the given name
	SmartPtr<Event> registerEvent(std::string _name);
	SmartPtr<Event> registerEvent(std::string _name, SmartPtr<Mutex> _mutex);
	SmartPtr<Event> getEventbyName(std::string _name);
	// Wait for an event to be signalled, and clear the signal
	void wait(Event* _event);
	// Check if an event has been signalled, and clear the signal
	bool poll(Event* _event);
	// Signal an event has occurred
	void signal(Event* _event);
	//
	void printEventsName();
private:
	std::map<std::string, SmartPtr<Event> > events_pool;
	
};

class Trigger
{
public:
	Trigger(SmartPtr<Synchronizer> _synchronizer, std::string _name, bool _state = false);
	~Trigger() {}

	void flip();
	void on();
	void off();
	//check the trigger state, DO NOT clear the state
	bool poll();
	void wait_on();
	void wait_off();
	void wait_flip();
private:
	SmartPtr<Mutex> mutex;
	SmartPtr<Event> on_event;
	SmartPtr<Event> off_event;
	SmartPtr<Event> flip_event;
	bool state;
};

class Thread
{
public:
	Thread(SmartPtr<Synchronizer> _synchronizer, std::string _name);
	~Thread();
	
	int start();
	void stop();
	virtual void run() = 0;
	bool isRunning() {return running; }
	// These are/should only fired once!  be careful, or deadlock could ensue
	const SmartPtr<Trigger> getTrigger() { return spMyTrigger; }

private:
	static void *runThread(void* _thread);
protected:
	bool running;
	int threadID;
	SmartPtr<Trigger> spMyTrigger;
	SmartPtr<Synchronizer> theSynchronizer;
private:
	pthread_t mythread;
public:
	std::string name;
	
};

#endif