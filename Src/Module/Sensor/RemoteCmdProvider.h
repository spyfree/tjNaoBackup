#ifndef __RemoteCmdProvider_h_
#define __RemoteCmdProvider_h_
#include "Module/Representations/BaseProvider.h"
#include "Module/Common/MotionCmd.h"
class RemoteCmdProvider : public BaseProvider
{
public:
RemoteCmdProvider();
~RemoteCmdProvider();

void setMotionCmd(const MotionCmd& motionCmd_);
};
#endif //__RemoteCmdProvider_h_
