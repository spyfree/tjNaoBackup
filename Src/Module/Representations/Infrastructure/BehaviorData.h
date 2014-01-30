#ifndef BehaviorData_h_DEFINED
#define BehaviorData_h_DEFINED
enum WalkTypes
{
	undefineWalktype
};
class BehaviorData
{
public:
	BehaviorData(){}
	
};

class WalkingParameters 
{
public:
	WalkingParameters():left(0),forward(0),turnCCW(0),walkType(undefineWalktype){}
	WalkingParameters(double left1,double forword1,double turnCCW1,WalkTypes walkType1)
	{
		left = left1; 
		forward = forword1; 
		turnCCW = turnCCW1;
		walkType = walkType1;
	}

	/* setters */
	void setLeft(double l){ left = l;}
	void setForward(double f){ forward = f;}
	void setTurnCCW(double t){ turnCCW = t;}
	void setWalkType(WalkTypes w){ walkType = w;}

	/*getters */
	const double getLeft() const { return left;}
	const double getForward() const { return forward;}
	const double getTurnCCW() const { return turnCCW;}
	const double getWalkType() const { return walkType;}

private:
	double left,forward,turnCCW;
	WalkTypes walkType;
};

class OutToNetwork 
	{
	public:
		OutToNetwork():value(0){}
		OutToNetwork(double value1)
		{
			value = value1;
		}

		/*setters */
		void setValue(double v){ value = v;}

		/*getters */
		const double getValue() const { return value;}

	private:
		double value;
	};

#endif