#include "PolicyGradientLearner.h"
#include "Tools/Math/Common.h"
//#include "../../MatlabHelp/MatlabHelp.h"
#include <cmath>
#include <cfloat>

const int PolicyGradientLearner::AtomicPolicy[PolicyGradientLearner::NUM_ATOMIC_POLICY] = {-1, 0, 1};

PolicyGradientLearner::PolicyGradientLearner(const Learner::Param& initParam_) : Learner(initParam_),
currentParam(initParam_), bestParam(initParam_), 
eta(0.01), stopCounter(0), totalStep(0), minScore(FLT_MAX),
pPendingParam(0),Epsilons(0), A(0), timesTested(0)
{
	bestParam.perform = 0;
	if(paramDim < 3)
		numPendings = static_cast<int>(pow((double)3, (double)paramDim));
	else
		numPendings = 20;
	Epsilons = new double[paramDim];
	double dsum = 0;
	for(int i = 0;i < paramDim; ++i){
		Epsilons[i] = 0.001;
		//Epsilons[i] =fabs(initialParam.genes[i] * 0.5);
		dsum += Epsilons[i] * Epsilons[i]; 
	}
	eta = sqrt(dsum);
	for(int i = 0; i < NUM_ATOMIC_POLICY; ++i)
	{
		Avg[i] = new double[paramDim];
		sums[i] = new int[paramDim];
	}
	A = new double[paramDim];
	R.reserve(numPendings);
	genNewPolicies();
}

PolicyGradientLearner::~PolicyGradientLearner()
{
	if(Epsilons)
	{
		delete[] Epsilons;
		Epsilons = 0;
	}
	if(A)
	{
		delete[] A;
		A = 0;
	}
	for(int i = 0; i < NUM_ATOMIC_POLICY; ++i)
	{
		if(Avg[i])
		{
			delete[] Avg[i];
			Avg[i] = 0;
		}
		if(sums[i])
		{
			delete[] sums[i];
			sums[i] = 0;
		}
	}

}

void PolicyGradientLearner::updatePerform(double perform)
{
	if(!updatePendingParam(perform))
		return;
	//========One iteration finished, calculate currentParam for next iteration==
	//memset(Avg, 0, paramDim * NUM_ATOMIC_POLICY * sizeof(double));
	//memset(sums, 0, paramDim * NUM_ATOMIC_POLICY * sizeof(int));
	for(int i = 0; i < NUM_ATOMIC_POLICY; ++i)
	{
		memset(Avg[i], 0, paramDim * sizeof(double));
		memset(sums[i], 0, paramDim * sizeof(int));
	}
	memset(A, 0, paramDim * sizeof(double));
	for(int j = 0; j < paramDim; ++j)
	{
		for(int i = 0; i < R.size(); ++i)
		{
			if(R[i].evaluated)
			{
				for(int p = 0; p < NUM_ATOMIC_POLICY; ++p)
				{
					if(R[i].genes[j] == currentParam.genes[j] + Epsilons[j] * AtomicPolicy[p])
					{
						Avg[p][j] += R[i].perform;
						sums[p][j]++;
						break;
					}
				}
			}
		}
		for(int p = 0; p < NUM_ATOMIC_POLICY; ++p)
			if(sums[p][j] > 0)
				Avg[p][j] = Avg[p][j] / sums[p][j];
			else
				Avg[p][j] = 0;
		// TODO: replace for more atomic policies extension
		if(Avg[1][j] > Avg[0][j] && Avg[1][j] > Avg[2][j])
		{
			A[j] = 0;
		}else
		{
			A[j] = Avg[2][j] - Avg[0][j];
		}
	}
	double mins = FLT_MAX;
	int minInd = 0;
	for(int i = 0;i < R.size(); ++i)
	{
		if(R[i].evaluated)
		{
			if(R[i].perform < mins){
				mins = R[i].perform;
				minInd = i;
			}
		}
	}
	bestParam = R[minInd];
	//========
	Param delta(paramDim);
	double sumA = 0;
	for(int i = 0;i < paramDim; ++i){
		delta.genes[i] = A[i];
		sumA += A[i]*A[i];
	}
	if(fabs(sumA)  > 0)
		delta = delta / sqrt(sumA) * (-eta);
	else
		delta = delta * 0;
	currentParam = currentParam + delta;
	if(mins >= minScore){
		stopCounter++;
		if(stopCounter >= 3)
		{
			double dsum = 0;
			for(int i = 0; i < paramDim; ++i)
			{
				Epsilons[i] *= 0.5;
				dsum = Epsilons[i]*Epsilons[i];
			}
			eta = sqrt(dsum);
			stopCounter = 0;
		}
	}
	else{
		stopCounter = 0;
		minScore = mins;
	}
	totalStep++;
	//===Generate new policies for next iteration=====
	double dsum = 0;
	for(int i = 0;i < paramDim; ++i){
		//	Epsilons[i] = 0.01;
		//Epsilons[i] =fabs(bestParam.genes[i] * 0.5);
		Epsilons[i] = randomDouble() * 0.01 + 0.001;
		dsum += Epsilons[i] * Epsilons[i]; 
	}
	eta = sqrt(dsum);
	genNewPolicies();
}
bool PolicyGradientLearner::canStopLearning()
{
	if(stopCounter > 10 || totalStep > MAX_ITERATIONS)
		return true;
	else
		return false;
}
void PolicyGradientLearner::genNewPolicies()
{
	R.clear();
	for(int i = 0; i < numPendings; ++i)
	{
		Param tmp(currentParam);
		for(int j = 0; j < paramDim; ++j)
		{
			int ind = random(NUM_ATOMIC_POLICY);
			tmp.genes[j] = tmp.genes[j] +  Epsilons[j] * AtomicPolicy[ind];
		}
		tmp.evaluated = false;
		R.push_back(tmp);
	}
	pPendingParam = &R[0];
}
const Learner::Param& PolicyGradientLearner::getPendingParam() const
{
	return *pPendingParam;
}
bool PolicyGradientLearner::updatePendingParam(double perform)
{
	performance[timesTested] = perform;
	timesTested++;
	//Each Parameters are tested three times, and mean of the closet two are used as final performance
	if(timesTested >= 3){
		
		double finalPerform = 0;
		int index = 0;
		double dist = 0, minDist = FLT_MAX;
		for(int i = 0; i < 3; ++i)
		{
			int i2 = i + 1;
			i2 = i2 >= 3? 0 : i2;
			dist = abs(performance[i] - performance[i2]);
			if(dist < minDist)
			{
				minDist = dist;
				index = i;
			}
		}
		int i2 = index + 1;
		i2 = i2 >= 3? 0 : i2;
		finalPerform = (performance[index] + performance[i2]) /2;
		pPendingParam->perform = finalPerform;
		pPendingParam->evaluated = true;
		timesTested = 0;
		//========Recording something....==============
		double recData[20];
		int k = 0;
		for(k = 0; k < currentParam.getDim(); ++k)
			recData[k] = bestParam.genes[k];
		recData[k++] = bestParam.perform;
		for(int j = 0; j < pPendingParam->getDim(); ++j)
			recData[k++] = pPendingParam->genes[j];
		recData[k++] = pPendingParam->perform;
		//MATLABHELP_SAVE_VECTOR(recData, k, "PGMLearnerRec");
	}else{
		return false;
	}

	bool allTested = true;
	for(int i = 0 ; i < R.size(); ++i)
	{
		if(R[i].evaluated == false)
		{
			pPendingParam = &R[i];
			allTested = false;
			break;
		}
	}
	return allTested;
}
