#ifndef __SPController_h_
#define __SPController_h_
#include "Tools/Math/Vector3.h"
class SPController
{
public:
	class Gain
	{
	public:
		Gain():G(0) {}
		Gain(const Vector3<double>& k, double g):K(k), G(g) {}
		Vector3<double> K;
		double G;
	};
	SPController(){}
	void addZMPError(const Vector3<double>& eP)
	{
		sumZMPError += eP;
	}
	void resetZMPError(){sumZMPError = Vector3<double>(0,0,0);}
	Vector3<double> getOutput(const Vector3<double>& x, const Vector3<double>& y)
	{
		return Vector3<double>(gainx.K*x + gainx.G*sumZMPError.x, gainy.K*y + gainy.G*sumZMPError.y, 0);
	}
	const Vector3<double>& getSumZMPError() const { return sumZMPError; }
	void setGainX(const Gain& otherGain)
	{
		gainx.K = otherGain.K;
		gainx.G = otherGain.G;
	}
	void setGainY(const Gain& otherGain)
	{
		gainy.K = otherGain.K;
		gainy.G = otherGain.G;
	}
	const Gain& getGainX() const{ return gainx; }
	const Gain& getGainY() const{ return gainy; }
private:
	Gain gainx, gainy;
	Vector3<double> sumZMPError;
};
#endif