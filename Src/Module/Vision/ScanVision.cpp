#include "ScanVision.h"
#include<iostream>
#include "Platform/SystemCall.h"
#include "Platform/GetSetting.h"
#include <fstream>
#include <string>

const int openscanline=true;
const int openFieldBoard=false;

//minSegSize is the min size of each color run
int minSegSize[10]={9999,//BLACK =0
					4,//	FIELD=1
					2,//WHITE=2
					1,//ORANGE=3
					1,//Robot RED=4
                   9999,//blue=5
				   9999,//GRAY=6
					1,//Robot Cyan=7
					9999,//PINK = 8,
			        9999};//YELLOW = 9,

#define IS_LEFT(x0, x1, x2) ((x1.x-x0.x)*(x2.y-x0.y)-(x2.x-x0.x)*(x1.y-x0.y) > 0)
ScanVision::ScanVision(const TJImage& imagetj, const CameraMatrix& cameraMatrix):
image(imagetj),ScanvisioncameraMatrix(cameraMatrix)
{
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/scanVisionConfig.cfg";
	//paramAssign::param paramMap;
	params.readCfgFile(path);
	params.massAssign();
	//params.paramReset();
	fieldBorders.reserve(64);
}


ScanVision::~ScanVision()
{

}
/************************************/
//scanVertically()检测物体主要有:门横梁、粉色守门员，蓝色守门员，白线和球以及其他可能出现的 场上的颜色
/************************************/

int ScanVision::scanVertically()
{
	yHorizon = HorizonLine.base.y;
	//int xStart = minBorder + ((IMAGE_WIDTH - 2 * minBorder - 1) % gridStepSize) / 2,
	int xStart = params.verticalgridStepsize-1 + ((IMAGE_WIDTH) %  params.verticalgridStepsize) / 2,
		xEnd =IMAGE_WIDTH,
		yEnd=IMAGE_HEIGHT-1;//because YUV of  last line point is not correct
	if(openFieldBoard)
	{
		//calcBorders(xStart,xEnd);
		for (std::vector<Vector2<int> >::iterator iter=fieldBorders.begin();iter!=fieldBorders.end();iter++)
		{
			std::vector<Vector2<int> >::iterator it=iter;
			it++;
			if (it!=fieldBorders.end())
			{
				DRAW_LINE(iter->x,iter->y,it->x,it->y,1,Drawings::ps_DashDotDotLine,Drawings::black);
			}
			else
				break;
		}
	}
	int yStart = max(0, yHorizon - params.pixelsBelowHorizon);
	bool perceptFull = false;//perceptFull = true means that the run is too much ,and the vector is full
	unsigned char curColor;
	BlobPercept::Segment *newSegment;
	int run_end_y, explored_min_y, explored_max_y, explored_size;
	for(int x = xStart; x < xEnd&&!perceptFull; x+= params.verticalgridStepsize)
	{
		if (openscanline)
		{
			DRAW_LINE(x,yStart,x,yEnd,1,Drawings::ps_SolidLine,Drawings::gray);
		}
		//scan for goal segments above horizon
		const int tyEnd = yHorizon < yEnd ? yHorizon : yEnd;
		int y=0;
		while(y < yHorizon && y < yEnd)
		{
			y = findDown2(x, y, CYAN,YELLOW, tyEnd, curColor);
			if(y >= yEnd)
				break;

			if(y < yHorizon)//找到门横梁
			{
				Run verticleRun;
				verticleRun.start.x=x;
				verticleRun.start.y=y;
				verticleRun.length=runDown(x,y, curColor, tyEnd) - y;// //TODO:?????is it need to scan until yEnd(IMAGEHEIGHT-1)?WHY NOT tyEnd?the cross bar can not be under horizon line even the horizon line is calculated wrong
				verticleRun.colour=curColor;
				verticleRun.end.x=x;
				verticleRun.end.y=y+verticleRun.length;
				RegionRun.VerticalRun.push_back(verticleRun);
				y += verticleRun.length;
			}
		}
		//scan the object below the horizon
		y=yStart;
		while(y < yEnd)
		{
			curColor = image.getClassifyedColor(x,y);
			explored_size = explorePoint(x, y, curColor, max(0, x - params.verticalgridStepsize), yEnd, yStart, run_end_y, explored_min_y, explored_max_y);//max(0, x - params.gridStepSize) make the run stored can not be duplicated

			if(run_end_y - y >=minSegSize[curColor])
			{
				
			   newSegment = addSegment(x, y, run_end_y - y, curColor/*, yEnd*/);
			
				if(!newSegment) //MAX_SEGMENTS_COUNT//当segmentsCounter达到MAX_SEGMENTS_COUNT时，将不再存储segments
				{
					if(RegionRun.segmentsCounter == MAX_SEGMENTS_COUNT)
					{
						perceptFull = true;
						break;
					}
				}

				newSegment->explored_min_y = explored_min_y;
				newSegment->explored_max_y = explored_max_y;
				newSegment->explored_size = explored_size;
	
			}
			y = run_end_y;
		}
	}
	return yEnd;
}
// int ScanVision::getHorizonStartY(const Geometry::GTLine& horizon )
// {
// 	Vector2<int> horizonstart,horizonend;
// 	bool horizoninimage;
// 	horizoninimage = Geometry::getIntersectionPointsOfLineAndRectangle(
// 		Vector2<int>(0,0),Vector2<int>(IMAGE_WIDTH-1,IMAGE_HEIGHT-1), horizon,
// 		horizonstart, horizonend);
// 	return horizonstart.y;
// }

void ScanVision::setNewHorizon(const Geometry::GTLine& newHorizon){
	HorizonLine = newHorizon;
	HorizonExist = true;
	VertLine = HorizonLine;
	VertLine.direction.x = - HorizonLine.direction.y;
	VertLine.direction.y = HorizonLine.direction.x;
	RegionRun.horizonline=HorizonLine;
	RegionRun.verticaline=VertLine;


	HorizenInImage = Geometry::getIntersectionPointsOfLineAndRectangle(
		Vector2<int>(0,0),Vector2<int>(IMAGE_WIDTH-1,IMAGE_HEIGHT-1), HorizonLine,
		horizonStart, horizonEnd);
	RegionRun.horizonStart.x=horizonStart.x;//生成球门需要用的水平线
	RegionRun.horizonStart.y=horizonStart.y;
	RegionRun.horizonEnd.x=horizonEnd.x;
	RegionRun.horizonEnd.y=horizonEnd.y;
	RegionRun.horizonImage=HorizenInImage;

	if(HorizenInImage)
		DRAW_LINE(horizonStart.x, horizonStart.y, horizonEnd.x, horizonEnd.y,3,Drawings::ps_DashLine, Drawings::red);
}


void ScanVision::ResetScanVision(){
	HorizonExist = false;
	HorizenInImage=false;
	maxBallSegmentLength = 5;
	//RegionRun.YellowHorizontalRun.clear();
	//RegionRun.CyanHorizontalRun.clear();
	RegionRun.HorizontalRun.clear();
	RegionRun.VerticalRun.clear();
	RegionRun.segmentsCounter=0;
	OBJECT_TO_DRAW_CLEAR();
}

void ScanVision::proccess()
{
	//lineSpecialist.reset();
	//TJVisionInterface.reset(image.frameNumber);
	unsigned processtime=SystemCall::getCurrentSystemTime();
	int yEnd = scanVertically();
	scanHorizontally();
	unsigned processtimeused=SystemCall::getTimeSince(processtime);
	//std::cout<<"sanvision process time--:"<<processtimeused<<" ms"<<std::endl;
}
/************************************/
//scanHorizontally()检测物体主要有:门
/************************************/
void ScanVision::scanHorizontally()
{
	int xStart = 0,
		xEnd = IMAGE_WIDTH,
		yStart = 0,
		yEnd = IMAGE_HEIGHT-1;//because YUV of  last line point is not correct

	int yHorizon = HorizonLine.base.y;

	int x = xStart,
		y = yStart;

	unsigned char curColor;
	//int stepSize = 2;//delete by wwj,replace it with params.skipOffset
	bool foundInLastLine = true;//foundInLastLine = true means that if find goal run above horizon,Go On to scan goal run under horizon with increase the y coordination until there is no goal run.
							//it is good for collect more runs near the horizonline

	while((y <= yHorizon || foundInLastLine) && y < yEnd)
	{
		foundInLastLine = false;
		x = xStart;
		if (openscanline)
		{
			DRAW_LINE(x,y,xEnd,y,1,Drawings::ps_SolidLine,Drawings::gray);
		}
		while(x < xEnd)
		{
			while(x < xEnd)
			{
				curColor = image.getClassifyedColor(x,y);
				if(curColor == CYAN || curColor == YELLOW||curColor == RED)
					break;
				x += params.skipOffset;
				// TODO: params.skipOffset can adjust bigger  ,now is 2,is not efficient
			}
			if(x < xEnd)
			{
				int i;
				for(i = 1; i < params.skipOffset && x-i > 0; i++)
				{
					if(image.getClassifyedColor(x-i,y) != curColor)
						break;
				}
				x -= (i-1);

				if (curColor==CYAN)
				{
					Run temp1;
					temp1.start.x=x;
					temp1.start.y=y;
					temp1.end.x=runRight(x+i-1, y, curColor, xEnd);//change from runRight(x, y, curColor, xEnd);because if the params.skipOffset  is large ,it is no need to scan the point (x) to the point (x+ i-1); because these point has check as the goal point in the loop for(i = 1; i < params.skipOffset && x-i > 0; i++)
					temp1.end.y=y;
					temp1.length=temp1.end.x-temp1.start.x;
					temp1.colour=CYAN;
					x+=temp1.length;
					if(temp1.length>=3)
					  RegionRun.HorizontalRun.push_back(temp1);

				}
				else if(curColor==YELLOW)
				{
					Run temp2;
					temp2.start.x=x;
					temp2.start.y=y;
					temp2.end.x=runRight(x+i-1, y, curColor, xEnd);//change from runRight(x, y, curColor, xEnd);because if the params.skipOffset  is large ,it is no need to scan the point (x) to the point (x+ i-1); because these point has check as the goal point in the loop for(i = 1; i < params.skipOffset && x-i > 0; i++)
					temp2.end.y=y;
					temp2.length=temp2.end.x-temp2.start.x;
					temp2.colour=YELLOW;
					x+=temp2.length;
					if(temp2.length>=3)
					RegionRun.HorizontalRun.push_back(temp2);
					DRAW_LINE(temp2.start.x,temp2.start.y,temp2.end.x,temp2.end.y,2,Drawings::ps_SolidLine,Drawings::darkBlue);
				}
				else if(curColor==RED)
				{
					Run temp3;
					temp3.start.x=x;
					temp3.start.y=y;
					temp3.end.x=runRight(x+i-1, y, curColor, xEnd);//change from runRight(x, y, curColor, xEnd);because if the params.skipOffset  is large ,it is no need to scan the point (x) to the point (x+ i-1); because these point has check as the goal point in the loop for(i = 1; i < params.skipOffset && x-i > 0; i++)
					temp3.end.y=y;
					temp3.length=temp3.end.x-temp3.start.x;
					temp3.colour=RED;
					x+=temp3.length;
					if(temp3.length>=3)
						RegionRun.HorizontalRun.push_back(temp3);
					//DRAW_LINE(temp3.start.x,temp3.start.y,temp3.end.x,temp3.end.y,2,Drawings::ps_SolidLine,Drawings::darkBlue);
				}

				//x += seg.length;
				foundInLastLine = true;
			}
		}
		y += params.gridStepSize;
	}
}
/****************************************************************
 Run to the right from (x,y) unless there is a run of skipOffset pixels with color != col.
 *****************************************************************/
int ScanVision::runRight(int x, int y, unsigned char col, int xMax)
{
	const int xStart = x;
	int tmp;

	for(x += params.skipOffset; x < xMax; x += params.skipOffset)
	{
		if(image.getClassifyedColor(x,y) != col)
		{
			tmp = x - params.skipOffset;
			for(--x; x > tmp; x--)
				if(image.getClassifyedColor(x,y) == col)
					break;

			if(x == tmp)
			{
				x++;
				break;
			}
		}
	}
	if(x > xMax)
		x = xMax;
	return x;
}

/*************************************
Explore a pixel: if col == green and force_detailed == false this is a normal runDown call
   *                  and additionally the size and explored_min_y (==y) and explored_max_y (==runEnd)
   *                  are returned.
   *                  Otherwise it will do a runDown and afterwards look from x to xMin(last scanline)
   *                  whether there are runs connected to the "runDown-run" which start before or end
   *                  after the "runDown-run". These y-coordinates are stored in explored_min_y and
   *                  explored_max_y.
   *                  The size returned is the length of the run multiplied by gridStepSize.
   **************************************/
int ScanVision::explorePoint(int x, int y, unsigned char col, int xMin, int yEnd, int yMin, int& run_end, int& explored_min_y, int& explored_max_y, bool force_detailed)
{
	int size = 0;
	if(col != FIELD || force_detailed)
	{
		run_end = runDown(x, y, col, yEnd);
		explored_min_y = y;
		explored_max_y = run_end-1;

		size = (run_end - y) * params.verticalgridStepsize;//the area of the blob,is calculat like that: length * width

		if(!(run_end - y >= minSegSize[col]))
			return -1;

		for(x -= params.exploreStepSize; x > xMin; x -= params.exploreStepSize)
		{
			if(image.getClassifyedColor(x,explored_min_y) == col)
			{
				const int expl_run_end = runUp(x,explored_min_y, col, yMin);
				if(expl_run_end < explored_min_y)
					explored_min_y = expl_run_end+1;
			}
			if(image.getClassifyedColor(x,explored_max_y) == col)
			{
				const int expl_run_end = runDown(x, explored_max_y, col, yEnd);
				if(expl_run_end > explored_max_y)
					explored_max_y = expl_run_end-1;
			}
		}
	}
	else
	{
		run_end = runDown(x, y, col, yEnd);
		size = (run_end - y) * params.verticalgridStepsize;
		explored_min_y = y;
		explored_max_y = run_end;
	}
	return size;
}
/*************************************************************
Run down from (x,y) unless there is a run of skipOffset pixels with color != col.
**************************************************************/
int ScanVision::runDown(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y+=params.skipOffset; y < yEnd; y+=params.skipOffset)
	{
		if(image.getClassifyedColor(x,y) != col)
		{
			tmp = y - params.skipOffset;
			for(--y; y > tmp; y--)
				if(image.getClassifyedColor(x,y) == col)
					break;

			if(y == tmp)
			{
				y++;
				break;
			}
		}
	}
	if(y > yEnd)
		y = yEnd;
	return y;
}
/*********************************************
Run down from (x,y) unless there is a run of skipOffset pixels with the color col1 or col2.
**********************************************/
int ScanVision::findDown2(int x, int y, unsigned char col1, unsigned char col2, int yEnd, unsigned char& foundCol)
{
	const int yStart = y;
	int tmp;
	unsigned char col = 0;

	for(y+=params.skipOffset; y < yEnd; y+=params.skipOffset)
	{
		col = image.getClassifyedColor(x,y);
		if(col == col1 || col == col2)
		{
			tmp = y - params.skipOffset;
			for(--y; y > tmp; y--)
				if(image.getClassifyedColor(x,y) != col)
					break;

			if(y != tmp)
				++y;
			break;
		}
	}
	if(y > yEnd)
		y = yEnd;

	foundCol = col;
	return y;
}
/************************************
 Run up from (x,y) unless there is a run of skipOffset pixels with color != col.
 *************************************/
int ScanVision::runUp(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y-=params.skipOffset; y > yEnd; y-=params.skipOffset)
	{
		if(image.getClassifyedColor(x,y) != col)
		{
			tmp = y + params.skipOffset;
			for(++y; y < tmp; y++)
				if(image.getClassifyedColor(x,y) == col)
					break;

			if(y == tmp)
			{
				y--;
				break;
			}
		}
	}
	if(y < yEnd)
		y = yEnd;
	return y;
}

/**************************************************
Run down from (x,y) unless there is a run of skipOffset pixels with the color col.
****************************************************/
int ScanVision::findDown(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y+=params.skipOffset; y < yEnd; y+=params.skipOffset)
	{
		if(image.getClassifyedColor(x,y) == col)
		{
			tmp = y - params.skipOffset;
			for(--y; y > tmp; y--)
				if(image.getClassifyedColor(x,y) != col)
					break;

			if(y != tmp)
				++y;
			break;
		}
	}
	if(y > yEnd)
		y = yEnd;
	return y;
}

void ScanVision::calcBorders(int xStart, int xEnd)
{
	fieldBorders.clear();

	Vector2<int> p1(0,HorizonLine.base.y) ;
	//p1.y= HorizonLine.base.y;
	Vector2<int> b;
	int min_y_border=0;

	//if horizon is below cameraimage
	if(p1.y >= IMAGE_HEIGHT)
	{
		fieldBorders.push_back(Vector2<int>(0, IMAGE_HEIGHT-1));
		fieldBorders.push_back(Vector2<int>(IMAGE_WIDTH-1, IMAGE_HEIGHT-1));
	}
	else
	{
		if(p1.y <= 0)
			min_y_border = 0;
		else
			min_y_border = p1.y;

		int x = xStart;
		int run_end;
		for(; x < xEnd; x += params.gridStepSize)
		{
			b.x = x;
			b.y = min_y_border;

			run_end = b.y;
			while(b.y < IMAGE_HEIGHT)
			{
				b.y = findDown(b.x, b.y,FIELD, IMAGE_HEIGHT);
				if(b.y >=IMAGE_HEIGHT)
					break;
				const int tyEnd =IMAGE_HEIGHT < b.y + params.borderMinGreen+1? IMAGE_HEIGHT : b.y + params.borderMinGreen+1;
				run_end = runDown(b.x, b.y, FIELD, tyEnd);

				if(run_end - b.y > params.borderMinGreen)
					break;

				b.y = run_end;
			}
			if(b.y >= IMAGE_HEIGHT)
				b.y =  IMAGE_HEIGHT-1;
			fieldBorders.push_back(b);
		}
	}
	fieldBorders = getConvexFieldBorders(fieldBorders);

	std::vector<Vector2<int> > newBorders;
	int x = xStart;
	std::vector<Vector2<int> >::const_iterator p = fieldBorders.begin();
	std::vector<Vector2<int> >::const_iterator last_p = p++;
	for(; p != fieldBorders.end(); p++)
	{
		ASSERT(p->x >= 0);
		ASSERT(p->y >= 0);
		ASSERT(p->x <IMAGE_WIDTH);
		ASSERT(p->y < IMAGE_HEIGHT);
		while(x <= p->x)
		{
			const Vector2<int> v_m = (*p) - (*last_p);
			b.x = x;
			b.y = (x - last_p->x) * v_m.y / v_m.x + last_p->y;
			if(b.y < 0)
				b.y = 0;
			if(b.y >= IMAGE_HEIGHT)
				b.y = IMAGE_HEIGHT-1;
			ASSERT(b.x >= 0);
			ASSERT(b.x < IMAGE_WIDTH);
			newBorders.push_back(b);
			x+=params.gridStepSize;
		}
		last_p = p;
	}
	fieldBorders = newBorders;
}
std::vector<Vector2<int> >ScanVision:: getConvexFieldBorders(std::vector<Vector2<int> >& fieldBorders)
{
	std::vector<Vector2<int> > hull;
	const std::vector<Vector2<int> >::const_iterator pmin = fieldBorders.begin(),
		pmax = fieldBorders.end()-1;
	hull.push_back(*pmin);
	for(std::vector<Vector2<int> >::const_iterator pi = pmin + 1; pi != pmax+1; pi++)
	{
		if(IS_LEFT((*pmin), (*pmax), (*pi)) && pi != pmax)
			continue;

		while((int)hull.size() > 1)
		{
			const std::vector<Vector2<int> >::const_iterator p1 = hull.end() - 1,
				p2 = hull.end() - 2;
			if(IS_LEFT((*p2), (*p1), (*pi)))
				break;
			hull.pop_back();
		}
		hull.push_back(*pi);
	}
	return hull;
}
void ScanVision::Parameters::massAssign()
{
	ASSIGN(gridStepSize);
	ASSIGN(exploreStepSize);
	ASSIGN(borderMinGreen);
	ASSIGN(minBorder);
	ASSIGN(skipOffset);
	ASSIGN(pixelsBelowHorizon);
	ASSIGN(verticalgridStepsize);
}
