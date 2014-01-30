#include "Learner.h"
#include <cstring>
Learner::Param::Param(int dim_):dim(dim_), perform(0), evaluated(false), genes(0)
{
	genes = new double[dim];
}

Learner::Param::Param(const Learner::Param& other)
{
	this->dim = other.dim; this->perform = other.perform; this->evaluated = other.evaluated;
	this->genes = new double[this->dim];
	memcpy(this->genes, other.genes, this->dim*sizeof(double));
}
Learner::Param::~Param()
{
	if(genes){delete genes; genes = 0;}
}
Learner::Param& Learner::Param::operator=(const Learner::Param& other)
{
	this->dim = other.dim; this->perform = other.perform; this->evaluated = other.evaluated;
	this->genes = new double[this->dim];
	memcpy(this->genes, other.genes, this->dim*sizeof(double));
	return *this;
}
Learner::Param Learner::Param::operator+ (const Learner::Param& other)
{
	Param tmp(*this);
	for(int i = 0; i < this->dim && i < other.dim; ++i)
	{
		tmp.genes[i] = this->genes[i] + other.genes[i];
	}
	return tmp;
}
Learner::Param Learner::Param::operator* (double factor)
{
	Param tmp(*this);
	for(int i = 0; i < this->dim; ++i)
	{
		tmp.genes[i] = this->genes[i] * factor;
	}
	return tmp;
}
Learner::Param Learner::Param::operator/ (double factor)
{
	Param tmp(*this);
	for(int i = 0; i < this->dim; ++i)
	{
		tmp.genes[i] = this->genes[i] / factor;
	}
	return tmp;
}