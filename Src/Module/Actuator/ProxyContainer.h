#ifndef PROXYCONTAINER_H
#define PROXYCONTAINER_H

#include "albroker.h"
#include <iostream>
#include "alerror.h"
#include "altexttospeechproxy.h"
#include "almotionproxy.h"

using namespace AL;

//==
class ProxyContainer
{
	public:
		ProxyContainer();
		~ProxyContainer();

	public:
		void registerProxy();

	public:
//		AL::ALSonarProxy * sonarProxy;
		AL::ALMotionProxy * motionProxy;
		AL::ALMemoryProxy * almemoryProxy;

		AL::ALTextToSpeechProxy * ttsProxy;

	private:
		AL::ALBroker::Ptr broker;
};

#endif	//PROXYCONTAINER_H
