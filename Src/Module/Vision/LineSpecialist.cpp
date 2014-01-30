#include "Platform/GetSetting.h"
#include "LineSpecialist.h"
#include <fstream>
#include <string>

int regionLengthFactor[10]={9999,//BLACK =0
						9999,//	FIELD=1
						4,//WHITE=2
						9999,//ORANGE=3
						9999,//RED=4
						6,//BLUE=5
						9999,//GRAY=6
						9999,//CYAN = 7,
						9999,//PINK = 8,
						6};//YELLOW = 9,

LineSpecialist::LineSpecialist()
{
	std::string path = std::string(GetSetting::GetConfigDir()) + "/VisionConfig/lineConfig.cfg";
	//paramAssign::param paramMap;
	params.readCfgFile(path);
	params.massAssign();
	lineWidthOnField.reserve(50);
	nonLineElements.reserve(50);
	//&blobs=theBlobPercept->regions;
	//params.paramReset();
}
LineSpecialist::~LineSpecialist(void)
{
}

void LineSpecialist::update(BlobPercept*theBlobPercept)
{    
	blobsCounter=0;
	buildRegions();
	for(int i=0;i<MAX_REGIONS_COUNT;i++)
	{
		theBlobPercept->regions[i]=blobs[i];
	}
    theBlobPercept->regionsCounter=blobsCounter;
	theBlobPercept->segmentsCounter=theRegionpercept->segmentsCounter;
}
void LineSpecialist::update(BallSpots* theBallSpots)
{

	lineWidthOnField.clear();
	lineElements.clear();
	nonLineElements.clear();
	theBallSpots->ballSpots.clear();
	//blobsCounter=0;
	//buildRegions();
	analyzeRegions(*theBallSpots);
	for (std::vector<BallSpot>::iterator iter=theBallSpots->ballSpots.begin();iter!=theBallSpots->ballSpots.end();iter++)
	{
		//DRAW_BIG_DOT(iter->position.x,iter->position.y,Drawings::blue,Drawings::blue);
	}
	std::list<std::list<LineElements::LineElement>::iterator> toDelete;
	for (std::list<LineElements::LineElement>::iterator iter=lineElements.begin();iter!=lineElements.end();iter++)
	{
			Vector2<double> p1;
			Vector2<double> p2;
			if (Geometry::calculatePointOnField(iter->x_s+iter->alpha_len2*sin(iter->alpha),iter->y_s-iter->alpha_len2*cos(iter->alpha),*theCameraMatrix,p1)
				&&Geometry::calculatePointOnField(iter->x_s-iter->alpha_len2*sin(iter->alpha),iter->y_s+iter->alpha_len2*cos(iter->alpha),*theCameraMatrix,p2))
			{
				double l=Geometry::distance(p1,p2);
				//if(l>1000)
				if(l>params.maxLineWidth)
					toDelete.push_back(iter);
				lineWidthOnField.push_back(l);
			}
			else
				toDelete.push_back(iter);
	}
		for (std::list<std::list<LineElements::LineElement>::iterator>::iterator iter1=toDelete.begin();iter1!=toDelete.end();iter1++)
		lineElements.erase(*iter1);
	//for (std::list<LineElements::LineElement>::iterator iter=lineElements.begin();iter!=lineElements.end();iter++)
	//{
	//	DRAW_LINE(iter->x_s-iter->alpha_len*cos(iter->alpha),iter->y_s-iter->alpha_len*sin(iter->alpha),
	//		iter->x_s+iter->alpha_len*cos(iter->alpha),iter->y_s+iter->alpha_len*sin(iter->alpha),3,Drawings::ps_SolidLine,Drawings::red);
	//}
}
void LineSpecialist::update(LinesPercept* theLinesPercept)
{
	//unsigned  time1=SystemCall::getCurrentSystemTime();
	theLinesPercept->clear();
	lineSegs.clear();
	banSectors.clear();

	//for (std::vector<LineElements::LineElement>::iterator iter=lineElements.begin();iter!=lineElements.end();iter++)
	//{
	//	DRAW_LINE(iter->p1.x,iter->p1.y,iter->p2.x,iter->p2.y,1,Drawings::ps_SolidLine,Drawings::red);
	//}
	createLineSegments(theLinesPercept->singleSegs,*theCameraMatrix);
	//for (std::list<LinesPercept::LineSegment>::iterator iter=lineSegs.begin();iter!=lineSegs.end();iter++)
	//{
	//	DRAW_LINE(iter->p1Img.x,iter->p1Img.y,iter->p2Img.x,iter->p2Img.y,2,Drawings::ps_SolidLine,Drawings::blue);
	//}
	createLines(theLinesPercept->lineOnField, theLinesPercept->singleSegs);
	//for (std::list<LinePercept::LineSegment>::iterator iter=theLinePercept->singleSegs.begin();iter!=theLinePercept->singleSegs.end();iter++)
	//{
	//	DRAW_LINE(iter->p1Img.x,iter->p1Img.y,iter->p2Img.x,iter->p2Img.y,2,Drawings::ps_SolidLine,Drawings::red);
	//}
	//for (std::list<LinePercept::Line>::iterator iter=theLinePercept->lines.begin();iter!=theLinePercept->lines.end();iter++)
	//{
	//	Vector2<int> p1;
	//	Vector2<int> p2;
	//	if (Geometry::calculatePointInImage(iter->first,cameraMatrixLine,p1)&&
	//		Geometry::calculatePointInImage(iter->last,cameraMatrixLine,p2))
	//	{
	//		DRAW_LINE(p1.x,p1.y,p2.x,p2.y,2,Drawings::ps_SolidLine,Drawings::yellow);
	//	}
	//}
	analyzeSingleSegments(theLinesPercept->singleSegs, theLinesPercept->circleCenter, theLinesPercept->lineOnField);
	//for (std::list<LinePercept::LineSegment>::iterator iter=theLinePercept->singleSegs.begin();iter!=theLinePercept->singleSegs.end();iter++)
	//{
	//	DRAW_LINE(iter->p1Img.x,iter->p1Img.y,iter->p2Img.x,iter->p2Img.y,2,Drawings::ps_SolidLine,Drawings::black);
	//}
	analyzeLines(theLinesPercept->lineOnField, theLinesPercept->intersections, theLinesPercept->circleCenter, theLinesPercept->singleSegs);
	//for (std::list<LinePercept::LineSegment>::iterator iter=theLinePercept->singleSegs.begin();iter!=theLinePercept->singleSegs.end();iter++)
	//{
	//	DRAW_LINE(iter->p1Img.x,iter->p1Img.y,iter->p2Img.x,iter->p2Img.y,2,Drawings::ps_SolidLine,Drawings::yellow);
	//}
	for (std::list<LinesPercept::LineOnField>::iterator iter=theLinesPercept->lineOnField.begin();iter!=theLinesPercept->lineOnField.end();iter++)
	{
		Vector2<int> p1;
		Vector2<int> p2;
		if (Geometry::calculatePointInImage(iter->first,*theCameraMatrix,p1)&&
			Geometry::calculatePointInImage(iter->last,*theCameraMatrix,p2))
		{
			DRAW_LINE(p1.x,p1.y,p2.x,p2.y,1,Drawings::ps_SolidLine,Drawings::yellow);
		}
		//Vector2<double>robotPos(0,0);
		//Vector2<int> base=(iter->first+iter->last)/2;
		//Vector2<int>direction=(iter->last-iter->first);
		//Geometry::GTLine gtLine(base,direction);
		//double distanceToLine=Geometry::getDistanceToLine(gtLine,robotPos);
		//std::cout<<"########"<<distanceToLine<<std::endl;
	}
	Vector2<int> circleCenterInImage;
	if ((Geometry::calculatePointInImage(theLinesPercept->circleCenter.pos,*theCameraMatrix,circleCenterInImage))&&theLinesPercept->circleCenter.found==true)
	{
		DRAW_BIG_DOT(circleCenterInImage.x,circleCenterInImage.y,Drawings::red,Drawings::black);
		const double stepSize = 0.2;
		for(double i = 0; i < pi*2; i += stepSize) 
		{
			Vector2<int> p1;
			Vector2<int> p2;
			if(Geometry::calculatePointInImage(theLinesPercept->circleCenter.pos+ Vector2<int>(params.centerCircleRadius+params.circleBiggerThanSpecified, 0).rotate(i), *theCameraMatrix,  p1) &&
				Geometry::calculatePointInImage(theLinesPercept->circleCenter.pos+ Vector2<int>(params.centerCircleRadius+params.circleBiggerThanSpecified, 0).rotate(i+stepSize), *theCameraMatrix,  p2))
			{
				DRAW_LINE(p1.x,p1.y,p2.x,p2.y,3, Drawings::ps_SolidLine, Drawings::darkGray);
			}
		}
	}
	for (std::vector<LinesPercept::Intersection>::iterator iterInter=theLinesPercept->intersections.begin();iterInter!=theLinesPercept->intersections.end();iterInter++)
	{
		Vector2<int> interInImage;
		if (Geometry::calculatePointInImage(iterInter->intersectionOnField,*theCameraMatrix,interInImage))
		{
			DRAW_BIG_DOT(interInImage.x,interInImage.y,Drawings::black,Drawings::black);
		}
	}
	outLinesPercept();
  
	//unsigned time2=SystemCall::getTimeSince(time1);
	//std::cout<<"&&&&&&the line finding time is   "<<time2<<"    "<<std::endl;
	//std::cout<<"updata::outLinesPercept"<<std::endl;
}


void LineSpecialist::buildRegions()
{
	int lastx = -1;

	BlobPercept::Segment* firstInColum=NULL, *lastColumPointer = NULL, *newSegment, *lastSegment = NULL;

	for(int i = 0; i < theRegionpercept->segmentsCounter; i++)
	{
		newSegment = theRegionpercept->segments + i;

		if(newSegment->color == FIELD)
			continue;
		//a new colum started
		if(lastx == -1 || lastx != newSegment->x)
		{
			if(lastx == newSegment->x - params.gridStepSize)
				lastColumPointer = firstInColum;
			else
				lastColumPointer = NULL;
			firstInColum = newSegment;
			lastSegment = NULL;
		}

		lastColumPointer = connectToRegions(newSegment, lastColumPointer, params.gridStepSize); 
		ASSERT(newSegment->blob != NULL || blobsCounter >= MAX_REGIONS_COUNT);
		if(newSegment->blob == NULL) //MAX_REGIONS_COUNT
			break;

		if(lastSegment != NULL)
		{
			if(newSegment-> y - (lastSegment->y + lastSegment->length) < params.skipOffset)
			{
				lastSegment->blob->neighborBlobs.push_back(newSegment->blob);
				newSegment->blob->neighborBlobs.push_back(lastSegment->blob);
			}
			else
				lastSegment = NULL;
		}

		lastSegment = newSegment;
		lastx = newSegment->x;
	}
}
BlobPercept::Segment* LineSpecialist::connectToRegions(BlobPercept::Segment* newSegment, BlobPercept::Segment* lastColumPointer, int xDiff)
{
	ASSERT(!newSegment->blob);
	if(!lastColumPointer)
	{
		createNewRegionForSegment(newSegment);
		return NULL;
	}

	ASSERT(lastColumPointer->x == newSegment->x-xDiff);//parameters.gridStepSize);
	//if lastColumPointer needs to move on
	// _
	//|_|
	//   _    this case
	//  |_| 
	//

	/********************************************************************************/
	////找到形如|     或者|       的线段，后面一根竖线是newSegment，前面一根竖线是lastColumPointer，两根有交叠，且lastColumPointer是newSegment 第一个交叠的竖线
	//			        | |              |  |
	//			           |              |
	while(lastColumPointer->y + lastColumPointer->length < newSegment->explored_min_y && lastColumPointer->x == newSegment->x-xDiff)
		lastColumPointer++;
	/********************************************************************************/


	
	//lastColumPointer is now either the first segment in the last line
	//which ends after the start of newSegment or is already in the next line -> return NULL

	if(lastColumPointer->x != newSegment->x-xDiff)
	{
		createNewRegionForSegment(newSegment);
		return NULL;
	}

	std::vector<BlobPercept::Blob*> neighborRegions;
	/********************************************************************************/
	//无论lastColumPointer的终点是不是比newSegment的终点y值大，
	//只要颜色相同都将这两块merge起来，如果merge不成功(说明不在一条直线上)，
	//则可能是一块新blob，这块新blob是lastColumPointer属于的blob的邻居，
	//如果颜色不同，前一块颜色是绿色(不属于任何blob)，则不管，
	//否则这块新blob是lastColumPointer属于的blob的邻居，
	if(lastColumPointer->y + lastColumPointer->length >= newSegment->explored_min_y)
		if(lastColumPointer->y <= newSegment->explored_max_y)
		{
			//TOUCHING
			if(lastColumPointer->color == newSegment->color)
			{
				if(!uniteRegions(lastColumPointer, newSegment))
					neighborRegions.push_back(lastColumPointer->blob);
			} 
			else
			{
				if(lastColumPointer->blob)
					neighborRegions.push_back(lastColumPointer->blob);
			}
		}
	/********************************************************************************/	

		//  _
		// | |_
		// |_| |
		//   |_| this case
		BlobPercept::Segment *tmpLastColumPointer;
		tmpLastColumPointer = lastColumPointer;

	/********************************************************************************/	
	//针对形如|    的情况分析lastColumPointer下面的线段，
	//			    | |             
	//			       |             
	//如果lastColumPointer下面还有可以被newSegment merge的线段就继续merge，
	//如果newSegment 同时属于两个blob也不会产生问题，
	//因为总是先被merge到第一个blob，再被merge到第二个，当被加到第二个里面的时候会在uniteRegions将第二块和第一块merge起来
		while(tmpLastColumPointer->y + tmpLastColumPointer->length <= newSegment->explored_max_y)
		{
			tmpLastColumPointer++;
			//if the lastColumPointer moved to the next colum, move one back and break
			if(tmpLastColumPointer->x != newSegment->x - xDiff)
			{
				if(!newSegment->blob)
					if(!createNewRegionForSegment(newSegment))
						return NULL;
				break;
			}

			ASSERT(tmpLastColumPointer->y + tmpLastColumPointer->length >= newSegment->explored_min_y);
			if(tmpLastColumPointer->y <= newSegment->explored_max_y)//此处无需判断newSegment起始点y是不是小于tmpLastColumPointer，因为已经进入while循环，是最初始的下面的线段了
			{
				//TOUCHING
				if(tmpLastColumPointer->color == newSegment->color)
				{
					if(!uniteRegions(tmpLastColumPointer, newSegment))
						neighborRegions.push_back(tmpLastColumPointer->blob);
				}
				else
				{
					if(tmpLastColumPointer->blob)
						neighborRegions.push_back(tmpLastColumPointer->blob);
				}
			}
			else
				break;
		}

		if(!newSegment->blob)
			if(!createNewRegionForSegment(newSegment))
				return NULL;

		for(std::vector<BlobPercept::Blob*>::iterator nb_reg = neighborRegions.begin(); nb_reg != neighborRegions.end(); nb_reg++)
		{
			ASSERT(newSegment->blob);
			(*nb_reg)->neighborBlobs.push_back(newSegment->blob);
			newSegment->blob->neighborBlobs.push_back(*nb_reg);
		}
		return lastColumPointer;
}
bool LineSpecialist::createNewRegionForSegment(BlobPercept::Segment* seg)
{
	if(blobsCounter< MAX_REGIONS_COUNT)
	{
		seg->blob = blobs+ blobsCounter++;
		seg->blob->color = seg->color;
		seg->blob->childs.clear();
		seg->blob->neighborBlobs.clear();
		seg->blob->min_y = seg->y;
		seg->blob->max_y = seg->y + seg->length;
		seg->blob->root = NULL;
		seg->blob->size = seg->explored_size;
		seg->blob->childs.push_back(seg);
		return true;
	}
	return false;
}
bool LineSpecialist::uniteRegions(BlobPercept::Segment* seg1, BlobPercept::Segment* seg2)
{
	ASSERT(seg1->blob);
	//we want straight white regions (lines) so don't unite white regions which would not be straight
	if(seg1->color == WHITE)
	{
		ASSERT(seg1->blob->childs.size() >= 1);
		if(seg1->blob->childs.at(seg1->blob->childs.size()-1)->x == seg2->x)
			return false;

		if(seg1->link)
		{
			Vector2<int> linkDiff = Vector2<int>(seg1->x - seg1->link->x, (seg1->y + seg1->length/2) - (seg1->link->y + seg1->link->length/2)),
				thisDiff = Vector2<int>(seg2->x - seg1->x, (seg2->y + seg2->length/2) - (seg1->y + seg1->length/2));
			if(fabs(linkDiff.angle() - thisDiff.angle()) > params.maxAngleDiff)// TODO:这个参数德国人为57度有点大，需要调整
				return false;

		}
	}
	//cnggmu
	if(seg1->length * regionLengthFactor[seg1->color] < seg2->length ||
		seg2->length * regionLengthFactor[seg1->color] < seg1->length)
		return false;

	//seg1 always has a region
	if(!seg2->blob)
	{
		BlobPercept::Segment *sWithRegion = seg1,
			*sWithoutRegion = seg2;

		if((int)sWithRegion->blob->childs.size() < params.regionMaxSize || 
			seg1->color == YELLOW || //yellow, blue and orange regions can grow unlimited
			seg1->color == BLUE ||
			seg1->color == ORANGE)
		{
			sWithRegion->blob->childs.push_back(sWithoutRegion);
			sWithoutRegion->blob = sWithRegion->blob;
			sWithRegion->blob->size += sWithoutRegion->explored_size;

			if(sWithoutRegion->y < sWithRegion->blob->min_y)
				sWithRegion->blob->min_y = sWithoutRegion->y;
			if(sWithoutRegion->y + sWithoutRegion->length > sWithRegion->blob->max_y)
				sWithRegion->blob->max_y = sWithoutRegion->y + sWithoutRegion->length;
			seg2->link = seg1;
			return true;
		}
		ASSERT(seg1->blob == seg2->blob || !seg2->blob);
	}
	
	else//both segments already have a region，这样避免了产生一个白线段属于两个blob的情况，因为我们只要直的白线，而不要拐弯的，更不要有两个拐弯的白线
	{
		//don't unite two white regions (since we want straight white regions -> lines)
		if(seg1->color != WHITE)
		{
			if(seg1->blob != seg2->blob && ((int)(seg1->blob->childs.size() + seg2->blob->childs.size()) < params.regionMaxSize || seg1->color ==YELLOW || seg1->color ==BLUE || seg1->color ==ORANGE))
			{
				BlobPercept::Blob* oldRegion = seg2->blob;
				seg1->blob->mergeWithBlob(seg2->blob);
				oldRegion->childs.clear();
				oldRegion->root = seg1->blob;
				seg2->link = seg1;
				return true;
			}
		}
	}
	return false;
}
void LineSpecialist::analyzeRegions(BallSpots& ballSpots)
{
	LineElements::LineElement lineElement;

	for(const BlobPercept::Blob* blob = blobs; blob - blobs< blobsCounter; blob++)
	{
		if(blob->childs.size() == 0)
			continue;

		switch(blob->color)
		{
		case WHITE:
			{
				float dir;
				if(isLine(blob, dir, lineElement))
				{
					lineElements.push_back(lineElement);
				}
				else
				{
					if(lineElement.alpha != TOTALE_GRUETZE)
					{
						//LINE("module:RegionAnalyzer:robots", lineSpot.x_s, lineSpot.y_s, lineSpot.x_s + (int)(cos(lineSpot.alpha+pi_2) * lineSpot.alpha_len2), lineSpot.y_s + (int)(sin(lineSpot.alpha+pi_2)*lineSpot.alpha_len2), 1, Drawings::ps_solid, ColorClasses::black);
						//ARROW("module:RegionAnalyzer:robots", lineSpot.p1.x, lineSpot.p1.y, lineSpot.p2.x, lineSpot.p2.y, 1, Drawings::ps_solid, ColorClasses::black);
						while(lineElement.alpha > pi)
							lineElement.alpha -= 2 * pi;
						while(lineElement.alpha < -pi)
							lineElement.alpha += 2 * pi;

						if(blob->size > params.minRobotRegionSize && fabs(fabs(lineElement.alpha)-pi_2) < params.maxRobotRegionAlphaDiff && lineElement.alpha_len / lineElement.alpha_len2 > params.minRobotWidthRatio )
						{
							Vector2<int> c = lineElement.p1.y > lineElement.p2.y ? lineElement.p1 : lineElement.p2;//region->getCenter();
							Vector2<int> c2 = lineElement.p1.y > lineElement.p2.y ? lineElement.p2 : lineElement.p1;//region->getCenter();
							//CROSS("module:RegionAnalyzer:robots", c.x, c.y, 4, 2, Drawings::ps_solid, ColorClasses::red);
							//CROSS("module:RegionAnalyzer:robots", c2.x, c2.y, 4, 2, Drawings::ps_solid, ColorClasses::red);
							//DRAWTEXT("module:RegionAnalyzer:robots", c.x, c.y, 150, ColorClasses::black, region->size);
							LineElements::NonLineElement nonLineElement;
							nonLineElement.p1 = c;
							nonLineElement.p2 = c2;
							nonLineElement.size = blob->size;
							nonLineElements.push_back(nonLineElement);
						}
					}
				}
			}
			break;
		case ORANGE:
			{
				Vector2<int> c = blob->getCenter();
                int BelowHorizon=c.y-theRegionpercept->horizonline.base.y;
				int m00 = blob->calcMoment00(),
					m10 = blob->calcMoment10(),
					m01 = blob->calcMoment01(),
					swpX = m10/m00,
					swpY = m01/m00,
					m20 = blob->calcCMoment20(swpX);

				double m02 = blob->calcCMoment02(swpY),
					m11 = blob->calcCMoment11(swpX, swpY);

				double eccentricity;
				//balls far away might just be scanned by one grid, so eccentricity would be 0
                 eccentricity = 1. - (sqr(m20 - m02) + 4 * sqr(m11)) / (sqr(m20 + m02));//图像偏心率计算公式
 				 if(blob->childs.size()==1&&eccentricity==0&&(blob->max_y-blob->min_y)>=3&&BelowHorizon>=20)
				 {	 
					 eccentricity=0.6;
					 ballSpots.addBallSpot(c.x, c.y, eccentricity);
					 DRAW_BIG_DOT(c.x,c.y,Drawings::darkRed,Drawings::darkRed);
					  //std::cout<<"离心率=="<<eccentricity<<std::endl;

				 }
				 if (blob->childs.size()>1&&eccentricity >=params.minEccentricityOfBallRegion&&(blob->max_y-blob->min_y)>=3&&BelowHorizon>=20)//将水平线上的点剔除
				 {
					 ballSpots.addBallSpot(c.x, c.y, eccentricity);
					 DRAW_BIG_DOT(c.x,c.y,Drawings::blue,Drawings::blue);
					  //std::cout<<"离心率=="<<eccentricity<<std::endl;
				 }
                 
				break;
			}
		default:
			break;
		}
	}
}
bool LineSpecialist::isLine(const BlobPercept::Blob* blob, float& direction, LineElements::LineElement& lineElement)
{
	bool ret = true;
	if(blob->size < params.minLineSize ||
		(blob->childs.size() < (size_t)params.minLineSegmentCount &&
		blob->size < params.minLineSingleSegmentSize))
	{
		//COMPLEX_DRAWING("module:RegionAnalyzer:Line",
		//	Vector2<int> c = region->getCenter();
		//if(region->size < parameters.minLineSize)
		//{
		//	DRAWTEXT("module:RegionAnalyzer:Line", c.x, c.y, 150, ColorClasses::black, region->size << "<s");
		//}
		//if(region->childs.size() < (size_t)parameters.minLineSegmentCount)
		//{
		//	DRAWTEXT("module:RegionAnalyzer:Line", c.x, c.y-5, 150, ColorClasses::black, region->childs.size() << "<c");
		//}

		//);

		ret = false;
	}

	int neighborNoneSize = 0;
	for(std::vector<BlobPercept::Blob*>::const_iterator neighbor = blob->neighborBlobs.begin(); neighbor != blob->neighborBlobs.end(); neighbor++)
	{
		switch((*neighbor)->color)
		{
		case BLACK:
			neighborNoneSize += (*neighbor)->size;
			break;
		default:
			break;
		}
	}

	if(neighborNoneSize > params.maxLineNghbNoneSize ||
		((float)neighborNoneSize / blob->size) > params.maxLineNghbNoneRatio)
	{
		//COMPLEX_DRAWING("module:RegionAnalyzer:Line",
		//	Vector2<int> c = region->getCenter();
		//if(neighborNoneSize > parameters.maxLineNghbNoneSize)
		//{
		//	DRAWTEXT("module:RegionAnalyzer:Line", c.x + 3, c.y - 5, 150, ColorRGBA(255,0,0), neighborNoneSize);
		//	ARROW("module:RegionAnalyzer:Line", c.x, c.y, c.x, c.y - 5, 1, Drawings::ps_solid, ColorRGBA(255, 0, 0));
		//}
		//if((float)neighborNoneSize / region->size > parameters.maxLineNghbNoneRatio)
		//{
		//	DRAWTEXT("module:RegionAnalyzer:Line", c.x + 5, c.y, 150, ColorRGBA(255,0,0), neighborNoneSize/(float)region->size);
		//	ARROW("module:RegionAnalyzer:Line", c.x, c.y, c.x + 5, c.y, 1, Drawings::ps_solid, ColorRGBA(255, 0, 0));
		//}

		//);
		ret = false;
	}

	//calculate haupttraegheitsachsen and schwerpunkt from moments
	//计算blob的矩，m00,m10,m01是00,10,01阶矩，m20,m02,m11是20,02,11阶中心矩
	//blob的主轴偏角为 atan2(-2 * m11),(m02-m20))/2
	//加上90度是因为这个公式是在标准笛卡尔坐标下的，但是现在图像坐标的y轴和笛卡尔坐标系相反，所以加90度
	const int m00 = blob->calcMoment00(),
		m10 = blob->calcMoment10(),
		m01 = blob->calcMoment01();
	const int x_s = m10/m00, 
		y_s = m01/m00;
	const double m20 = blob->calcCMoment20(x_s),
		m02 = blob->calcCMoment02(y_s),
		m11 = blob->calcCMoment11(x_s, y_s);
	const double cm20 = m20/m00,
		cm02 = m02/m00;
	//CROSS("module:RegionAnalyzer:Line", x_s, y_s, 2, 2, Drawings::ps_solid, ColorClasses::black);
	lineElement.x_s = x_s;
	lineElement.y_s = y_s;
	float alpha = 0;
	if(cm20 != cm02)
	{
		//for an (yet) unknown reason we need to add an offset of 90 degrees
		alpha = .5f * atan2(-2 * ((float)m11),(float)(m02-m20))+(float)pi_2;
		lineElement.alpha = alpha;
	}
	else
	{
		lineElement.alpha = TOTALE_GRUETZE;
		ret = false;
	}

	bool verticalRegion = false;
	if(alpha > pi_2 - pi_4 && alpha < pi_2 + pi_4)
	{
		verticalRegion = true;
		//CROSS("module:RegionAnalyzer:Line", x_s, y_s, 2, 2, Drawings::ps_solid, ColorClasses::green);
	}

	if(verticalRegion)
	{
		int greenLeftSize = getGreenLeft(blob);
		int greenRightSize = getGreenRight(blob);

		if(greenLeftSize < params.minLineNghbGreenSideSize || greenRightSize < params.minLineNghbGreenSideSize)
		{
			//COMPLEX_DRAWING("module:RegionAnalyzer:Line",
			//	Vector2<int> c = region->getCenter();
			//if(greenLeftSize < parameters.minLineNghbGreenSideSize)
			//{
			//	ARROW("module:RegionAnalyzer:Line", c.x, c.y, c.x-5, c.y, 1, Drawings::ps_solid, ColorClasses::green);
			//	DRAWTEXT("module:RegionAnalyzer:Line", c.x - 3, c.y + 5, 150, ColorClasses::black, greenLeftSize);
			//}
			//if(greenRightSize < parameters.minLineNghbGreenSideSize)
			//{
			//	ARROW("module:RegionAnalyzer:Line", c.x, c.y, c.x+5, c.y, 1, Drawings::ps_solid, ColorClasses::green);
			//	DRAWTEXT("module:RegionAnalyzer:Line", c.x + 3, c.y - 5, 150, ColorClasses::black, greenRightSize);
			//}
			//);
			ret = false;
		}
	}
	else
	{
		int greenAboveSize = getGreenAbove(blob);
		int greenBelowSize = getGreenBelow(blob);

		if(greenAboveSize < params.minLineNghbGreenAboveSize ||
			greenBelowSize < params.minLineNghbGreenBelowSize)
		{
			//COMPLEX_DRAWING("module:RegionAnalyzer:Line",
			//	Vector2<int> c = region->getCenter();
			//if(greenAboveSize < parameters.minLineNghbGreenAboveSize)
			//{
			//	ARROW("module:RegionAnalyzer:Line", c.x, c.y, c.x, c.y - 5, 1, Drawings::ps_solid, ColorClasses::green);
			//	DRAWTEXT("module:RegionAnalyzer:Line", c.x + 3, c.y - 5, 150, ColorClasses::green, greenAboveSize);
			//}
			//if(greenBelowSize < parameters.minLineNghbGreenBelowSize)
			//{
			//	ARROW("module:RegionAnalyzer:Line", c.x, c.y - 5, c.x, c.y, 1, Drawings::ps_solid, ColorClasses::green);
			//	DRAWTEXT("module:RegionAnalyzer:Line", c.x + 3, c.y - 5, 150, ColorClasses::green, greenBelowSize);
			//}
			//);
			ret = false;
		}

	}

	//cut alpha_len and alpha_len2 to the maximum distance a point has to
	//the haupttraegheitsachse
	const Vector2<double> achse1(cos(alpha), sin(alpha)),
		achse2(cos(alpha + pi_2), sin(alpha+pi_2));
	double len_max = 0,
		len_min = 0,
		len_max2 = 0,
		len_min2 = 0;
	for(std::vector<BlobPercept::Segment*>::const_iterator child = blob->childs.begin(); child != blob->childs.end(); child++)
	{
		const BlobPercept::Segment* seg = *child;
		const Vector2<double> childRelToSwp(seg->x - lineElement.x_s, seg->y - lineElement.y_s),
			child2RelToSwp(seg->x - lineElement.x_s, seg->y + seg->length - lineElement.y_s);
	//childRelToSwp 为blob质心到每段segment的起点的向量，child2RelToSwp为blob质心到每段segment终点的向量
	
	//计算childRelToSwp和child2RelToSwp 在主轴方向上的投影，如果为正 找正的最大值，如果为负找负的最小值
	//这样会得到沿主轴方向离质心最远的点len_max和背对主轴方向离质心最远的点len_min
		const double dist = childRelToSwp * achse1;
		if(dist > len_max)
			len_max = dist;
		else if(dist < len_min)
			len_min = dist;

		const double dist2 = child2RelToSwp * achse1;
		if(dist2 > len_max)
			len_max = dist2;
		else if(dist2 < len_min)
			len_min = dist2;

	//计算childRelToSwp和child2RelToSwp 在辅轴 (即法线)方向上的投影，如果为正 找正的最大值，如果为负找负的最小值
	//这样会得到沿辅轴方向离质心最远的点len_max2和背对辅轴方向离质心最远的点len_min2

		const double dist3 = childRelToSwp * achse2;
		if(dist3 > len_max2)
			len_max2 = dist3;
		else if(dist3 < len_min2)
			len_min2 = dist3;

		const double dist4 = child2RelToSwp * achse2;
		if(dist4 > len_max2)
			len_max2 = dist4;
		else if(dist4 < len_min2)
			len_min2 = dist4;

	}
	//len_max和len_min中小的一个为主轴的实际长的一半，len_max2和len_min2中小的一个为辅轴实际长的一半，这样是从更精确的角度出发，不取最大值取最小值为了可信
	lineElement.alpha_len = fabs(len_min) < len_max ? fabs(len_min) : len_max;
	lineElement.alpha_len2 = fabs(len_min2) < len_max2 ? fabs(len_min2) : len_max2;
	if(lineElement.alpha_len2 == 0)
		lineElement.alpha_len2 = params.gridStepSize;
	if(lineElement.alpha_len == 0) //I don't think this can happen, but.....
		lineElement.alpha_len = params.gridStepSize;

	lineElement.p1 = Vector2<int>((int)(x_s + cos(lineElement.alpha) * len_max), (int)(y_s + sin(lineElement.alpha) * len_max));
	lineElement.p2 = Vector2<int>((int)(x_s + cos(lineElement.alpha) * len_min), (int)(y_s + sin(lineElement.alpha) * len_min));
	direction = alpha;

	return ret;
}
int LineSpecialist::getGreenBelow(const BlobPercept::Blob* blob)
{
	int greenBelow = 0;
	for(std::vector<BlobPercept::Segment*>::const_iterator child = blob->childs.begin(); child != blob->childs.end(); child++)
	{
		if((*child) - theRegionpercept->segments == theRegionpercept->segmentsCounter-1)
			continue;

		BlobPercept::Segment* next = (*child)+1;
		const int childEndY = (*child)->y + (*child)->length;

		//find next green segment
		while(next - theRegionpercept->segments < theRegionpercept->segmentsCounter && 
			next->x == (*child)->x && 
			next->color != FIELD)
			next++;

		if(next - theRegionpercept->segments < theRegionpercept->segmentsCounter && next->x == (*child)->x)
		{
			//implizit durch die if-Bedingung und die while-Bedingung
			//if(next->color == ColorClasses::green)
			if(next->y - childEndY <= params.maxLineNghbGreySkip)
			{
				greenBelow += next->length;
			/*	LINE("module:RegionAnalyzer:greenBelow", next->x, next->y, next->x, next->y + next->length, 1, Drawings::ps_solid, ColorClasses::green); */
			}
		}
	}
	ASSERT(greenBelow >= 0);
	return greenBelow;
}

int LineSpecialist::getGreenAbove(const BlobPercept::Blob* blob)
{
	int greenAbove = 0;
	for(std::vector<BlobPercept::Segment*>::const_iterator child = blob->childs.begin(); child != blob->childs.end(); child++)
	{
		if((*child) == theRegionpercept->segments)
			continue;

		BlobPercept::Segment* next = (*child)-1;

		//find previous green segment
		while(next->x == (*child)->x && 
			next > theRegionpercept->segments && 
			next->color !=FIELD)
			next--;

		ASSERT(next >= theRegionpercept->segments);

		if(next->color == FIELD && next->x == (*child)->x)
		{
			//if(next->color == ColorClasses::green)
			if((*child)->y - (next->y + next->length) <= params.maxLineNghbGreySkip)
			{
				greenAbove += next->length;
				//LINE("module:RegionAnalyzer:greenAbove", next->x, next->y, next->x, (*child)->y, 1, Drawings::ps_solid, ColorClasses::blue); 
			}
		}
	}
	ASSERT(greenAbove >= 0);
	return greenAbove;
}

int LineSpecialist::getGreenRight(const BlobPercept::Blob* blob)
{
	const BlobPercept::Segment* nextColumnSegment;
	//const int& gridStepSize = theRegionPercept.gridStepSize;
	int greenRight = 0;
	for(std::vector<BlobPercept::Segment*>::const_iterator seg_iter = blob->childs.begin();
		seg_iter != blob->childs.end();
		seg_iter++)
	{
		nextColumnSegment = *seg_iter+1;
		//find first segment in next column
		while(nextColumnSegment < theRegionpercept->segments + theRegionpercept->segmentsCounter && 
			nextColumnSegment->x == (*seg_iter)->x)
			nextColumnSegment++;

		if(nextColumnSegment >= theRegionpercept->segments + theRegionpercept->segmentsCounter ||
			nextColumnSegment->x > (*seg_iter)->x + params.gridStepSize)
			continue;

		ASSERT(nextColumnSegment->x == (*seg_iter)->x + params.gridStepSize);

		//find first segment which ends after seg_iter->y
		while(nextColumnSegment < theRegionpercept->segments + theRegionpercept->segmentsCounter && 
			nextColumnSegment->x == (*seg_iter)->x + params.gridStepSize &&
			nextColumnSegment->y + nextColumnSegment->length <= (*seg_iter)->y)
			nextColumnSegment++;

		if(nextColumnSegment >= theRegionpercept->segments + theRegionpercept->segmentsCounter ||
			nextColumnSegment->x > (*seg_iter)->x + params.gridStepSize)
			continue;

		//find first segment touching seg_iter and check from there on whether there are green segments
		while(nextColumnSegment->y < (*seg_iter)->y + (*seg_iter)->length && 
			nextColumnSegment->x == (*seg_iter)->x + params.gridStepSize &&
			nextColumnSegment < theRegionpercept->segments + theRegionpercept->segmentsCounter)
		{
			if(nextColumnSegment->color ==FIELD)
			{
				const int start = std::max<int>(nextColumnSegment->y, (*seg_iter)->y);
				const int end = std::min<int>(nextColumnSegment->y + nextColumnSegment->length, (*seg_iter)->y + (*seg_iter)->length);
				ASSERT(end >= start);
				greenRight += end - start;
				//LINE("module:RegionAnalyzer:greenRight", nextColumnSegment->x, start, nextColumnSegment->x, end, 1, Drawings::ps_solid, ColorClasses::blue); 
			}
			nextColumnSegment++;
		}
	}
	ASSERT(greenRight >= 0);
	return greenRight;
}


int LineSpecialist::getGreenLeft(const BlobPercept::Blob* blob)
{
	const BlobPercept::Segment* lastColumnSegment;
	//const int& gridStepSize = theRegionPercept.gridStepSize;
	int greenLeft = 0;
	for(std::vector<BlobPercept::Segment*>::const_iterator seg_iter = blob->childs.begin();
		seg_iter != blob->childs.end();
		seg_iter++)
	{
		lastColumnSegment = *seg_iter-1;
		//find first segment in previous column
		while(lastColumnSegment >= theRegionpercept->segments && 
			lastColumnSegment->x == (*seg_iter)->x)
			lastColumnSegment--;

		if(lastColumnSegment < theRegionpercept->segments || 
			lastColumnSegment->x < (*seg_iter)->x - params.gridStepSize)
			continue;

		ASSERT(lastColumnSegment->x == (*seg_iter)->x - params.gridStepSize);

		//find first segment which begins before seg_iter->y + set_iter->length
		while(lastColumnSegment >= theRegionpercept->segments && 
			lastColumnSegment->x == (*seg_iter)->x - params.gridStepSize &&
			lastColumnSegment->y >= (*seg_iter)->y + (*seg_iter)->length)
			lastColumnSegment--;

		if(lastColumnSegment < theRegionpercept->segments || 
			lastColumnSegment->x < (*seg_iter)->x - params.gridStepSize)
			continue;

		//find first segment touching seg_iter and check from there on whether there are green segments
		while(lastColumnSegment >= theRegionpercept->segments &&
			lastColumnSegment->y + lastColumnSegment->length > (*seg_iter)->y && 
			lastColumnSegment->x == (*seg_iter)->x - params.gridStepSize)
		{
			if(lastColumnSegment->color == FIELD)
			{
				const int start = std::max<int>(lastColumnSegment->y, (*seg_iter)->y);
				const int end = std::min<int>(lastColumnSegment->y + lastColumnSegment->length, (*seg_iter)->y + (*seg_iter)->length);
				ASSERT(end >= start);
				greenLeft += end - start;
				//LINE("module:RegionAnalyzer:greenLeft", lastColumnSegment->x, start, lastColumnSegment->x, end, 1, Drawings::ps_solid, ColorClasses::blue); 
			}
			lastColumnSegment--;
		}
	}
	ASSERT(greenLeft >= 0);
	return greenLeft;
}
void LineSpecialist::createLineSegments(std::list<LinesPercept::LineSegment>& singleSegs,const CameraMatrix& cameraMatrix)
{
	createBanSectors(cameraMatrix);

	for(std::list<LineElements::LineElement>::const_iterator lineElement = lineElements.begin(); lineElement !=lineElements.end(); lineElement++)
	{
		if(lineElement->alpha_len/lineElement->alpha_len2 > params.minWidthRatio)
		{
			//transform to field coordinates
			//const Vector2<double> p1_cor = theImageCoordinateSystem.toCorrected(spot->p1),
			//	p2_cor = theImageCoordinateSystem.toCorrected(spot->p2);
			Vector2<int> pf1, pf2;
			if(!Geometry::calculatePointOnField((int)lineElement->p1.x, (int)lineElement->p1.y, cameraMatrix, pf1))
			{
				//CROSS("module:LinePerceptor:LineSegmentsImg", ((spot->p1+spot->p2)/2).x, ((spot->p1+spot->p2)/2).y,4, 2, Drawings::ps_solid, ColorClasses::orange);
				continue;
			}
			if(!Geometry::calculatePointOnField((int)lineElement->p2.x, (int)lineElement->p2.y, cameraMatrix, pf2))
			{
				//CROSS("module:LinePerceptor:LineSegmentsImg", ((spot->p1+spot->p2)/2).x, ((spot->p1+spot->p2)/2).y,4, 2, Drawings::ps_solid, ColorClasses::orange);
				continue;
			}

			//hmmmmm at least in some special cases (corrupted log file) this happens and causes
			//other parts of this module to crash. So if it happens, just ignore the spot
			//this seems to happen if the camera matrix gets messed up
			if(pf1 == pf2)
			{
				//CROSS("module:LinePerceptor:LineSegmentsImg", ((spot->p1+spot->p2)/2).x, ((spot->p1+spot->p2)/2).y,4, 2, Drawings::ps_solid, ColorClasses::robotBlue);
				continue;
			}

			if(pf1 * pf1 > sqr(params.maxLineDistance) || pf2 * pf2 > sqr(params.maxLineDistance))
			{
				//CROSS("module:LinePerceptor:LineSegments", ((pf1+pf2)/2).x, ((pf1+pf2)/2).y,20, 20, Drawings::ps_solid, ColorClasses::red);
				continue;
			}


			if(fabs(fabs(lineElement->alpha) - pi_2) < params.maxAlphaDiff && lineElement->alpha_len / lineElement->alpha_len2 > params.minWidthRatio && (pf1 - pf2).squareAbs() > sqr(params.minLineLength))
			{
				//ARROW("module:LinePerceptor:LineSegmentsImg", spot->p1.x, spot->p1.y, spot->p2.x, spot->p2.y, 3, Drawings::ps_dash, ColorClasses::red);
				//ARROW("module:LinePerceptor:NonLineSegments", pf1.x, pf1.y, pf2.x, pf2.y, 30, Drawings::ps_dash, ColorClasses::red);

				double alpha = (pf1.angle() + pf2.angle()) / 2;

				while(alpha >= pi_2)
					alpha -= pi;
				while(alpha < -pi_2)
					alpha += pi;

				bool con = false;
				const int dist = ((pf1 + pf2)/2).squareAbs();
				for(std::list<BanSector>::iterator s = banSectors.begin(); s != banSectors.end(); s++)
				{
					if((alpha > s->alphaLeft && alpha < s->alphaRight) ||
						fabs(alpha - s->alphaLeft) < params.maxLineAngleDiff ||
						fabs(alpha - s->alphaRight) < params.maxLineAngleDiff)
					{
						if(dist > sqr(s->start))
						{
							//ARROW("module:LinePerceptor:LineSegmentsImg", spot->p1.x, spot->p1.y, spot->p2.x, spot->p2.y, 3, Drawings::ps_dash, ColorClasses::yellow);
							con = true;
							break;
						}
					}
				}
				if(con)
					continue;
			}


			LinesPercept::LineSegment s;
			s.p1 = pf1;
			s.p2 = pf2;
			s.p1Img = lineElement->p1;
			s.p2Img = lineElement->p2;

			Vector2<int> diff = pf2 - pf1;
			//hmmm here once again we need a magic offset of 90 degree
			s.alpha = diff.angle() + pi_2;
			//normalize alpha
			while(s.alpha < 0)
				s.alpha += pi;
			while(s.alpha > pi)
				s.alpha -= pi;
			const double d1 = s.p1.x * cos(s.alpha) + s.p1.y * sin(s.alpha),
				d2 = s.p2.x * cos(s.alpha) + s.p2.y * sin(s.alpha);
			s.d = (d1 + d2) / 2.0f;
			lineSegs.push_back(s);

			//ARROW("module:LinePerceptor:LineSegmentsImg", spot->p1.x, spot->p1.y, spot->p2.x, spot->p2.y, 1, Drawings::ps_solid, ColorClasses::blue);
			//COMPLEX_DRAWING("module:LinePerceptor:LineSegments",
			//	ARROW("module:LinePerceptor:LineSegments", pf1.x, pf1.y, pf2.x, pf2.y, 15, Drawings::ps_solid, ColorClasses::blue);
			///*
			//DRAWTEXT("module:LinePerceptor:LineSegments", (pf1.x+pf2.x)/2, (pf1.y+pf2.y)/2+50, 10, ColorClasses::black, s.alpha);
			//DRAWTEXT("module:LinePerceptor:LineSegments", (pf1.x+pf2.x)/2, (pf1.y+pf2.y)/2, 10, ColorClasses::black, s.d);*/
			//);

		}
	}
}

void LineSpecialist::createLines(std::list<LinesPercept::LineOnField>& lines, std::list<LinesPercept::LineSegment>& singleSegs)
{
	//Hough Transformation fuer (ganz) arme....
	while(lineSegs.size() > 0)
	{
		//pick a segment...
		LinesPercept::LineSegment seg = *lineSegs.begin();
		lineSegs.erase(lineSegs.begin());

		//ARROW("module:LinePerceptor:Lines1", seg.p1.x, seg.p1.y, seg.p2.x, seg.p2.y, 15, Drawings::ps_solid, ColorClasses::white);

		//collect supporters...
		std::vector<std::list<LinesPercept::LineSegment>::iterator> supporters;
		int maxSegmentLength = 0;
		for(std::list<LinesPercept::LineSegment>::iterator other = lineSegs.begin(); other != lineSegs.end(); other++)
		{
			if((fabs(other->alpha - seg.alpha) < params.maxAlphaDiff &&
				fabs(other->d - seg.d) < params.maxDDiff))
			{
				const int sqr_length = (other->p1-other->p2).squareAbs();
				if(sqr_length > maxSegmentLength)
					maxSegmentLength = sqr_length;
				supporters.push_back(other);
			}
			else if((fabs(fabs(other->alpha - seg.alpha) - pi) < params.maxAlphaDiff &&
				fabs(other->d + seg.d) < params.maxDDiff))
			{
				const int sqr_length = (other->p1-other->p2).squareAbs();
				if(sqr_length > maxSegmentLength)
					maxSegmentLength = sqr_length;
				//make supporters all look into the same direction (alpha in [0...pi])
				if(other->alpha > seg.alpha)
					other->alpha -= pi;
				else
					other->alpha += pi;
				other->d *= -1;
				supporters.push_back(other);
			}
		}
		maxSegmentLength = static_cast<int>(sqrt(static_cast<double>(maxSegmentLength)));

		//if you have enough supporters, you become a line
		if((int)supporters.size() >= params.minSupporters && maxSegmentLength > params.minLineStartLength)
		{
			//COMPLEX_DRAWING("module:LinePerceptor:Lines1",
			//	CROSS("module:LinePerceptor:Lines1", (seg.p1.x+seg.p2.x)/2, (seg.p1.y+seg.p2.y)/2,20, 20, Drawings::ps_solid, ColorClasses::red);
			//DRAWTEXT("module:LinePerceptor:Lines1", seg.p1.x+50, seg.p1.y+100, 10, ColorClasses::black, (int)supporters.size());
			//);
			LinesPercept::LineOnField l;
			double d = seg.d, alpha = seg.alpha;
			l.dead = false;
			l.midLine = false;
			l.segments.push_back(seg);
			for(std::vector<std::list<LinesPercept::LineSegment>::iterator>::const_iterator sup = supporters.begin(); sup != supporters.end(); sup++)
			{
				//ARROW("module:LinePerceptor:Lines1", (*sup)->p1.x, (*sup)->p1.y, (*sup)->p2.x, (*sup)->p2.y, 15, Drawings::ps_solid, ColorClasses::red);
				//ARROW("module:LinePerceptor:Lines1", seg.p1.x, seg.p1.y, (*sup)->p1.x, (*sup)->p1.y, 5, Drawings::ps_solid, ColorClasses::robotBlue);
				d += (*sup)->d;
				alpha += (*sup)->alpha;
				l.segments.push_back(*(*sup));
			}
			for(std::vector<std::list<LinesPercept::LineSegment>::iterator>::const_reverse_iterator sup = supporters.rbegin(); sup != supporters.rend(); sup++)
				lineSegs.erase(*sup);
			l.d = d / ((int)supporters.size()+1);
			l.alpha = alpha / ((int)supporters.size()+1);
			lines.push_back(l); 
		}
		else
			singleSegs.push_back(seg);
	}
}

void LineSpecialist::getFirstAndLastOfLine(LinesPercept::LineOnField& line, Vector2<int>& first, Vector2<int>& last, bool updateLine)
{
	Vector2<int> p_ref=line.segments.at(0).p1;
	first=p_ref;
	last=p_ref;
	double first_dist=0, last_dist=0;
	Vector2<int> alphaDir = Vector2<int>(0,0); 
	std::vector<LinesPercept::LineSegment>::const_iterator prevSeg;
	for(std::vector<LinesPercept::LineSegment>::const_iterator seg = line.segments.begin(); seg != line.segments.end(); seg++)
	{
		if(seg != line.segments.begin())
		{
			//ARROW("module:LinePerceptor:Lines2", ((seg->p1 + seg->p2)/2).x, ((seg->p1 + seg->p2)/2).y,((prevSeg->p1 + prevSeg->p2)/2).x, ((prevSeg->p1 + prevSeg->p2)/2).y , 5, Drawings::ps_solid, ColorClasses::yellow);
			alphaDir += (seg->p1 + seg->p2)/2 - (prevSeg->p1 + prevSeg->p2)/2;
		}

		const Vector2<int> diffp1 = seg->p1 - p_ref,
			diffp2 = seg->p2 - p_ref;

		//if dist(p1, p_ref) > dist(first, p_ref) and dist(first, p1) < dist(p1, p_ref
		//-->means if p1 is farer away from p_ref as first and is on the same side of p_ref as first
		if(diffp1.squareAbs() > first_dist && (seg->p1 - first).squareAbs() <= diffp1.squareAbs())
		{
			first = seg->p1;
			first_dist = diffp1.squareAbs();
		}
		else if(diffp1.squareAbs() > last_dist && (seg->p1 - first).squareAbs() > (p_ref - first).squareAbs() && (seg->p1 - last).squareAbs() <= diffp1.squareAbs())
		{
			last = seg->p1;
			last_dist = diffp1.squareAbs();
		}
		if(diffp2.squareAbs() > first_dist && (seg->p2 - first).squareAbs() <= diffp2.squareAbs())
		{
			first = seg->p2;
			first_dist = diffp2.squareAbs();
		}
		else if(diffp2.squareAbs() > last_dist && (seg->p2 - first).squareAbs() > (p_ref - first).squareAbs() && (seg->p2 - last).squareAbs() <= diffp2.squareAbs())
		{
			last = seg->p2;
			last_dist = diffp2.squareAbs();
		}
		prevSeg = seg;
	}

	if(updateLine)
	{
		if((int) line.segments.size() > 1)
		{
			alphaDir /= (int)line.segments.size()-1;
			line.alpha = alphaDir.angle()+pi_2;
			while(line.alpha < 0)
				line.alpha += pi;
			while(line.alpha > pi)
				line.alpha -= pi;

			line.d = 0;
			const double c = cos(line.alpha),
				s = sin(line.alpha);
			for(std::vector<LinesPercept::LineSegment>::const_iterator seg = line.segments.begin(); seg != line.segments.end(); seg++)
				line.d += seg->p1.x * c + seg->p1.y * s;
			line.d /= (int)line.segments.size();
		}
		else
		{
			line.alpha = prevSeg->alpha;
			line.d = prevSeg->d;
		}
	}

	first = line.calculateClosestPointOnLine(first);
	last = line.calculateClosestPointOnLine(last);
}

void LineSpecialist::analyzeLines(std::list<LinesPercept::LineOnField>& lines, std::vector<LinesPercept::Intersection>& intersections, LinesPercept::CircleSpot& circle, std::list<LinesPercept::LineSegment>& singleSegs)
{
	//the points first and last are the two points on the line which
	//have the greatest distance to each other ("endpoints")
	for(std::list<LinesPercept::LineOnField>::iterator line = lines.begin(); line != lines.end(); line++)
		getFirstAndLastOfLine(*line, line->first, line->last);


	//delete lines if their circleSpot is near the found circle
	if(circle.found)
	{
		std::vector<std::list<LinesPercept::LineOnField>::iterator> toDelete;
		for(std::list<LinesPercept::LineOnField>::iterator l1 = lines.begin(); l1 != lines.end(); l1++)
		{
			Vector2<int> line_mid = (l1->first + l1->last)/2;
			//CROSS("module:LinePerceptor:CircleSpots", line_mid.x, line_mid.y, 30, 30, Drawings::ps_solid, ColorClasses::green);
			Vector2<int> bla = Vector2<int>(line_mid + (l1->first-l1->last).rotate(pi_2).normalize(params.centerCircleRadius+params.circleBiggerThanSpecified));
			//CROSS("module:LinePerceptor:CircleSpots", bla.x, bla.y, 30, 30, Drawings::ps_solid, ColorClasses::green);
			if((bla - circle.pos).squareAbs() < sqr(params.maxLineCircleDist))
				toDelete.push_back(l1);
			else 
			{
				Vector2<int> bla = Vector2<int>(line_mid + (l1->first-l1->last).rotate(-pi_2).normalize(params.centerCircleRadius+params.circleBiggerThanSpecified));
				//CROSS("module:LinePerceptor:CircleSpots", bla.x, bla.y, 30, 30, Drawings::ps_solid, ColorClasses::green);
				if((bla - circle.pos).squareAbs() < sqr(params.maxLineCircleDist))
					toDelete.push_back(l1);
			}

		}
		for(std::vector<std::list<LinesPercept::LineOnField>::iterator>::iterator t = toDelete.begin(); t != toDelete.end(); t++)
			lines.erase(*t);
	}

	//find lines which are allmost parallel
	std::vector<std::list<LinesPercept::LineOnField>::iterator> toDelete;
	for(std::list<LinesPercept::LineOnField>::iterator line = lines.begin(); line != lines.end(); line++)
	{
		if(line->dead)
			continue;
		std::list<LinesPercept::LineOnField>::iterator other = line;
		other++;
		for(; other != lines.end(); other++)
		{
			if(other->dead)
				continue;

			double alphaDiff = line->alpha - other->alpha;
			while(alphaDiff < - pi_2)
				alphaDiff += pi;
			while(alphaDiff > pi_2)
				alphaDiff -= pi;
			//if endpoints of the other line are close to the line
			if( ((fabs(line->calculateDistToLine(other->first)) < params.maxLineUniteDist &&
				fabs(line->calculateDistToLine(other->last)) < params.maxLineUniteDist)
				||
				(fabs(other->calculateDistToLine(line->first)) < params.maxLineUniteDist &&
				fabs(other->calculateDistToLine(line->last)) < params.maxLineUniteDist)
				)
				&&
				fabs(alphaDiff) < params.maxLineUniteAlphaDiff
				)
			{
				line->segments.insert(line->segments.end(), other->segments.begin(), other->segments.end());
				line->d = (line->d + other->d) / 2;
				if(line->alpha - other->alpha > pi_2)
					other->alpha += pi;
				else if(other->alpha - line->alpha > pi_2)
					other->alpha -= pi;
				line->alpha = (line->alpha + other->alpha) / 2;
				if(line->alpha < 0)
					line->alpha += pi;
				getFirstAndLastOfLine(*line, (*line).first, (*line).last);
				toDelete.push_back(other);
				other->dead = true;

			}
		}
	}
	for(std::vector<std::list<LinesPercept::LineOnField>::iterator>::iterator t = toDelete.begin(); t != toDelete.end(); t++)
		lines.erase(*t);

	//add singleSegments where the start and end pos is close to a line to the line
	std::vector<std::list<LinesPercept::LineSegment>::iterator> ttoDelete;
	for(std::list<LinesPercept::LineSegment>::iterator seg = singleSegs.begin(); seg != singleSegs.end(); seg++)
	{
		for(std::list<LinesPercept::LineOnField>::iterator line = lines.begin(); line != lines.end(); line++)
		{

			if(fabs(seg->p1.x * cos(line->alpha) + seg->p1.y * sin(line->alpha) - line->d) < params.maxLineSingleSegDist &&
				fabs(seg->p2.x * cos(line->alpha) + seg->p2.y * sin(line->alpha) - line->d) < params.maxLineSingleSegDist)
			{
				double firstToLast = (line->last - line->first).abs();
				const Vector2<int> segMid = (seg->p1 + seg->p2)/2;
				//CROSS("module:LinePerceptor:Lines2", segMid.x, segMid.y,30, 20, Drawings::ps_solid, ColorClasses::yellow);
				if( (firstToLast < (line->last - segMid).abs() || firstToLast < (line->first - segMid).abs()) )
				{
					//seg is not between first and last
					const double minToLine = (line->last - segMid).abs() > (line->first - segMid).abs() ? (line->first - segMid).abs() : (line->last - segMid).abs();

					//COMPLEX_DRAWING("module:LinePerceptor:Lines2",
					//	DRAWTEXT("module:LinePerceptor:Lines2", segMid.x+20, segMid.y, 10, ColorClasses::black, minToLine);
					//);

					if(minToLine > params.maxLineSingleSegDist2)
						continue;
					//CROSS("module:LinePerceptor:Lines2", segMid.x, segMid.y,30, 20, Drawings::ps_solid, ColorClasses::red);
				}
				else
					//CROSS("module:LinePerceptor:Lines2", segMid.x, segMid.y,30, 20, Drawings::ps_solid, ColorClasses::blue);

				line->segments.push_back(*seg);
				ttoDelete.push_back(seg);
				getFirstAndLastOfLine(*line, line->first, line->last, false);
				break;
			}
		}
	}
	for(std::vector<std::list<LinesPercept::LineSegment>::iterator>::iterator d = ttoDelete.begin(); d != ttoDelete.end(); d++)
		singleSegs.erase(*d);


	//delete lines which do not "hard cover" (length / sum(segemnts.length)) enough
	toDelete.clear();
	for(std::list<LinesPercept::LineOnField>::iterator l1 = lines.begin(); l1 != lines.end(); l1++)
	{
		int hardcover = 0;
		for(std::vector<LinesPercept::LineSegment>::iterator seg = l1->segments.begin(); seg != l1->segments.end(); seg++)
			hardcover += (seg->p1 - seg->p2).abs();
		if(hardcover/(double)(l1->first - l1->last).abs() < params.minHardcover)
			toDelete.push_back(l1);
	}
	for(std::vector<std::list<LinesPercept::LineOnField>::iterator>::iterator t = toDelete.begin(); t != toDelete.end(); t++)
	{
		for(std::vector<LinesPercept::LineSegment>::iterator seg = (*(*t)).segments.begin(); seg != (*(*t)).segments.end(); seg++)
			singleSegs.push_back(*seg);
		lines.erase(*t);
	}

	//find intersections
	for(std::list<LinesPercept::LineOnField>::iterator l1 = lines.begin(); l1 != lines.end(); l1++)
	{
		std::list<LinesPercept::LineOnField>::iterator l2 = l1;
		for(l2++; l2 != lines.end(); l2++)
		{
			double alphaDiff = l1->alpha - l2->alpha;
			while(alphaDiff < -pi_2)
				alphaDiff += pi;
			while(alphaDiff > pi_2)
				alphaDiff -= pi;
			if(fabs(alphaDiff) < params.minIntersectionAlphaDiff)
				continue;

			if((l1->first - l1->last).squareAbs() < sqr(params.minIntersectionLength) &&
				(l2->first - l2->last).squareAbs() < sqr(params.minIntersectionLength))
				continue;

			//zwei hessesche normaleformen gleichsetzen und aufloesen, dann kommt das bei raus
			const double zaehler = l1->d - (l2->d * cos(l1->alpha)/cos(l2->alpha)),
				nenner = sin(l1->alpha) - (sin(l2->alpha)*cos(l1->alpha)/cos(l2->alpha));
			const double y_s = zaehler/nenner,
				x_s = (l1->d - y_s * sin(l1->alpha))/cos(l1->alpha);

			if(y_s == y_s && x_s == x_s)//intersection exists -> not paralel || ident
			{
				const Vector2<int> s_p = Vector2<int>((int)x_s, (int)y_s);
				//this is some freay stuff which determines in which relation the
				//point s_p is to l1->first/last and l2->first/last given s_p is the
				//intersectionpoint of l1 and l2
				//distToLx = ( -min(dist(sp,last/first)) if in between,  )
				//           (  min(dist(s_p,last/first) else)           )
				double spToFirst = (s_p - l1->first).abs(),
					spToLast = (s_p - l1->last).abs(),
					firstToLast = (l1->first - l1->last).abs();
				double distToL1=0, distToL2=0;
				if(spToFirst < firstToLast && spToLast < firstToLast)
					//sp is between first and last
					distToL1 = - ( spToFirst > spToLast ? spToLast : spToFirst);
				else if(spToFirst >= firstToLast)
					//sp is closer to last
					distToL1 = spToLast;
				else if(spToLast >= firstToLast)
					//sp is closer to first
					distToL1 = spToFirst;
				else
					ASSERT(false);
				spToFirst = (s_p - l2->first).abs(),
					spToLast = (s_p - l2->last).abs(),
					firstToLast = (l2->first - l2->last).abs();
				if(spToFirst < firstToLast && spToLast < firstToLast)
					//sp is between first and last
					distToL2 = - ( spToFirst > spToLast ? spToLast : spToFirst);
				else if(spToFirst >= firstToLast)
					//sp is closer to last
					distToL2 = spToLast;
				else if(spToLast >= firstToLast)
					//sp is closer to first
					distToL2 = spToFirst;
				else
					ASSERT(false);
				//end freaky stuff

				LinesPercept::Intersection inter;
				inter.intersectionOnField = Vector2<int>((int)x_s, (int)y_s);
				Vector2<int> t1 = l1->first - l1->last,
					t2 = l2->first - l2->last;
				//this checks whether the intersection point is closer to first
				//or to last and if it is closer to first we need to flip the
				//direction
				if((l1->first - inter.intersectionOnField).squareAbs() < (l1->last - inter.intersectionOnField).squareAbs())
					t1 = l1->last - l1->first;
				if((l2->first - inter.intersectionOnField).squareAbs() < (l2->last - inter.intersectionOnField).squareAbs())
					t2 = l2->last - l2->first;
				//this is the heading of the intersection (to l1 and l2)
				Vector2<double> dirL1 = Vector2<double>(t1.x, t1.y).normalize(),
					dirL2 = Vector2<double>(t2.x, t2.y).normalize();


				if(distToL1 < -params.minTToEnd && distToL2 < -params.minTToEnd)
				{
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l1->last.x, l1->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l2->last.x, l2->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//this is a X
					inter.intersectionType = LinesPercept::Intersection::X;
					inter.dir1 = dirL1;
					inter.dir2 = dirL2;
					intersections.push_back(inter);
				}
				else if((distToL1 < -params.minTToEnd && distToL2 < params.maxTFromEnd) ||
					(distToL2 < -params.minTToEnd && distToL1 < params.maxTFromEnd))
				{
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l1->last.x, l1->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l2->last.x, l2->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//this is a T
					inter.intersectionType = LinesPercept::Intersection::T;
					if(distToL2 < -params.minTToEnd && distToL1 < params.maxTFromEnd)
					{
						//l2 is the intersected line (the upper part of the T)
						inter.dir1 = dirL1;
						inter.dir2 = dirL2;
					}
					else
					{
						//l1 is the intersected line (the upper part of the T)
						inter.dir1 = dirL2;
						inter.dir2 = dirL1;
					}
					intersections.push_back(inter);
				}
				else if(distToL1 < params.maxTFromEnd && distToL2 < params.maxTFromEnd)
				{
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l1->last.x, l1->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//ARROW("module:LinePerceptor:Intersections", x_s, y_s, l2->last.x, l2->last.y, 5, Drawings::ps_solid, ColorClasses::yellow);
					//this is a L
					inter.intersectionType = LinesPercept::Intersection::L;
					inter.dir1 = dirL1;
					inter.dir2 = dirL2;
					intersections.push_back(inter);
				}
			}
		}
	}

	//find "mittellinie"
	if(circle.found)
	{
		std::list<LinesPercept::LineOnField>::iterator closestLine;
		int minDist = -1;
		for(std::list<LinesPercept::LineOnField>::iterator l1 = lines.begin(); l1 != lines.end(); l1++)
		{
			const int dist = (int)fabs(l1->calculateDistToLine(circle.pos));
			if( dist < params.maxMidLineToCircleDist &&
				(dist < minDist || minDist == -1) &&
				(l1->first - l1->last).squareAbs() > sqr(params.minMidLineLength))
			{
				closestLine = l1;
				minDist = dist;
			}
		}

		if(minDist != -1)
		{
			closestLine->midLine = true;
			circle.pos = closestLine->calculateClosestPointOnLine(circle.pos);

			//intersections
			const Vector2<int> midLineDir = (closestLine->first - closestLine->last).normalize(params.centerCircleRadius + params.circleBiggerThanSpecified);
			LinesPercept::Intersection inter;

			inter.intersectionOnField = circle.pos + midLineDir;
			inter.dir1 = Vector2<double>(midLineDir.x, midLineDir.y).normalize();
			inter.dir2 = inter.dir1;
			inter.dir2.rotateLeft();
			inter.intersectionType = LinesPercept::Intersection::X;
			intersections.push_back(inter);

			inter.intersectionOnField = circle.pos - midLineDir;
			inter.dir1 = Vector2<double>(midLineDir.x, midLineDir.y).normalize();
			inter.dir2 = inter.dir1;
			inter.dir2.rotateLeft();
			inter.intersectionType = LinesPercept::Intersection::X;
			intersections.push_back(inter);
		}
	}
}

void LineSpecialist::analyzeSingleSegments(std::list<LinesPercept::LineSegment>& singleSegs, LinesPercept::CircleSpot& circle, std::list<LinesPercept::LineOnField>& lines)
{

	std::list<LinesPercept::CircleSpot> circleSpots;
	std::list<LinesPercept::CircleSpot> circleSpots2;
	LinesPercept::CircleSpot spot;

	std::list<LinesPercept::LineSegment>::iterator seg2;
	for(std::list<LinesPercept::LineSegment>::iterator seg = singleSegs.begin(); seg != singleSegs.end(); seg++)
	{
		//ARROW("module:LinePerceptor:CircleSpots", seg->p1.x, seg->p1.y, seg->p2.x, seg->p2.y, 20, Drawings::ps_solid, ColorClasses::robotBlue);

		const Vector2<int> seg_dir = seg->p1 - seg->p2;
		const Vector2<int> seg_mid = (seg->p1 + seg->p2)/2;
		const Vector2<int> seg_norm = Vector2<int>(seg_dir.x, seg_dir.y).rotateLeft();

		const Vector2<int> spot1 = seg_mid + (seg->p1-seg->p2).rotate(pi_2).normalize(params.centerCircleRadius+params.circleBiggerThanSpecified);
		spot.pos = spot1;
		spot.iterator = seg;
		//LINE("module:LinePerceptor:CircleSpots2", spot.pos.x, spot.pos.y, seg_mid.x, seg_mid.y, 5, Drawings::ps_solid, ColorClasses::yellow);
		//CROSS("module:LinePerceptor:CircleSpots2", spot.pos.x, spot.pos.y,20, 20, Drawings::ps_solid, ColorClasses::yellow);
		circleSpots2.push_back(spot);
		const Vector2<int> spot2 = seg_mid + (seg->p1-seg->p2).rotate(-pi_2).normalize(params.centerCircleRadius+params.circleBiggerThanSpecified);
		spot.pos = spot2;
		spot.iterator = seg;
		//LINE("module:LinePerceptor:CircleSpots2", spot.pos.x, spot.pos.y, seg_mid.x, seg_mid.y, 5, Drawings::ps_solid, ColorClasses::yellow);
		//CROSS("module:LinePerceptor:CircleSpots2", spot.pos.x, spot.pos.y,20, 20, Drawings::ps_solid, ColorClasses::yellow);
		circleSpots2.push_back(spot);

		if(seg_dir.squareAbs() < sqr(params.minSegmentLength) || (seg->p1Img - seg->p2Img).squareAbs() < sqr(params.minSegmentImgLength))
			continue;

		//LINE("module:LinePerceptor:CircleSpots", seg_mid.x, seg_mid.y, seg_mid.x + seg_norm.x, seg_mid.y + seg_norm.y, 20, Drawings::ps_solid, ColorClasses::orange);

		seg2 = seg;
		seg2++;
		for(; seg2 != singleSegs.end(); seg2++)
		{
			const Vector2<int> seg2_dir = seg2->p1 - seg2->p2;
			if(seg2_dir.squareAbs() < sqr(params.minSegmentLength) || (seg2->p1Img - seg2->p2Img).squareAbs() < sqr(params.minSegmentImgLength))
				continue;

			if((seg->p1 - seg2->p1).squareAbs() < sqr(params.maxNgbhDist) ||
				(seg->p1 - seg2->p2).squareAbs() < sqr(params.maxNgbhDist) ||
				(seg->p2 - seg2->p1).squareAbs() < sqr(params.maxNgbhDist) ||
				(seg->p2 - seg2->p2).squareAbs() < sqr(params.maxNgbhDist))
			{
				const Vector2<int> seg2_mid = (seg2->p1 + seg2->p2)/2;
				//LINE("module:LinePerceptor:CircleSpots", seg_mid.x, seg_mid.y, seg2_mid.x, seg2_mid.y, 20, Drawings::ps_solid, ColorClasses::red);
				const Vector2<int> seg2_norm = Vector2<int>(seg2_dir.x, seg2_dir.y).rotateLeft();

				const Vector2<int> p1 = seg_mid;
				const Vector2<int> p2 = seg_mid + seg_norm;
				const Vector2<int> p3 = seg2_mid;
				const Vector2<int> p4 = seg2_mid + seg2_norm;

				const int zaehler1 = (p4.x - p3.x)*(p1.y-p3.y) - (p4.y-p3.y)*(p1.x-p3.x);
				//const int zaehler1=(p4.x-p3.x)*(p1.y-p4.y)-(p4.y-p3.y)*(p1.x-p4.x);
				const int nenner1 = (p4.y-p3.y)*(p2.x-p1.x) - (p4.x-p3.x)*(p2.y-p1.y);
				double X1factor = zaehler1/(double)nenner1;

				const int zaehler2 = (p2.x-p1.x)*(p1.y-p3.y) - (p2.y-p1.y)*(p1.x-p3.x);
				//const int zaehler2=(p2.x-p1.x)*(p2.y-p3.y)-(p2.y-p1.y)*(p2.x-p3.x);
				const int nenner2 = (p4.y-p3.y)*(p2.x-p1.x) - (p4.x-p3.x)*(p2.y-p1.y);
				const double X2factor = zaehler2/(double)nenner2;

				const Vector2<int> t = p2 - p1;
				const Vector2<int> inter = p1 + Vector2<int>((int)(t.x*X1factor), (int)(t.y*X1factor));

				if(fabs(fabs(seg_norm.abs() * X1factor) - params.centerCircleRadius+params.circleBiggerThanSpecified) > params.maxRadiusError || fabs(fabs(seg2_norm.abs() * X2factor) - params.centerCircleRadius+params.circleBiggerThanSpecified) > params.maxRadiusError)
					continue;

				const int X1Sign = X1factor > 0 ? 1 : -1;
				const int X2Sign = X2factor > 0 ? 1 : -1;
				const Vector2<int> i1 = seg_mid + Vector2<int>(seg_norm).normalize((params.centerCircleRadius+params.circleBiggerThanSpecified) * X1Sign);
				const Vector2<int> i2 = seg2_mid + Vector2<int>(seg2_norm).normalize((params.centerCircleRadius+params.circleBiggerThanSpecified) * X2Sign);

				//CROSS("module:LinePerceptor:CircleSpots", inter.x, inter.y, 40, 20, Drawings::ps_solid, ColorClasses::red);
				//CROSS("module:LinePerceptor:CircleSpots", i1.x, i1.y, 40, 20, Drawings::ps_solid, ColorClasses::blue);
				//CROSS("module:LinePerceptor:CircleSpots", i2.x, i2.y, 40, 20, Drawings::ps_solid, ColorClasses::blue);
				//LINE("module:LinePerceptor:CircleSpots", seg_mid.x, seg_mid.y, inter.x, inter.y, 10, Drawings::ps_solid, ColorClasses::orange);
				//LINE("module:LinePerceptor:CircleSpots", seg2_mid.x, seg2_mid.y, inter.x, inter.y, 10, Drawings::ps_solid, ColorClasses::orange);
				spot.pos = i1;
				circleSpots.push_back(spot);
				spot.pos = i2;
				circleSpots.push_back(spot);
			}
		}
	}

	//Hough Transformation fuer (ganz) arme ;-)
	for(std::list<LinesPercept::CircleSpot>::iterator spot_iter = circleSpots.begin(); spot_iter != circleSpots.end(); spot_iter++)
	{
		spot = *spot_iter;
		Vector2<int> center(0,0);

		std::vector<std::list<LinesPercept::CircleSpot>::iterator> supporters;

		for(std::list<LinesPercept::CircleSpot>::iterator other = circleSpots.begin(); other != circleSpots.end(); other++)
		{
			if((other->pos - spot.pos).squareAbs() < sqr(params.maxSupporterDist))
			{
				supporters.push_back(other);
				center += other->pos;
			}
		}

		if((int)supporters.size() >= params.minCircleSupporters)
		{
			center /= supporters.size();

			//collect second round of supporters
			for(std::list<LinesPercept::CircleSpot>::iterator other = circleSpots2.begin(); other != circleSpots2.end(); other++)
				if((other->pos - center).squareAbs() < sqr(params.maxSupporterDist2))
					singleSegs.erase(other->iterator);

			if (center.x<4000)
			{
				circle.pos = center;
				circle.found = true;
			}

			//circle.lastSeen = theFrameInfo.time;
			//CIRCLE("module:LinePerceptor:CircleSpots", circle.pos.x, circle.pos.y, circleParams.maxSupporterDist, 30, Drawings::ps_solid, ColorClasses::blue, Drawings::bs_null, ColorClasses::robotBlue);
			//CIRCLE("module:LinePerceptor:CircleSpots", circle.pos.x, circle.pos.y, circleParams.maxSupporterDist2, 30, Drawings::ps_solid, ColorClasses::blue, Drawings::bs_null, ColorClasses::robotBlue);
			break;
		}
	}

	//a single segment is assumed to be a line if it's size is sufficent (and it's not part of the circle)
	std::vector<std::list<LinesPercept::LineSegment>::iterator> toDelete;
	for(std::list<LinesPercept::LineSegment>::iterator seg = singleSegs.begin(); seg != singleSegs.end(); seg++)
	{
		if((seg->p1 - seg->p2).squareAbs() > sqr(params.minLineSingleRegionLength))
		{
			LinesPercept::LineOnField l;
			l.d = seg->d;
			l.alpha = seg->alpha;
			l.segments.push_back(*seg);
			l.dead = false;
			l.midLine = false;
			lines.push_back(l);
			toDelete.push_back(seg);
		}
	}
	for(std::vector<std::list<LinesPercept::LineSegment>::iterator>::iterator d = toDelete.begin(); d != toDelete.end(); d++)
		singleSegs.erase(*d);
}

void LineSpecialist::createBanSectors(const CameraMatrix& cameraMatrix)
{
	for(std::vector<LineElements::NonLineElement>::const_iterator nonLineElement = nonLineElements.begin(); nonLineElement != nonLineElements.end(); nonLineElement++)
	{
		//const Vector2<double> p1_cor = theImageCoordinateSystem.toCorrected(spot->p1),
		//	p2_cor = theImageCoordinateSystem.toCorrected(spot->p2);
		Vector2<double> pf1, pf2;
		if(!Geometry::calculatePointOnField((int)nonLineElement->p1.x, (int)nonLineElement->p1.y, cameraMatrix, pf1))
		{
			//CROSS("module:LinePerceptor:LineSegmentsImg", ((spot->p1+spot->p2)/2).x, ((spot->p1+spot->p2)/2).y,4, 2, Drawings::ps_solid, ColorClasses::orange);
			//continue;
		}
		if(!Geometry::calculatePointOnField((int)nonLineElement->p2.x, (int)nonLineElement->p2.y, cameraMatrix, pf2))
		{
			pf2 = pf1 * 100;

			//CROSS("module:LinePerceptor:LineSegmentsImg", ((spot->p1+spot->p2)/2).x, ((spot->p1+spot->p2)/2).y,4, 2, Drawings::ps_solid, ColorClasses::orange);
			//continue;
		}

		//ARROW("module:LinePerceptor:NonLineSegments", pf1.x, pf1.y, pf2.x, pf2.y, 30, Drawings::ps_solid, ColorClasses::orange);

		double alpha = (pf1.angle() + pf2.angle()) / 2;

		while(alpha > pi_2)
			alpha -= pi;
		while(alpha < -pi_2)
			alpha += pi;

		const int dist = (int)pf1.abs();
		const int dist2 = (int)pf2.abs();

		for(std::list<BanSector>::iterator s = banSectors.begin(); s != banSectors.end(); s++)
		{
			if(s->alphaLeft < alpha && s->alphaRight > alpha)
			{
				if((dist > s->start && dist < s->end) ||
					(dist2 > s->start && dist2 < s->end))
				{
					if(s->start > dist)
						s->start = dist;
					if(s->end < dist2)
						s->end = dist2;
					if(alpha - params.angleStepSize < s->alphaLeft)
						s->alphaLeft = alpha - params.angleStepSize;
					if(alpha + params.angleStepSize > s->alphaRight)
						s->alphaRight = alpha + params.angleStepSize;
					s->counter++;
					goto continueOuter;
				}
			}
		}
		BanSector sector;
		sector.alphaLeft = alpha - params.angleStepSize;
		sector.alphaRight = alpha + params.angleStepSize;
		sector.start = dist;
		sector.end = dist2;
		sector.counter = 1;
		banSectors.push_back(sector);
continueOuter:
		;
	}

	for(std::list<BanSector>::iterator s = banSectors.begin(); s != banSectors.end(); s++)
	{
		//COMPLEX_DRAWING("module:LinePerceptor:banSectors",
		//	Vector2<int> p1((int)(cos(s->alphaLeft) * s->start), (int)(sin(s->alphaLeft) * s->start));
		//Vector2<int> p2((int)(cos(s->alphaRight) * s->start), (int)(sin(s->alphaRight) * s->start));
		//Vector2<int> p3((int)(cos(s->alphaLeft) * s->end), (int)(sin(s->alphaLeft) * s->end));
		//Vector2<int> p4((int)(cos(s->alphaRight) * s->end), (int)(sin(s->alphaRight) * s->end));
		//LINE("module:LinePerceptor:banSectors", p1.x, p1.y, p2.x, p2.y, 20, Drawings::ps_solid, ColorClasses::blue);
		//LINE("module:LinePerceptor:banSectors", p1.x, p1.y, p3.x, p3.y, 20, Drawings::ps_solid, ColorClasses::blue);
		//LINE("module:LinePerceptor:banSectors", p2.x, p2.y, p4.x, p4.y, 20, Drawings::ps_solid, ColorClasses::blue);
		//LINE("module:LinePerceptor:banSectors", p3.x, p3.y, p4.x, p4.y, 20, Drawings::ps_solid, ColorClasses::blue);
		//);

	}

	std::list<BanSector>::iterator nexts2;
	for(std::list<BanSector>::iterator s1 = banSectors.begin(); s1 != banSectors.end(); s1++)
	{
		nexts2 = s1;
		nexts2++;
		for(std::list<BanSector>::iterator s2 = banSectors.begin(); s2 != banSectors.end(); s2 = nexts2)
		{
			nexts2 = s2;
			nexts2++;

			if(s2 == s1)
				continue;

			if((s1->alphaLeft > s2->alphaLeft && s1->alphaLeft < s2->alphaRight) ||
				(s1->alphaRight > s2->alphaLeft && s1->alphaRight < s2->alphaRight) ||
				(s2->alphaLeft > s1->alphaLeft && s2->alphaLeft < s1->alphaRight) ||
				(s2->alphaRight > s1->alphaLeft && s2->alphaRight < s1->alphaRight) )
			{
				if((s1->start > s2->start && s1->start < s2->end) || 
					(s2->start > s1->start && s2->start < s1->end))
				{
					if(s2->alphaLeft < s1->alphaLeft) 
						s1->alphaLeft = s2->alphaLeft;
					if(s2->alphaRight > s1->alphaRight) 
						s1->alphaRight = s2->alphaRight;
					if(s2->start < s1->start)
						s1->start = s2->start;
					if(s2->end > s1->end)
						s1->end = s2->end;
					s1->counter += s2->counter;
					nexts2 = banSectors.erase(s2);
				}
			}
		}
	}

	std::list<BanSector>::iterator nexts;
	for(std::list<BanSector>::iterator s = banSectors.begin(); s != banSectors.end(); s = nexts)
	{
		nexts = s;
		nexts++;
		if(s->counter < params.minSectorCounter)
		{
			//COMPLEX_DRAWING("module:LinePerceptor:banSectors",
			//	Vector2<int> p1((int)(cos(s->alphaLeft) * s->start), (int)(sin(s->alphaLeft) * s->start));
			//Vector2<int> p2((int)(cos(s->alphaRight) * s->start), (int)(sin(s->alphaRight) * s->start));
			//Vector2<int> mid = (p1 + p2)/2;
			//CROSS("module:LinePerceptor:banSectors", mid.x, mid.y, 80, 40, Drawings::ps_solid, ColorClasses::red);
			//);
			nexts = banSectors.erase(s);
			continue;
		}

		//COMPLEX_DRAWING("module:LinePerceptor:banSectors",
		//	Vector2<int> p1((int)(cos(s->alphaLeft) * s->start), (int)(sin(s->alphaLeft) * s->start));
		//Vector2<int> p2((int)(cos(s->alphaRight) * s->start), (int)(sin(s->alphaRight) * s->start));
		//Vector2<int> p3((int)(cos(s->alphaLeft) * s->end), (int)(sin(s->alphaLeft) * s->end));
		//Vector2<int> p4((int)(cos(s->alphaRight) * s->end), (int)(sin(s->alphaRight) * s->end));
		//LINE("module:LinePerceptor:banSectors", p1.x, p1.y, p2.x, p2.y, 20, Drawings::ps_dash, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectors", p1.x, p1.y, p3.x, p3.y, 20, Drawings::ps_dash, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectors", p2.x, p2.y, p4.x, p4.y, 20, Drawings::ps_dash, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectors", p3.x, p3.y, p4.x, p4.y, 20, Drawings::ps_dash, ColorClasses::red);
		//Vector2<int> pmid = (p1 + p2)/2 + p3/2;
		//DRAWTEXT("module:LinePerceptor:banSectors", pmid.x, pmid.y, 150, ColorClasses::black, s->counter);

		///*Vector2<double> p1cor = theImageCoordinateSystem.fromCorrectedApprox(p1);
		//Vector2<double> p2cor = theImageCoordinateSystem.fromCorrectedApprox(p2);
		//Vector2<double> p3cor = theImageCoordinateSystem.fromCorrectedApprox(p3);
		//Vector2<double> p4cor = theImageCoordinateSystem.fromCorrectedApprox(p4);*/
		//Vector2<double> p1cor = Vector2<double>(p1.x, p1.y);
		//Vector2<double> p2cor = Vector2<double>(p2.x, p2.y);
		//Vector2<double> p3cor = Vector2<double>(p3.x, p3.y);
		//Vector2<double> p4cor = Vector2<double>(p4.x, p4.y);

		//Vector2<int> p1Img;
		//Vector2<int> p2Img;
		//Vector2<int> p3Img;
		//Vector2<int> p4Img;

		//Geometry::calculatePointInImage(Vector2<int>((int)p1cor.x, (int)p1cor.y), theCameraMatrix, theCameraInfo, p1Img);
		//Geometry::calculatePointInImage(Vector2<int>((int)p2cor.x, (int)p2cor.y), theCameraMatrix, theCameraInfo, p2Img);
		//Geometry::calculatePointInImage(Vector2<int>((int)p3cor.x, (int)p3cor.y), theCameraMatrix, theCameraInfo, p3Img);
		//Geometry::calculatePointInImage(Vector2<int>((int)p4cor.x, (int)p4cor.y), theCameraMatrix, theCameraInfo, p4Img);

		//LINE("module:LinePerceptor:banSectorsImg", p1Img.x, p1Img.y, p2Img.x, p2Img.y, 2, Drawings::ps_dot, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectorsImg", p1Img.x, p1Img.y, p3Img.x, p3Img.y, 2, Drawings::ps_dot, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectorsImg", p2Img.x, p2Img.y, p4Img.x, p4Img.y, 2, Drawings::ps_dot, ColorClasses::red);
		//LINE("module:LinePerceptor:banSectorsImg", p3Img.x, p3Img.y, p4Img.x, p4Img.y, 2, Drawings::ps_dot, ColorClasses::red);
		//);
	}

}
void LineSpecialist::sendCircleResult()
{
  netCircle *netVision;
  netVision=new netCircle();
  netVision->CircleSeen=theLinesPercept->circleCenter.found;
  netVision->CircleCenterX=(int32)100*(theLinesPercept->circleCenter.pos.x);
  netVision->CircleCenterY=(int32)100*(theLinesPercept->circleCenter.pos.y);
  selfMessageQueue->SetMessageData(idCircle,idRobotThread,idNetworkThread,
	  sizeof(netCircle),(char*)netVision);
  delete netVision;
}
void LineSpecialist::outLinesPercept()
{
#ifdef TJClassfiedPic
// 	selfMessageQueue->SetMessageData(idClassifyImage,idRobotThread,idNetworkThread,
// 		IMAGE_HEIGHT*IMAGE_WIDTH,(char*)theTJImage->classifiedPic);
	//sendCircleResult();
#endif
#ifdef TJPIC
	sendCircleResult();
#endif

}
void LineSpecialist::Parameters::massAssign()
{
	ASSIGN(maxLineNghbGreySkip);
	ASSIGN(minLineSize);
	ASSIGN(minLineSegmentCount);
	ASSIGN(minLineSingleSegmentSize);
	ASSIGN(maxLineNghbNoneSize);
	ASSIGN(maxLineNghbNoneRatio);
	ASSIGN(minLineNghbGreenSideSize);
	ASSIGN(minLineNghbGreenAboveSize);
	ASSIGN(minLineNghbGreenBelowSize);
	ASSIGN(minRobotRegionSize);
	ASSIGN(maxRobotRegionAlphaDiff);
	ASSIGN(minRobotWidthRatio);
	ASSIGN(angleStepSize);
	ASSIGN(minSectorCounter);
	ASSIGN(minWidthRatio);
	ASSIGN(maxLineDistance);
	ASSIGN(maxAlphaDiff);
	ASSIGN(minLineLength);
	ASSIGN(maxLineAngleDiff);
	ASSIGN(maxDDiff);
	ASSIGN(minSupporters);
	ASSIGN(minLineStartLength);
	ASSIGN(circleBiggerThanSpecified);
	ASSIGN(minSegmentLength);
	ASSIGN(minSegmentImgLength);
	ASSIGN(maxNgbhDist);
	ASSIGN(centerCircleRadius);
	ASSIGN(maxRadiusError);
	ASSIGN(maxSupporterDist);
	ASSIGN(maxSupporterDist2);
	ASSIGN(minLineSingleRegionLength);
	ASSIGN(maxLineCircleDist);
	ASSIGN(maxLineUniteDist);
	ASSIGN(maxLineUniteAlphaDiff);
	ASSIGN(maxLineSingleSegDist);
	ASSIGN(maxLineSingleSegDist2);
	ASSIGN(minHardcover);
	ASSIGN(minIntersectionAlphaDiff);
	ASSIGN(minIntersectionLength);
	ASSIGN(minTToEnd);
	ASSIGN(maxTFromEnd);
	ASSIGN(maxMidLineToCircleDist);
	ASSIGN(minMidLineLength);
	ASSIGN(minEccentricityOfBallRegion);
	ASSIGN(maxAngleDiff);
	ASSIGN(regionMaxSize);
	ASSIGN(gridStepSize);
	ASSIGN(skipOffset);
	ASSIGN(minCircleSupporters);
	ASSIGN(maxLineWidth);
}


MAKE_KS(LineSpecialist);
