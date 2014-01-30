#ifndef __Regionpercept_h_
#define __Regionpercept_h_

#include "Tools/Math/Vector2.h"
#include "Tools/Math/Vector3.h"
#include"Tools/Math/Common.h"
//#include "Module/common/VisionHeader.h"
#include <vector>
#include "Module/Vision/Run.h"
#include "Module/Representations/Perception/BallSpot.h"
#include "Module/Representations/Perception/BlobPercept.h"
#include "Module/Common/Geometry.h"
#include "Module/Vision/LinePoint.h"
class Regionpercept
{
public:
	//std::vector<LinePoint> linePoints;
	//int numberOfLinePoints;
	BlobPercept::Segment segments[MAX_SEGMENTS_COUNT]; /**< This array stores all the segments. */

	int segmentsCounter;/**< A counter for the segments. This counter always points to index after
						the last segment in segments. */
	std::vector<Run> VerticalRun;
	std::vector<Run> HorizontalRun;
	//std::vector<Run> PinkGoalKeeperRun;
	//std::vector<Run> BlueGoalKeeperRun;
	Geometry::GTLine horizonline;
	Geometry::GTLine verticaline;
	Vector2<int> horizonStart;
	Vector2<int> horizonEnd;
	bool horizonImage;

	Regionpercept()
	{
		//segments.reserve(1000);
		VerticalRun.reserve(100);
		HorizontalRun.reserve(200);
	/*    PinkGoalKeeperRun.reserve(50);
        BlueGoalKeeperRun.reserve(50);*/
	}
};
#endif