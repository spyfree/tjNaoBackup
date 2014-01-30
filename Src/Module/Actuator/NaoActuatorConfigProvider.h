#ifndef __NaoActuatorConfigProvider_h_
#define __NaoActuatorConfigProvider_h_

#include "Module/Representations/KS.h"
#include "Module/Common/NaoConfig.h"
DEFINE_KS(NaoActuatorConfigProvider)
PROVIDES(NaoConfig)
END_KS

class NaoActuatorConfigProvider : public NaoActuatorConfigProviderBase
{
public:
	NaoActuatorConfigProvider(){}
	~NaoActuatorConfigProvider(){}
private:
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
	void update(NaoConfig* theNaoConfig);
	void outNaoConfig();
};
#endif //__NaoActuatorConfigProvider_h_
