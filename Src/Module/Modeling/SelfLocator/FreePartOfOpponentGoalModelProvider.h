/************************************************************************/
/* The Ks that give the free part of opponent goal
Author:laura
Time:2011.5.2                                                           */
/************************************************************************/
#ifndef FreePartOfOpponentGoalModelProvider_H
#define FreePartOfOpponentGoalModelProvider_H
#include "Module/Vision/DebugDrawings.h"
#include "Module/Representations/Modeling/BallModel.h"
#include "Module/Common/Geometry.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/Blackboard.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/RobotPercept.h"
#include "Module/Representations/Configuration/FieldDimensions.h"
#include "Module/Representations/Modeling/RobotPose.h"
#include "Module/Representations/Modeling/FreePartOfOpponentGoalModel.h"
#include "Module/Modeling/SelfLocator/SampleTransferredToPc.h"

DEFINE_KS(FreePartOfOpponentGoalProvider)
REQUIRES(RobotPercept)
REQUIRES(CameraMatrix)
REQUIRES(RobotPose)
REQUIRES(BallModel)
REQUIRES(FieldDimensions)
PROVIDES(FreePartOfOpponentGoalModel)
PROVIDES(FreePartData)
END_KS

/** if a robot occupies a cell on the opponent groundline this cells value will get increased by this */
static const double RENEW_VALUE = 1.0;
/** max value a cell on the opponent groundline can have */
static const double MAX_VALUE = 255.0;
/** every iteration all cells on on the opponent groundline will get multiplied by this */
static const double AGING_FACTOR = 0.85;
/** a cell on the opponent groundline is free if its value i lower than this */
static const double IS_FREE_THRESHOLD = 0.2;
/** robot width in mm */
static const double ROBOT_WIDTH = 200.0;
/** a free part must at least consist of this many cells */
static const int   MIN_LARGEST_SIZE = 2;
/** if already a free part was found the next one has to be at least this many cells bigger */
static const int   NEW_MAX_FREE_THRESHOLD = 2;




class FreePartOfOpponentGoalProvider:public FreePartOfOpponentGoalProviderBase,BaseProvider
{
public:
	FreePartOfOpponentGoalProvider()
	{
		for(int i = 0; i < NUM_OF_CELLS; i++)
		{
			cellsOnOppGoalline[i] = 0.0f;
		}
		lastFreePartHigh = NUM_OF_CELLS;
		lastFreePartLow = 0;
	}
		~FreePartOfOpponentGoalProvider(){}

	void inRobotPose(){}
	void inCameraMatrix(){}
	void inRobotPercept(){}
	void inFieldDimensions(){}
	void inBallModel(){}
	void update(FreePartOfOpponentGoalModel* theFreePartOfOpponentGoalModel);
	void update(FreePartData*theFreePartData);
	void outFreePartData(){}
	void outFreePartOfOpponentGoalModel(){}
	Vector2<double> FieldCoord2relative(const RobotPose& rp, const Vector2<double>& FieldCoord);
   inline Vector2<double> relative2FieldCoord(const RobotPose& rp,const Vector2<double>& realcoordOnField);
   void drawingOnField(const FreePartOfOpponentGoalModel& freePartOfOpponentGoalModel);
private:
	/** goal line of opponent goal will be divided into this many cell */
	static const int NUM_OF_CELLS = 14;

	/** this array holds values (likeliness a robot is standing there) for each cell on the opponent goal */
	double cellsOnOppGoalline[NUM_OF_CELLS];

	/** highest (left side when facing the opponent goal) cell of the largest free part of opponent goal */
	int largestFreePartHigh;

	/** lowest (right side when facing the opponent goal) cell of the largest free part of opponent goal */
	int largestFreePartLow;

	/** high cell of largest free part found in last iteration */
	int lastFreePartHigh;

	/** low cell of largest free part found in last iteration */
	int lastFreePartLow;

};

#endif






