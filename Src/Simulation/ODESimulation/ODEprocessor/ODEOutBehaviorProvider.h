#ifndef ODEOutBehaviorProvider_h_DEFINED
#define ODEOutBehaviorProvider_h_DEFINED
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/FallDownState.h"
#include "Module/Common/KeyStates.h"
#include "Module/Common/JointData.h"
#include "Module/Representations/Infrastructure/OutBehaviorData.h"
#include "Module/Common/NaoStructure.h"

DEFINE_KS(ODEOutBehaviorProvider)
REQUIRES(FallDownState)
/*REQUIRES(KeyStates)*/
REQUIRES(SensoryJointData)
REQUIRES(NaoStructure)
PROVIDES(OutBehaviorData)
USES(OutBehaviorData)
END_KS

class ODEOutBehaviorProvider:public ODEOutBehaviorProviderBase
{
public:
	ODEOutBehaviorProvider();
	~ODEOutBehaviorProvider();
	void inFallDownState(){}
	/*void inKeyStates(){}*/
	void inSensoryJointData(){}
	void inNaoStructure(){}

	void update(OutBehaviorData *theOutBehaviorData);
	void outOutBehaviorData();
	void setBehaviorData();
	OutBehaviorData outBehaviorData;

};

#endif
