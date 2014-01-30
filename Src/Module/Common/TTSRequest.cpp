#include "TTSRequest.h"
#include <iostream>
extern ALPtr<ALBroker>test_pBroker;

TTSRequest::TTSRequest()
{
	try
	{
		ttsproxy=ALPtr<ALTextToSpeechProxy>(new ALTextToSpeechProxy(test_pBroker));
	}
	catch(AL::ALError &e){
		cout<<"Failed to initialize LEDproxy" << endl;
	}
}
TTSRequest::~TTSRequest()
{

}

void TTSRequest::tts_say( std::string pStringToSay )
{
	ttsproxy->say(pStringToSay);
}
