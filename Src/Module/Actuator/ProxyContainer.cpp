#include "ProxyContainer.h"
#include "almotionproxy.h"
#include "almemoryproxy.h"

ProxyContainer::ProxyContainer()
{
//	sonarProxy=NULL;
	motionProxy=NULL;
	almemoryProxy=NULL;
	ttsProxy = NULL;

	//--Init
#ifndef NAO_DEBUG
	registerProxy();
#endif
	
}

ProxyContainer::~ProxyContainer()
{
//	if (sonarProxy != NULL)
//	{
//		delete sonarProxy;
//		sonarProxy = NULL;
//	}
	if (motionProxy != NULL)
	{
		delete motionProxy;
		motionProxy = NULL;
	}
	if (almemoryProxy != NULL)
	{
		delete almemoryProxy;
		almemoryProxy = NULL;
	}
	if (ttsProxy != NULL)
	{
		delete ttsProxy;
		ttsProxy = NULL;
	}
}

void ProxyContainer::registerProxy()
{
	std::string brokerName = "ForActuator";
	std::string brokerIP = "0.0.0.0";
	int brokerPort = 54000 ;
	std::string parentBrokerIP = "127.0.0.1";  	// Default parent broker IP
	int parentBrokerPort = 9559;  //kBrokerPort=9559			// Default parent broker port

	std::cout << "Try to connect to parent Broker at ip :" << parentBrokerIP
		<< " and port : " << parentBrokerPort << std::endl;
	std::cout << "Start the server bind on this ip :  " << brokerIP
		<< " and port : " << brokerPort << std::endl;

	//--------Broker
	try
	{
		broker = AL::ALBroker::createBroker(brokerName, brokerIP, brokerPort, parentBrokerIP, parentBrokerPort);
	}
	catch(ALError &e)
	{
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << "Creation of broker failed. Application will exit now." << std::endl;
		std::cout << "-----------------------------------------------------" << std::endl;
		std::cout << e.toString() << std::endl;
		//exit(0);
	}

	//--------MotionProxy
	try
	{
		motionProxy = new AL::ALMotionProxy(broker);
	} catch (ALError& )
	{
		std::cout<<"ALMotionProxy Creating Problem!!"<<std::cout;
	}
	//--------MemoryProxy
	try
	{
		almemoryProxy = new AL::ALMemoryProxy(broker);
	} catch(ALError&)
	{
		std::cout<<"ALMemoryProxy Creating Problem!!"<<std::cout;
	}
	//--------SonarProxy
//	try
//	{
//		sonarProxy = new AL::ALSonarProxy(broker);
//	} catch (ALError& )
//	{
//		std::cout<<"SonarProxy Creating Problem!!"<<std::cout;
//	}

	try
	{
		ttsProxy = new AL::ALTextToSpeechProxy(broker);

	} catch(ALError&)
	{
		std::cout<<"ttsProxy Creating Problem!!"<<std::cout;
	}
}
