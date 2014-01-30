#ifndef __ScanVision__
#define __ScanVision__
#include "Module/Vision/DebugDrawings.h"
#include "Module/Vision/TJImage.h"
#include "Module/Common/Geometry.h"
#include "Module/Vision/BallSpecialist.h"
#include "Module/Vision/GoalSpecialist.h"
#include "Module/Vision/Run.h"
#include <vector>
#include "Module/Representations/Perception/BlobPercept.h"
#include "Module/Representations/Perception/BallSpot.h"
#include "Module/Vision/LineSpecialist.h"
#include "Module/Vision/Regionpercept.h"
#include "Tools/TJAssign.h"
#ifndef sgn
#define sgn(a)   ( (a) < 0 ? -1 : ((a)==0) ? 0 : 1 )
#endif
//#define maxNumberOfLinePoints 80
class ScanVision
{
public:
	ScanVision(const TJImage& imagetj, const CameraMatrix& cameraMatrix);
	virtual ~ScanVision();
public:
    Regionpercept RegionRun;
	BlobPercept *blobPercept; /**< internal pointer to the RegionPercept */
	int maxBallSegmentLength; /**< The maximum length of a ball segment found so far. */

	inline BlobPercept::Segment* addSegment(int x,int y,int length,unsigned char color/*,int yEnd*/)
	{
		if (!(RegionRun.segmentsCounter<MAX_SEGMENTS_COUNT))
			return NULL;
		BlobPercept::Segment* seg =RegionRun.segments + RegionRun.segmentsCounter++;
		seg->color=color;
		seg->x=x;
		seg->y=y;
		seg->explored_min_y = y;
		seg->explored_max_y = y+length;
		seg->explored_size = 0;
		seg->length = length;
		seg->blob = NULL;
		seg->link = NULL;

		return seg;
	}
	int runUp(int x, int y, unsigned char col, int yEnd);
	int runDown(int x, int y, unsigned char col, int yEnd);
	int runRight(int x, int y, unsigned char col, int xMax);
	int findDown2(int x, int y, unsigned char col1, unsigned char col2, int yEnd, unsigned char& foundCol);
	int findDown(int x, int y, unsigned char col, int yEnd);
	int explorePoint(int x, int y, unsigned char col, int xMin, int yEnd, int yMin, int& runEnd, int& explored_min_y, int& explored_max_y, bool force_detailed=false);

	int scanVertically();

	void scanHorizontally();

	void calcBorders(int xStart, int xEnd);
	std::vector<Vector2<int> > getConvexFieldBorders(std::vector<Vector2<int> >& fieldBorders);

// 	int getHorizonStartY(const Geometry::GTLine& horizon );
	void proccess();
	void ResetScanVision();
	void setNewHorizon(const Geometry::GTLine& newHorizon);
public:
	/**
	* Default constructor.
	*/
	unsigned char teamcolour[2];
	std::vector<Vector2<int> > fieldBorders;
	bool HorizenInImage;
	Vector2<int> horizonStart;
	Vector2<int> horizonEnd;
	//VisionInterface TJVisionInterface;

private:
	int yHorizon;
	/** HorizonLine in ScanVision */
	Geometry::GTLine HorizonLine;
	bool HorizonExist;
	/**
	* VerticalLine, same base with Horizonline;
	*/
	Geometry::GTLine VertLine;
	const CameraMatrix& ScanvisioncameraMatrix;
    const TJImage& image;
	/**
	* The function converts an address to pixel coordinates.
	* @param p An address in image.image.
	* @return The x- and y-coordinates of the corresponding pixel.
	*/
	Vector2<int> getCoords(const unsigned char* p) const
	{
		const int diff((p - &image.unCorrectedRaw[0][0][0])/3);
		return Vector2<int>(diff %IMAGE_WIDTH, diff /IMAGE_WIDTH );
	}
public:
	class Parameters:public paramAssign
	{
	public:
		void massAssign();
	public:
		int gridStepSize;
		int exploreStepSize;
		int borderMinGreen;
		int minBorder;
		int skipOffset;
		int pixelsBelowHorizon;
		int verticalgridStepsize;
	};
	Parameters params;
};
#endif // __ScanVision__
