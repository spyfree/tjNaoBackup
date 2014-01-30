#ifndef __LearnerAgent_h_
#define __LearnerAgent_h_
#include "../SPController.h"
class Learner;
class LearnerAgent
{
public:
	enum
	{
		PGM = 0
	};
	LearnerAgent();
	~LearnerAgent();
	void initPGM(const SPController& spc);
	bool canLearningStop();
	void getPendingParam(SPController& spController);
	void getBestParam(SPController& spController);
	void updatePerform(double perform);
private:
	Learner* pLearner;
};
#endif