#ifndef _LineSpecialist_H_
#define _LineSpecialist_H_

#include "Module/Vision/TJImage.h"
#include "Tools/Math/Vector2.h"
#include "Module/Common/Geometry.h"
#include "Module/Vision/DebugDrawings.h"
#include "Module/Vision/Regionpercept.h"
#include "Module/Representations/KS.h"
#include "Module/Representations/BaseProvider.h"
#include "Module/Representations/Perception/CameraMatrix.h"
#include "Module/Representations/Perception/LinesPercept.h"
#include "Module/Representations/Perception/BlobPercept.h"
#include "Module/Representations/Perception/BallSpots.h"
#include <vector>
#include "Module/Vision/LineElements.h"
#include "Tools/TJAssign.h"

#define TOTALE_GRUETZE 42345645

DEFINE_KS(LineSpecialist)
REQUIRES(TJImage)
REQUIRES(CameraMatrix)
REQUIRES(Regionpercept)
PROVIDES(BallSpots)
PROVIDES(LinesPercept)
PROVIDES(BlobPercept)
USES(LinesPercept)
END_KS

class LineSpecialist:public LineSpecialistBase,BaseProvider
{
public:
	LineSpecialist(void);
	~LineSpecialist(void);

	void update(LinesPercept* theLinesPercept);
	void update(BallSpots* theBallSpots);
	void update(BlobPercept*theBlobPercept);
	void inTJImage(){}
	void inCameraConfig(){}
	void inRegionpercept(){}
	void inCameraMatrix(){}
	void inFrameInfo(){}
	void outBallSpots(){}
	void outLinesPercept();
	void outBlobPercept(){}
public:
	//Regionpercept regionPercept;
	BlobPercept::Blob blobs[MAX_REGIONS_COUNT]; /**< This array stores all the regions. */
	int blobsCounter;  /**< A counter the the regions. This counter always points to the index after
				   the last region in regions. */
	std::vector<double>lineWidthOnField;
	std::list<LineElements::LineElement> lineElements; /**< All the line spots */
	std::vector<LineElements::NonLineElement> nonLineElements; /**< All the non line spots (= white regions which are no lines)*/
public:
	  bool uniteRegions(BlobPercept::Segment* seg1, BlobPercept::Segment* seg2);
	  inline bool createNewRegionForSegment(BlobPercept::Segment* seg);
	  inline BlobPercept::Segment* connectToRegions(BlobPercept::Segment* newSegment, BlobPercept::Segment* lastColumPointer, int xDiff);
	  void buildRegions();
	  void analyzeRegions(BallSpots& ballSpots);
	  inline bool isLine(const BlobPercept::Blob* blob, float& direction, LineElements::LineElement& lineElement);
	  /** Returns how much green is below the region */
	  int getGreenBelow(const BlobPercept::Blob* blob);
	  /** Returns how much green is above the region */
	  int getGreenAbove(const BlobPercept::Blob* blob);
	  /** Returns how much green is left to the region */
	  int getGreenLeft(const BlobPercept::Blob *blob);
	  /** Returns how much green is right to the region */
	  int getGreenRight(const BlobPercept::Blob *blob);
	  /** create BanSectors */
	  void createBanSectors(const CameraMatrix& cameraMatrix);

	  /** 
	  * creates the linesegments from the lineSpots, it creates the BanSector and filters out lines which meet
	  * the banSector filter criterions
	  * @param singleSegs a reference to the singleSegs list in the LinePercept
	  * */
	  void createLineSegments(std::list<LinesPercept::LineSegment>& singleSegs,const CameraMatrix& cameraMatrix);

	  /** 
	  * creates the lines from the singleSegments 
	  * @param lines a reference to the lines list in the LinePercept
	  * @param singleSegs a reference to the singleSegs list in the LinePercept
	  * */
	  void createLines(std::list<LinesPercept::LineOnField>& lines, std::list<LinesPercept::LineSegment>& singleSegs);

	  /** 
	  * analyzes the lines, merges and deletes some if neccessary and creates intersections 
	  * @param lines a reference to the lines list in the LinePercept
	  * @param intersections a reference to the intersections vector in the LinePercept
	  * @param circle a reference to the circle in the LinePercept
	  * @param singleSegs a reference to the singleSegs list in the LinePercept
	  * */
	  void analyzeLines(std::list<LinesPercept::LineOnField>& lines, std::vector<LinesPercept::Intersection>& intersections, LinesPercept::CircleSpot& circle, std::list<LinesPercept::LineSegment>& singleSegs);

	  /**
	  * analyze the singleSegments and try to find the center circle 
	  * @param singleSegs a reference to the singleSegs list in the LinePercept
	  * @param circle a reference to the circle in the LinePercept
	  * @param lines a reference to the lines list in the LinePercept
	  * */
	  void analyzeSingleSegments(std::list<LinesPercept::LineSegment>& singleSegs, LinesPercept::CircleSpot& circle, std::list<LinesPercept::LineOnField>& lines);

	  /** 
	  * Determines the start and end point of a line. If updateLine == true the d and alpha values of the line 
	  * (Hess norm form) are recalculated.
	  * @param line the line to determine the start and end point of
	  * @param first returns the start/end point of the line
	  * @param last returns the start/end point of the line
	  * @param updateLine recalculate d and alpha of line (Hess norm form)?
	  * */
	  void getFirstAndLastOfLine(LinesPercept::LineOnField& line, Vector2<int>& first, Vector2<int>& last, bool updateLine = true);
      void sendCircleResult();
public:
	class BanSector
	{
	public:
		int start; /**< Distance the Sector starts */
		int end; /**<Distance the sector ends */
		double alphaLeft; /**< left angle of the sector */
		double alphaRight; /**< right angle of the sector */
		int counter; /**< Number of nonLineSpots in this sector */
	};
public:
	std::list<LinesPercept::LineSegment> lineSegs; /**< All the lineSegments */
	std::list<BanSector> banSectors; /**< The ban sectors, where no vertical, long spots are accepted */
public:
	class Parameters:public paramAssign
	{
	public:
		void massAssign();
	protected:
	public:
		int maxLineNghbGreySkip;
		int minLineSize;
		int minLineSegmentCount;
		int minLineSingleSegmentSize;
		int maxLineNghbNoneSize;
		double maxLineNghbNoneRatio;
		int minLineNghbGreenSideSize;
		int minLineNghbGreenAboveSize;
		int minLineNghbGreenBelowSize;
		int minRobotRegionSize;
		double maxRobotRegionAlphaDiff;
		int minRobotWidthRatio;
		double angleStepSize;
		int minSectorCounter;
		double minWidthRatio;
		int maxLineDistance;
		double maxAlphaDiff;
		int minLineLength;
		double maxLineAngleDiff;
		int maxDDiff;
		int minSupporters;
		int minLineStartLength;
		int circleBiggerThanSpecified;
		int minSegmentLength;
		int minSegmentImgLength;
		int maxNgbhDist;
		int centerCircleRadius;
		int maxRadiusError;
		int maxSupporterDist;
		int maxSupporterDist2;
		int minLineSingleRegionLength;
		int maxLineCircleDist;
		int maxLineUniteDist;
		double maxLineUniteAlphaDiff;
		int maxLineSingleSegDist;
		int maxLineSingleSegDist2;
		double minHardcover;
		double minIntersectionAlphaDiff;
		int minIntersectionLength;
		int minTToEnd;
		int maxTFromEnd;
		int maxMidLineToCircleDist;
		int minMidLineLength;
		double minEccentricityOfBallRegion;
		double maxAngleDiff;
		int regionMaxSize;
		int gridStepSize;
		int skipOffset;
		int minCircleSupporters;
		int maxLineWidth;
	};
	Parameters params;
};



#endif