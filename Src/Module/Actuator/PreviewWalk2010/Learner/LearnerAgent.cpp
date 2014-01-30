#include "LearnerAgent.h"
#include "PolicyGradientLearner.h"
LearnerAgent::LearnerAgent():pLearner(0)
{

}
LearnerAgent::~LearnerAgent()
{
	if(pLearner){
		delete pLearner;
		pLearner = 0;
	}
}
void 
LearnerAgent::initPGM(const SPController& spc)
{
	const SPController::Gain& gainx = spc.getGainX();
	const SPController::Gain& gainy = spc.getGainY();
	Learner::Param param(8);
	param.genes[0] = gainx.K.x;
	param.genes[1] = gainx.K.y;
	param.genes[2] = gainx.K.z;
	param.genes[3] = gainx.G;
	param.genes[4] = gainy.K.x;
	param.genes[5] = gainy.K.y;
	param.genes[6] = gainy.K.z;
	param.genes[7] = gainy.G;
	pLearner = new PolicyGradientLearner(param);
}
bool 
LearnerAgent::canLearningStop()
{
	if(pLearner)
		return pLearner->canStopLearning();
	else
		return true;
}
void 
LearnerAgent::getPendingParam(SPController& spController)
{
	if(pLearner)
	{
		const Learner::Param& param = pLearner->getPendingParam();
		SPController::Gain gainx,gainy;
		gainx.K = Vector3<double>(param.genes[0],  param.genes[1], param.genes[2]);
		gainx.G = param.genes[3];
		gainy.K = Vector3<double>(param.genes[4],  param.genes[5], param.genes[6]);
		gainy.G = param.genes[7];
		spController.setGainX(gainx);
		spController.setGainY(gainy);
	}
}
void LearnerAgent::getBestParam(SPController& spController)
{
	if(pLearner)
	{
		const Learner::Param& param = pLearner->getBestParam();
		SPController::Gain gainx,gainy;
		gainx.K = Vector3<double>(param.genes[0],  param.genes[1], param.genes[2]);
		gainx.G = param.genes[3];
		gainy.K = Vector3<double>(param.genes[4],  param.genes[5], param.genes[6]);
		gainy.G = param.genes[7];
		spController.setGainX(gainx);
		spController.setGainY(gainy);
	}
}
void LearnerAgent::updatePerform(double perform)
{
	if(pLearner)
		pLearner->updatePerform(perform);
}