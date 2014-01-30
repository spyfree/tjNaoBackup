#ifndef __Learner_h_
#define __Learner_h_
class Learner
{
public:
	class Param
	{
	public:
		Param(int dim_);
		Param(const Param& other);
		~Param();
		int getDim() const { return dim; }
		Param operator+ (const Param& other);
		Param operator* (double factor);
		Param operator/ (double factor);
		Param& operator=(const Param& other);
		double* genes;
		double perform;
		bool evaluated;
	private:
		int dim;
	};
public:
	Learner(const Param& initParam_) : paramDim(initParam_.getDim()), initialParam(initParam_) {}
	~Learner() {}
public:
	virtual void updatePerform(double perform) = 0;
	virtual bool canStopLearning() = 0;
	virtual const Param& getPendingParam() const = 0;
	virtual const Param& getBestParam() const = 0;
protected:
	Param initialParam;
	int paramDim; /**< dim of params*/
};

#endif