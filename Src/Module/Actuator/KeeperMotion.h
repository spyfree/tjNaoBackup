#ifndef __KEEPERMOTION_H
#define __KEEPERMOTION_H

#include "Module/Representations/Modeling/BallModel.h"
#include "Module/Representations/BaseProvider.h"
#include "ProxyContainer.h"

class keeperMotion:public BaseProvider
{
public:
	keeperMotion();
	~keeperMotion();

	void run();

private:
	void getTeamNum();
	void inBallModel();
	int shouldDive();
	void diveLeft();
	void diveRight();

private:
	int teamNum;
	BallModel *theCurrentBallModel;
	ProxyContainer myProxyContainer;

};
#endif