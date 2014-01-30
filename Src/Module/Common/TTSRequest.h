#ifndef TTSRequest_H
#define TTSRequest_H
#include "altexttospeechproxy.h"
using namespace AL;
class TTSRequest
{
public:
	TTSRequest();
	~TTSRequest();
	void tts_say (string pStringToSay);

private:
	ALPtr<ALTextToSpeechProxy> ttsproxy;

};
#endif//TTSRequest_H
