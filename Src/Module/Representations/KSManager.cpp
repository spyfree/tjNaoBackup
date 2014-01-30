// TODO: check possible memory leak with this class!!
#include "KSManager.h"
#include <iostream>
#include "Module/Sensor/Sensor.h"
#include "Module/Behavior/TJRobot.h"
#include "Platform/GetSetting.h"
//#include <Module\Vision\Vision.h>
//#include <Module\Location\Loc.h>
#include <fstream>
#include <algorithm>

KSManager::KSManager():
	timeStamp(0)
{
	//push back all ks available in our project in our ksAents
	for(std::list<KSBase*>::iterator itr = KSBase::kses.begin(); itr != KSBase::kses.end(); ++itr)
	{
		ksAgents.push_back(*itr);
	}
}

KSManager::KSManager(const std::string& nameThread)
{
	//2 Load needed ks of this thread from cfg file...
	//  InTextFile stream("Processes/" + process + "Modules.cfg");
	std::list<std::string > filter;
	std::string buffer;
	managername=nameThread;

	std::cout<<"init KSManager"<<std::endl;
#ifdef NAO_DEBUG
	std::string path = std::string(GetSetting::GetConfigDir()) +"/MessageConfig/Simulation/"+ nameThread + "ks.cfg";
#else
	std::string path = std::string(GetSetting::GetConfigDir()) +"/MessageConfig/Nao/"+ nameThread + "ks.cfg";
#endif
	std::ifstream stream(path.c_str());
	//std::cout<<path<<std::endl;
    if(!stream.is_open())
    {
        std::cout<<path<<" Open failed! "<<std::endl;
    }
	ASSERT(stream.is_open());
	while(!stream.eof())
	{
		stream>>buffer;

		if(buffer == "[KS]")
			break;
	}
	std::cout<<"KSManager [KS]  over"<<std::endl;
#ifndef NAO_DEBUG
	while(!stream.eof())
	{
		stream >> buffer;
		if(buffer == "[Requirements]")
		{
			stream.close();
			break;
		}
		else
		{

			filter.push_back(buffer);
		}
	}
#else
	while(!stream.eof())
	{
		stream >> buffer;
		if(buffer == "[Requirements]")
		{
			stream.close();
			break;
		}
		else
		{
			if(SignalControl::getInstance()->locatorSignal == SignalControl::LODE)
			{
				if(buffer == "LogPercept")
					continue;
				if(SignalControl::getInstance()->visionSignal == SignalControl::VOFFLINE)
				{
					if(buffer == "ODEData" || buffer == "SelfLocator"
						|| buffer == "ODEOutBehaviorProvider" || buffer == "BehaviorSimProvider")
						continue;
				}
				filter.push_back(buffer);
			}
			else if(SignalControl::getInstance()->locatorSignal == SignalControl::LOFFLINE)
			{
				if(buffer == "LogPercept" || buffer == "SelfLocator" || buffer == "BallLocator")
				{
					filter.push_back(buffer);
					continue;
				}
			}
		}
	}
#endif
	std::cout<<"KSManager [Requirements]  over  "<<std::endl;
	//put need kses into ksAgents
	for(std::list<KSBase*>::iterator itrKSBase = KSBase::kses.begin(); itrKSBase != KSBase::kses.end(); ++itrKSBase)
	{
		//std::cout<<"Checking KS: "<<((*itrKSBase)->name)<<std::endl;
		if(find(filter.begin(), filter.end(), (*itrKSBase)->name) != filter.end())
		{
			//std::cout<<"KSManager push_back  over  name   "<<((*itrKSBase)->name)<<std::endl;
			ksAgents.push_back(*itrKSBase);
			//std::cout<<"Push_Back "<<((*itrKSBase)->name)<<std::endl;
		}
	}
	std::cout<<"KSManager init  over"<<std::endl;
}

KSManager::~KSManager()
{
	destroy();
}
//2 This function should be called in the run() of each Thread!! So that the memory allocated within this tread can be freed!!
void KSManager::destroy()
{
	// Delete all modules
	for(std::list<KSAgent>::iterator j = ksAgents.begin(); j != ksAgents.end(); ++j)
		if(j->ksInstance)
		{
			delete j->ksInstance;
			j->ksInstance = 0;
		}
	// Delete all provided representations
	for(std::list<Provider>::iterator j = providers.begin(); j != providers.end(); ++j)
	{
		j->free();
	}
	// Delete all required representation
	for(std::list<Requirement>::iterator j = requires.begin(); j != requires.end(); ++j)
	{
		j->free();
	}
}
#if 1
#define KSM_DEBUG_PRINT(msg) {std::cout<<msg;}
#else
#define KSM_DEBUG_PRINT(msg)
#endif
// TODO: feed a stream param to enable change of config source. This is better to be done after the In/Out stream completed for the disk file reading and writing
//2 This function should be called in the run() of each thread!!
void KSManager::update(unsigned _timeStamp)
{
	std::string strRequire, strRep, strKS;
	providers.clear();
	//Remove all markings
	for(std::list<KSAgent>::iterator itr = ksAgents.begin(); itr != ksAgents.end(); ++itr)
		itr->required = false;
	//In the construction, we already get which KS are needed, now, build requirements and providers

	//std::string path = "Config/" + managername + "ks.cfg";
#ifdef NAO_DEBUG
	std::string path = std::string(GetSetting::GetConfigDir()) +"/MessageConfig/Simulation/"+ managername + "ks.cfg";
#else
	std::string path = std::string(GetSetting::GetConfigDir()) +"/MessageConfig/Nao/"+ managername + "ks.cfg";
#endif
    KSM_DEBUG_PRINT("ksmanager    update: "<<path.c_str()<<std::endl)
	std::ifstream stream(path.c_str());
	ASSERT(stream.is_open());
	while(!stream.eof())
	{
		stream >> strRequire;

		if(strRequire == "[Requirements]")
		{

			break;
		}
	}
	
	//build requirements list
	while(!stream.eof())
	{
		stream >> strRequire;
		if(strRequire == "[Provides]")
			break;
		else{

			Requires::List::iterator itr;
			itr = find(Requires::entries.begin(), Requires::entries.end(), strRequire);
			if(itr != Requires::entries.end())
			{
				Requirement req(strRequire);
				req.create = ((*itr).create);
				req.free = ((*itr).free);
				requires.push_back(req);
			}
		}
	}

	//build provides list
	while(!stream.eof())
	{
		stream >> strRep >> strKS;
		if(strKS == "[END]")
			break;
		else{
			Provides::List::iterator i;
			std::list<KSAgent>::iterator j;
			Provides::ProviderPair ptmp(strRep, strKS);
			i = find(Provides::entries.begin(), Provides::entries.end(), ptmp);
			j = find(ksAgents.begin(), ksAgents.end(), strKS);
			if(i != Provides::entries.end() && j != ksAgents.end())
			{
				Provider req(strRep, strKS,  &(*j), ((*i).update), ((*i).create), ((*i).free));
				providers.push_back(req);
				j->required = true;
				//KSM_DEBUG_PRINT(managername.c_str()<<" Add: "<<strRep.c_str()<<" <=== "<<strKS.c_str()<<std::endl)
			}
		}
	}
	
	stream.close();
	// Delete all modules that are not required anymore
	for(std::list<KSAgent>::iterator j = ksAgents.begin(); j != ksAgents.end(); ++j)
		if(!j->required && j->ksInstance)
		{
			delete j->ksInstance;
			j->ksInstance = 0;
		}
		// Create all provided representations that are missing
		for(std::list<Provider>::iterator j = providers.begin(); j != providers.end(); ++j)
		{

			j->create();
		}
		// Create all required representation that are missing
		for(std::list<Requirement>::iterator j = requires.begin(); j != requires.end(); ++j)
		{
			j->create();
		}
		//Create all KS that are missing
		for(std::list<KSAgent>::iterator j = ksAgents.begin(); j != ksAgents.end(); ++j)
		{
			if(j->required && !j->ksInstance)
			{
				j->ksInstance = j->ks->createNew();
			}
		}
		this->timeStamp = _timeStamp;
}

void KSManager::execute()
{

	// Execute all providers in the given sequence
	for(std::list<Provider>::iterator itr = providers.begin(); itr != providers.end(); ++itr)
	{
		if(itr->ksAgent->ksInstance)
			itr->update(itr->ksAgent->ksInstance);
       //std::cout<<itr->ksname<<" is update"<<std::endl;
	}
}
