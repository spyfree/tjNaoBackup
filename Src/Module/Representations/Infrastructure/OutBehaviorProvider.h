#ifndef OutBehaviorProvider_h_DEFINED
#define OutBehaviorProvider_h_DEFINED
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/FallDownState.h"
#include "Module/Common/KeyStates.h"
#include "Module/Common/JointData.h"
#include "Module/Representations/Infrastructure/OutBehaviorData.h"
#include "Module/Common/NaoStructure.h"

DEFINE_KS(OutBehaviorProvider)
REQUIRES(FallDownState)
REQUIRES(KeyStates)
REQUIRES(SafeJointData)
REQUIRES(NaoStructure)
PROVIDES(OutBehaviorData)
USES(OutBehaviorData)
END_KS

class OutBehaviorProvider:public OutBehaviorProviderBase,BaseProvider
{
public:
    OutBehaviorProvider();
    ~OutBehaviorProvider();
	void inFallDownState(){}
	void inKeyStates(){}
	void inSafeJointData(){}
	void inNaoStructure(){}
	void inSpecActionOutput(){}

    void update(OutBehaviorData *theOutBehaviorData);
    void outOutBehaviorData();
    void setBehaviorData();
    OutBehaviorData outBehaviorData;
	
};

#endif
