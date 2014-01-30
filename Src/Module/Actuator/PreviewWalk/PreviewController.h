#ifndef __PreviewController_h_
#define __PreviewController_h_
#include "Tools/Math/Matrix.h"
#include "PreBufferGenerator.h"
class PreviewController
{
public:
	PreviewController();
	~PreviewController();
	/**
	* Load Gain from Config File
	*/
	bool loadGainFile();
	/**
	* Reset Coefficient
	*/
	void resetCoefficient(double PG, double Zc, double dt, double g);
	unsigned int getNumPreviewFrames(){return numPreviewFrames;}
	/**
	* Reset initial state for new control
	* @param stateX0 [x, vx, ax] of x direction of COM
	* @param stateY0 [y, vy, ay] of y direction of COM
	*/
	void resetController(const Vector3<double>& stateX0, const Vector3<double>& stateY0);
	Vector3<double> getRefCOM(PreviewBuffer& preBuffer, const PreviewWalkParam& walkParam);
private:
	void setCoefficientFromTable(double Zc_, double dt_ = 0.04);
	void setDefaultCoefficient();
private:
	static const unsigned int MAX_PREVIEW_FRAMES = 80;
	//static const unsigned int Num_Preview_Frames = 40;
private:
	Matrix3x3<double> A;	/**< 3 by 3*/
	Vector3<double> b;		/**< 3by 1*/
	Vector3<double> c;		/**< 1 by 3*/
	double Q;
	double R;
//	Matrix3x3<double> P;	/**< 3 by 3*/
	Vector3<double> K;		/**< 1 by 3*/
	double F[MAX_PREVIEW_FRAMES];
	int numPreviewFrames;
private:
	class GainTable
	{
	public:
		GainTable();
		bool load(const std::string& filename);
	public:
		static const int MAX_TABLE_PREVIEW_FRAMES = 80;
		static const int MAX_TABLE_NUM = 30;
		double FTable[MAX_TABLE_NUM][MAX_TABLE_PREVIEW_FRAMES];
		double KTable[MAX_TABLE_NUM][3];
		double ZcTable[MAX_TABLE_NUM];
		double nLen;	/**< Length of the gain table*/
		double nFLen;	/**< Length of F gain in the table*/
		double dt;
		double g;
		bool valid;
	};
	GainTable gainTable;
	///=========Temporary buffer and vectors====
	Vector3<double> VecX, VecY;
};
#endif