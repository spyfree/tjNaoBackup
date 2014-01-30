#ifndef __NaoVisionConfigProvider_h_
#define __NaoVisionConfigProvider_h_

#include "Module/Representations/KS.h"
#include "Module/Common/NaoConfig.h"
#include "Module/Representations/Perception/CameraConfig.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
DEFINE_KS(NaoVisionConfigProvider)
PROVIDES(NaoConfig)
PROVIDES(CameraConfig)
PROVIDES(FieldDimensions)
END_KS

class NaoVisionConfigProvider : public NaoVisionConfigProviderBase
{
public:
	NaoVisionConfigProvider(){}
	~NaoVisionConfigProvider(){}
private:
	/************************************************************************/
	/*		Implement functions derived from Base                                        */
	/************************************************************************/
	void update(NaoConfig* theNaoConfig);
	void update(CameraConfig* theCameraConfig);
	void update(FieldDimensions* theFieldDimensions);
	void outNaoConfig();
	void outCameraConfig();
	void outFieldDimensions();

	
};
#endif //__NaoVisionConfigProvider_h_