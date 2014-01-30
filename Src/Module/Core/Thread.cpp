#include "Thread.h"
//#include "alxplatform.h"
#include <iostream>
using namespace std;
//=========Mutex Implement===========//
Mutex::Mutex()
{
	mutex = SmartPtr<pthread_mutex_t>(new pthread_mutex_t(), MutexDeleter());
	pthread_mutex_init(mutex.get(), NULL);
}
Mutex::Mutex(SmartPtr<pthread_mutex_t> _mutex)
	:mutex(_mutex)
{
}
//=========Event Implement===========//
Event::Event(std::string _name)
	:name(_name), signalled(false)
{
	mutex = SmartPtr<Mutex>(new Mutex());
}
Event::Event(std::string _name, SmartPtr<Mutex> _mutex)
	:name(_name), signalled(false), mutex(_mutex)
{
}
Event::~Event()
{
}
void Event::wait()
{
	mutex->lock();
	if (!signalled)
	{
		cond.wait(mutex);
	}
	signalled = false;
	mutex->unlock();
}

void Event::signal()
{
	mutex->lock();
	signalled = true;
	cond.signal();
	mutex->unlock();
}

bool Event::poll()
{
	mutex->lock();
	const bool result = signalled;
	signalled = false;
	mutex->unlock();
	return result;
}
//=========Synchronizer Implement===========//
Synchronizer::Synchronizer()
	:events_pool()
{

}
Synchronizer::~Synchronizer()
{

}

SmartPtr<Event> Synchronizer::registerEvent(std::string _name)
{
	map<string, SmartPtr<Event> >::iterator itr = events_pool.find(_name);
	if(itr == events_pool.end())
	{
		const SmartPtr<Event> e(new Event(_name));
		events_pool[_name] = e;
	}
	return events_pool[_name];
}

SmartPtr<Event> Synchronizer::registerEvent(std::string _name, SmartPtr<Mutex> _mutex)
{
	map<string, SmartPtr<Event> >::iterator itr = events_pool.find(_name);
	if(itr == events_pool.end())
	{
		const SmartPtr<Event> e(new Event(_name, _mutex));
		events_pool[_name] = e;
	}
	return events_pool[_name];
}

SmartPtr<Event> Synchronizer::getEventbyName(std::string _name)
{
	map<std::string, SmartPtr<Event> >::iterator itr = events_pool.find(_name);
	if(itr == events_pool.end())
	{
		cout<<"Error! No such Event called "<<_name<<endl;
		return SmartPtr<Event>();
	}else{
		return events_pool[_name];
	}
}

void Synchronizer::wait(Event* _event)
{
	_event->wait();
}

bool Synchronizer::poll(Event* _event)
{
	return _event->poll();
}

void Synchronizer::signal(Event* _event)
{
	_event->signal();
}

void Synchronizer::printEventsName()
{
	map<std::string, SmartPtr<Event> >::iterator itr = events_pool.begin();
	cout<<"=========Print Out Events Names================"<<endl;
	while(itr != events_pool.end())
	{
		cout<<itr->first<<endl;
		itr++;
	}
}
//=========Trigger Implement===========//
Trigger::Trigger(SmartPtr<Synchronizer> _synchronizer, std::string _name, bool _state)
	:mutex(SmartPtr<Mutex>(new Mutex)),
	on_event(_synchronizer->registerEvent(_name + "_on", mutex)),
	off_event(_synchronizer->registerEvent(_name + "_off", mutex)),
	flip_event(_synchronizer->registerEvent(_name + "_flip", mutex)),
	state(_state)
{
	// Set mutex to recursive
	pthread_mutexattr_t attrs;
	pthread_mutexattr_init(&attrs);
	pthread_mutexattr_settype(&attrs, PTHREAD_MUTEX_RECURSIVE);
	// Initialize mutex
	pthread_mutex_init(mutex->getptheadMutex(), &attrs);

	//pthread_mutexattr_destroy(&attrs);
}
void Trigger::flip()
{
	mutex->lock();
	state = !state;
	if(state)
	{
		on_event->signal();
	}else
	{
		off_event->signal();
	}
	flip_event->signal();
	mutex->unlock();
}
void Trigger::on()
{
	mutex->lock();
	if(!state){
		state = true;
		on_event->signal();
		flip_event->signal();
	}
	mutex->unlock();
}
void Trigger::off()
{
	mutex->lock();
	if(state)
	{
		state = false;
		off_event->signal();
		flip_event->signal();
	}
	//------add by chen
		mutex->unlock();
}
bool Trigger::poll()
{
	mutex->lock();
	const bool _v = state;
	mutex->unlock();
	return _v;
}
void Trigger::wait_on()
{
	if(!poll())
		on_event->wait();
}
void Trigger::wait_off()
{

	if(poll())
		off_event->wait();
}
void Trigger::wait_flip()
{
	flip_event->wait();
}

//=========Thread Implement===========//
Thread::Thread(SmartPtr<Synchronizer> _synchronizer, std::string _name)
	:theSynchronizer(_synchronizer),
	name(_name), running(false)

{
	spMyTrigger = SmartPtr<Trigger>(new Trigger(theSynchronizer, _name, false));
	theSynchronizer->printEventsName();
}
Thread::~Thread()
{

}

int Thread::start()
{
	if(running) return -1;
	// Set thread attributes
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	//
	threadID = pthread_create(&mythread, &attr, runThread, (void*)this);
	// Free attribute data
	pthread_attr_destroy(&attr);
	return threadID;
}
void Thread::stop()
{
	running = false;
  #ifdef  DEBUG_THREAD
	std::cout<<"stop "<<name<<" thread"<<std::endl;
 #endif
}

void *Thread::runThread (void* _this)
{
	reinterpret_cast<Thread*>(_this)->run();
	pthread_exit(NULL);
	return NULL;
}
/*
void Thread::run()
{
	running = true;
    while(running){
	cout << "I'ms Thread :"<<threadID<<endl;
	//SleepMs( 1000);
    }
}
*/
