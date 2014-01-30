#ifndef __PolicyGradientLearner_h_
#define __PolicyGradientLearner_h_
#include "Learner.h"
#include <vector>
/** A class doing policy gradient method
* refer to "Policy Gradient Reinforcement Learning for Fast Quadrupedal Locomotion"
*/
class PolicyGradientLearner : public Learner
{
public:
	PolicyGradientLearner(const Param& initParam_);
	~PolicyGradientLearner();
	virtual void updatePerform(double perform);
	virtual bool canStopLearning();
	virtual const Param& getPendingParam() const;
	virtual const Param& getBestParam() const { return bestParam; }
private:
	bool updatePendingParam(double perform);
	void genNewPolicies();
private:
	enum{
		NUM_ATOMIC_POLICY = 3, 
		MAX_ITERATIONS = 5000
	};
	int numPendings;/**< number of pending policies in each iteration*/
	std::vector< Param > R;/**< Pending params in each iteration*/
	double eta; /**< ¦Ç: Step length of each iteration*/
	double* Epsilons;/**< ¦Å : Step length of each gene of each testing param*/
	double* Avg[NUM_ATOMIC_POLICY];
	int* sums[NUM_ATOMIC_POLICY];
	double* A;
	static const int AtomicPolicy[3];
	double minScore;
	int stopCounter;	/**< A counter to test when we meet the minimum*/
	int totalStep;	/**< Iterations did since now*/
	Param* pPendingParam;
	double performance[3];
	int timesTested;
	Param currentParam;
	Param bestParam;
};
#endif