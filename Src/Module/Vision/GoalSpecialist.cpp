#include "GoalSpecialist.h"
#include "Module/Vision/DebugDrawings.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
GoalSpecialist::GoalSpecialist():lastOdometry(*theOdometryData),goaldebugcounter(0)
{

}
GoalSpecialist::~GoalSpecialist()
{

}
/*@
main function of goal detection
@para goalColor  the goal post's colour
@polePercepts[]  the blob for detecting goal
@*/
void GoalSpecialist::detectGoal(unsigned char goalColor, blob polePercepts[])
{
	// Initialize two new percepts

	blob polePercept1;
	polePercept1.color = goalColor;
	polePercept1.yImageTop = IMAGE_HEIGHT-1;
	blob  polePercept2;
	polePercept2.color = goalColor;
	polePercept2.yImageTop = IMAGE_HEIGHT-1;
	polePercepts[0] = polePercept1;
	polePercepts[1] = polePercept2;
	owngoalpercepts[0] = polePercept1;
	owngoalpercepts[1] = polePercept2;
	oppgoalpercepts[0] = polePercept1;
	oppgoalpercepts[1] = polePercept2;

	// build accumulator of horizontal segment percepts
	std::vector<int> verticalAcc (IMAGE_WIDTH, 0);
	for(unsigned int i=0; i<theRegionpercept->HorizontalRun.size(); i++)
	{
		Run segment = theRegionpercept->HorizontalRun[i];
		if(segment.colour == goalColor && segment.length < 100)
		{
			for(int i=segment.start.x; i<segment.start.x+segment.length; i++)
				verticalAcc[i]++;
		}
	}

	// find best x-coordinate from accumulator
	for(int p=0; p<MAX_NUMBER_OF_POST; p++)
	{
		for(int j=0; j<(int)IMAGE_WIDTH; j++)
		{
			int points = verticalAcc[j];
			if(points > 4)
			{
				if(points >= PoleAccThreshold && points >= polePercepts[p].accumulatorMatchSize)
				{
					if(p==0 || abs(j-polePercepts[0].xImage) >=PoleAccMinDistance)
					{
						polePercepts[p].xImage = j;
						polePercepts[p].xImageTop = j;
						polePercepts[p].xImageBottom = j;
						polePercepts[p].accumulatorMatchSize = points;
					}
				}
			}
		}
	}

	// Calculate distance to horizon
	/*int yHorizon = theImageCoordinateSystem.fromHorizonBased(Vector2<double>()).y,
	yHorizonClipped = std::min(theImage.cameraInfo.resolutionHeight - 1, std::max(0, yHorizon)),
	yHorizonCorrected = (int) theImageCoordinateSystem.toCorrected(Vector2<int>(0, yHorizonClipped)).y;

	yHorizon += yHorizonClipped - yHorizonCorrected;*/
	int yHorizon=int(theRegionpercept->horizonline.base.y);
	yHorizon = std::max<>(0,yHorizon);

	int numberOfPercepts = 0;
	if(polePercepts[0].xImage != 0)
		numberOfPercepts++;
	if(polePercepts[1].xImage != 0)
		numberOfPercepts++;

	for(int p=0; p<numberOfPercepts; p++)
	{
		int segmentsOnPole = 0;
		std::vector<Run> segmentsOnPoleVector;
		for(int i=theRegionpercept->HorizontalRun.size()-1; i>=0; i--)//从栈末开始做即从最下面的Run开始
		{
			Run segment = theRegionpercept->HorizontalRun[i];
			if(segment.colour == goalColor)
			{
				// from all segments with intersections find the segment with greatest and lowest y-coordinate
				// calculate percentage of coverage with correct colored segments
				if((segment.start.x <= polePercepts[p].xImage)
					&& (segment.start.x + segment.length >= polePercepts[p].xImage))
				{
					//segmentsOnPoleVector.push_back(segment);
					//DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::yellow);
					//switch(goalColor)
					//{
					//	//case YELLOW: DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);
					//case CYAN : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::yellow);break;
					//case YELLOW : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);break;
					//}
					//segmentsOnPole++;
					if(segment.start.y> polePercepts[p].yImageBottom && (polePercepts[p].yImageBottom == 0 || (segment.start.y- polePercepts[p].yImageBottom) <= 24))//防止距离很远的run被当做门柱内的run
					{
						polePercepts[p].yImageBottom = segment.start.y;
						segmentsOnPoleVector.push_back(segment);
						segmentsOnPole++;
						switch(goalColor)
						{
							//case YELLOW: DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);
						case CYAN : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::yellow);break;
						case YELLOW : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);break;
						}
					}
					if((segment.start.y< polePercepts[p].yImageTop&&polePercepts[p].yImageTop-segment.start.y<=24)||polePercepts[p].yImageTop==IMAGE_HEIGHT-1)
					{
						polePercepts[p].yImageTop = segment.start.y;
						segmentsOnPoleVector.push_back(segment);
						segmentsOnPole++;
						switch(goalColor)
						{
							//case YELLOW: DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);
						case CYAN : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::yellow);break;
						case YELLOW : DRAW_LINE(segment.start.x,segment.start.y,segment.end.x,segment.end.y,1,Drawings::ps_SolidLine,Drawings::cyan);break;
						}
					}

				}
			}
		}
		DRAW_BIG_DOT(polePercepts[p].xImage,polePercepts[p].yImageTop,Drawings::black,Drawings::darkGreen);
		DRAW_BIG_DOT(polePercepts[p].xImage,polePercepts[p].yImageBottom,Drawings::black,Drawings::darkGreen);
		DRAW_LINE(polePercepts[p].xImage,polePercepts[p].yImageTop,polePercepts[p].xImage,polePercepts[p].yImageBottom,1,Drawings::ps_DashDotLine,Drawings::black);
		double segmentsOnPoleMax = (double) (polePercepts[p].yImageBottom - polePercepts[p].yImageTop) / GridStepSize+1;
		if((segmentsOnPole / segmentsOnPoleMax) < 0.8)
		{
			polePercepts[p].discarded = true;
			continue; // next percept
		}

		// find the most accurate footpoint (look into the image for this because it could not be on a scanline)
		int xCorrection = 0;
		while(polePercepts[p].yImageBottom + 1 < IMAGE_HEIGHT
			&& polePercepts[p].xImage + xCorrection > 2
			&& polePercepts[p].xImage + xCorrection + 2 < IMAGE_WIDTH)
		{
			if(convertColorAt(polePercepts[p].xImage + xCorrection, polePercepts[p].yImageBottom+1) == goalColor)
			{
				polePercepts[p].yImageBottom++;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection + 1, polePercepts[p].yImageBottom+1) == goalColor)
			{
				polePercepts[p].yImageBottom++;
				xCorrection++;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection + 2, polePercepts[p].yImageBottom+1) == goalColor)
			{
				polePercepts[p].yImageBottom++;
				xCorrection+=2;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection - 1, polePercepts[p].yImageBottom+1) == goalColor)
			{
				polePercepts[p].yImageBottom++;
				xCorrection--;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection - 2, polePercepts[p].yImageBottom+1) == goalColor)
			{
				polePercepts[p].yImageBottom++;
				xCorrection-=2;
				continue;
			}
			break;
		}

		polePercepts[p].xImageBottom += xCorrection;

		int toRight = 0;
		while(polePercepts[p].xImageBottom + toRight < IMAGE_WIDTH
			&& convertColorAt(polePercepts[p].xImageBottom + toRight + 1, polePercepts[p].yImageBottom) == goalColor)
		{
			toRight++;
		}

		int toLeft = 0;
		while(polePercepts[p].xImageBottom + toLeft < IMAGE_WIDTH
			&& convertColorAt(polePercepts[p].xImageBottom + toLeft - 1, polePercepts[p].yImageBottom) == goalColor)
		{
			toLeft--;
		}

		polePercepts[p].xImageBottom += (toRight + toLeft)/2;

		if(polePercepts[p].yImageBottom >= IMAGE_HEIGHT - pixelsToTreatPoleAsNear
			|| polePercepts[p].xImageBottom <= 2
			|| polePercepts[p].xImageBottom >= IMAGE_WIDTH- 2)
			polePercepts[p].footVisible = false;

		// check whether percepted pole footpoint is below horizon
		if(polePercepts[p].yImageBottom < yHorizon - 5) // TODO确定柱子底与水平线的位置关系
		{
			polePercepts[p].discarded = true;
			continue; // next percept
		}

		// find more accurate top y-coordinate
		xCorrection = 0;
		while(polePercepts[p].yImageTop > 0
			&& polePercepts[p].xImage + xCorrection > 2
			&& polePercepts[p].xImage + xCorrection + 2 < IMAGE_WIDTH)
		{
			if(convertColorAt(polePercepts[p].xImage + xCorrection, polePercepts[p].yImageTop-1) == goalColor)
			{
				polePercepts[p].yImageTop--;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection + 1, polePercepts[p].yImageTop-1) == goalColor)
			{
				polePercepts[p].yImageTop--;
				xCorrection++;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection + 2, polePercepts[p].yImageTop-1) == goalColor)
			{
				polePercepts[p].yImageTop--;
				xCorrection+=2;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection - 1, polePercepts[p].yImageTop-1)  == goalColor)
			{
				polePercepts[p].yImageTop--;
				xCorrection--;
				continue;
			}
			if(convertColorAt(polePercepts[p].xImage + xCorrection - 2, polePercepts[p].yImageTop-1) == goalColor)
			{
				polePercepts[p].yImageTop--;
				xCorrection-=2;
				continue;
			}

			break;
		}

		polePercepts[p].xImageTop += xCorrection;
		if( polePercepts[p].yImageTop <= 2)//当柱子上面太靠近图片边缘时就认为顶部不在图片中了
		{
			polePercepts[p].headVisible = false;
		}
		if(polePercepts[p].headVisible&&polePercepts[p].yImageTop > yHorizon+5)//确定柱子顶与水平线的关系
		{
			polePercepts[p].discarded=true;
			continue;
		}

		// calculate width of pole
		int leftScanned = 0;
		int rightScanned = 0;

		int y = (polePercepts[p].yImageBottom - polePercepts[p].yImageTop) / 2 + polePercepts[p].yImageTop;
		// scan to left
		for(int x = polePercepts[p].xImage; x >= 0 && x >= polePercepts[p].xImage - 75; x--)
		{
			if(convertColorAt(x, y) == goalColor)
				leftScanned++;
		}
		// scan to right
		for(int x = polePercepts[p].xImage; x < IMAGE_WIDTH&& x <= polePercepts[p].xImage + 75; x++)
		{
			if(convertColorAt(x, y) == goalColor)
				rightScanned++;
		}

		polePercepts[p].width = leftScanned + rightScanned;//从柱子中部确定柱子的宽度

		//Todo1:通过expectedWidth判断是否为门柱有待提高计算精度，精度提高可以采用
		int expectedWidth;
		//int expectedWidth = calculateExpectedPixelWidth(Vector2<int>(polePercepts[p].xImageBottom, polePercepts[p].yImageBottom),*theCameraMatrix);
		if(polePercepts[p].headVisible&&polePercepts[p].footVisible)
		{
			expectedWidth=(polePercepts[p].yImageBottom-polePercepts[p].yImageTop)*GoalPostWidthInMM/GoalHeightInMM;
		}



		else  expectedWidth=polePercepts[p].width;//calculateExpectedPixelWidth(Vector2<int>(polePercepts[p].xImageBottom, polePercepts[p].yImageBottom),*theCameraMatrix);
		//std::cout<<"球门柱反算的宽度="<<expectedWidth<<std::endl;
		//std::cout<<"图片中的门柱宽度="<<polePercepts[p].width<<std::endl;

		if(goalColor == CYAN||goalColor==YELLOW)
		{
			int maxAcceptedWidth = (int)(expectedWidth * 1.7 + 0.5);
			int minAcceptedWidth = (int)(expectedWidth * 0.6 + 0.5);
			int segmentsOnPoleVectorAccepted = 0;

			for(unsigned int i=0; i<segmentsOnPoleVector.size(); i++)
			{
				Run segment = segmentsOnPoleVector[i];
				if(segment.length >= minAcceptedWidth && segment.length <= maxAcceptedWidth)
					segmentsOnPoleVectorAccepted++;
			}

			if(segmentsOnPoleVectorAccepted / ((double) segmentsOnPoleMax) < 0.6)
			{
				polePercepts[p].discarded = true;
				continue; // next percept
			}
		}


		int firstGreen = IMAGE_HEIGHT-1;
		for(int i=-1; i<2; i++)
		{
			int x = polePercepts[p].xImageBottom + i * polePercepts[p].width/ 2;
			if(x >= 0 && x <IMAGE_WIDTH)
			{
				int firstGreenCurrentScan = findDown(x, polePercepts[p].yImageBottom, 	FIELD, IMAGE_HEIGHT);
				if(firstGreenCurrentScan < firstGreen)
					firstGreen = firstGreenCurrentScan;
			}
		}
		//当foot接近图片底部时，认为柱子是close也将坐标返回给定位
		if( polePercepts[p].footVisible&&firstGreen > polePercepts[p].yImageBottom + 10)//绿色离底很远时当做假//|| firstGreen == IMAGE_HEIGHT-1)//当柱子底在图片中不可见时
		{
			polePercepts[p].discarded = true;
			continue; // next percept
		}

		Geometry::calculatePointOnField(polePercepts[p].xImageBottom, polePercepts[p].yImageBottom,*theCameraMatrix,polePercepts[p].onField);
		Vector2<double> lAddon(GoalPostWidthInMM/2.0, 0);
		lAddon.rotate(polePercepts[p].onField.angle());
		polePercepts[p].onField += lAddon;
		//std::cout<<"distance~~~~"<<polePercepts[p].onField.abs()<<std::endl;
		//当整根柱子都可以看到时，距离可以用高来算
		if(!polePercepts[p].discarded && polePercepts[p].headVisible && polePercepts[p].footVisible )
		{
			const blob& pp = polePercepts[p];
			Vector2<double> anglesBottom;
			Geometry::calculateAnglesForPoint(Vector2<int>(pp.xImageBottom, pp.yImageBottom), *theCameraMatrix, anglesBottom);
			Vector2<double> anglesTop;
			Geometry::calculateAnglesForPoint(Vector2<int>(pp.xImageTop, pp.yImageTop), *theCameraMatrix,  anglesTop);
			if(anglesTop.y != anglesBottom.y && polePercepts[p].onField.abs())
			{
				double distanceCorrected = GoalHeightInMM / (tan(anglesTop.y) - tan(anglesBottom.y));
				//double distanceByHeight=getGoalPostDistFromHeight(pp.yImageBottom-pp.yImageTop,GoalHeightInMM);
				//std::cout<<"distanceCorrected~~~~~~~~~"<<distanceCorrected<<std::endl;;
				//std::cout<<"distanceByHeight~~~~~~~~~~"<<distanceByHeight<<std::endl;
				Vector2<double> addon (distanceCorrected - polePercepts[p].onField.abs(), 0);
				addon.rotate(polePercepts[p].onField.angle());
				polePercepts[p].onField+=addon;
				//std::cout<<"distance 最后"<<polePercepts[p].onField.abs()<<std::endl;
				polePercepts[p].distanceType = GoalPost::HEIGHT_BASED;
			}

		}
		if(polePercepts[p].onField.x>=7000) polePercepts[p].discarded=true;

		if(polePercepts[p].xImage != 0 && !polePercepts[p].footVisible)
			polePercepts[p].distanceType = GoalPost::IS_CLOSER;

		DRAW_LINE(polePercepts[p].xImageTop,polePercepts[p].yImageTop,polePercepts[p].xImageBottom,polePercepts[p].yImageBottom,1,Drawings::ps_SolidLine,Drawings::blue);

	}

	// Todo2:discard misplaced poles 用柱子上下的坐标反算距离，确定是否为实际的柱子，等实际机器人回来测试

	//for(int p = 0; p < numberOfPercepts; ++p)
	//{
	//	if(!polePercepts[p].discarded)
	//	{
	//		double distance = polePercepts[p].onField.abs();
	//		double distanceByHead, distanceByFoot;
	//		bool headOverHorizon, footUnderHorizon;
	//		//std::cout<<"~~~~~~distance~~~~~~"<<distance<<std::endl;
	//		{
	//		 Vector3<double> field;
	//		//const Vector2<>& corrected(theImageCoordinateSystem.toCorrected(Vector2<int>(polePercepts[p].xImageTop, polePercepts[p].yImageTop)));
	//		 headOverHorizon = Geometry::calculatePointOnField(polePercepts[p].xImageTop,polePercepts[p].yImageTop, GoalHeightInMM, *theCameraMatrix, field);
	//		 Vector2<double> Field=Vector2<double>(field.x,field.y);
	//		 distanceByHead = Field.abs();
	//		 //std::cout<<"~~~~~distanceByHead~~~~~"<<distanceByHead<<std::endl;
	//		}
	//		//{  Vector3<double> field;
	//		////const Vector2<>& corrected(theImageCoordinateSystem.toCorrected(Vector2<int>(polePercepts[p].xImageBottom, polePercepts[p].yImageBottom)));
	//		//footUnderHorizon = Geometry::calculatePointOnField(polePercepts[p].xImageBottom, polePercepts[p].yImageBottom,0.0, *theCameraMatrix, field);
	//		// Vector2<double> Field=Vector2<double>(field.x,field.y);
	//		//distanceByFoot =Field.abs();
	//		//std::cout<<"~~~~~~~distanceByFoot~~~~~~~"<<distanceByFoot<<std::endl;
	//		//}

	//		if(polePercepts[p].headVisible && !headOverHorizon)
	//		{
	//			polePercepts[p].discarded2 = true;
	//			//std::cout<<"Head see,not above horizon"<<std::endl;
	//		}


	//		if(polePercepts[p].headVisible && headOverHorizon && (fabs(distance - distanceByHead) / std::max<>(distance, distanceByHead)) > allowedDistanceNoise)
	//		{
	//			polePercepts[p].discarded2 = true;
	//			//std::cout<<"head see,over horizon, 比例="<<fabs(distance - distanceByHead) / std::max<>(distance, distanceByHead)<<std::endl;
	//		}
	//           //	std::cout<<"fabs(distance - distanceByHead) / std::max<>(distance, distanceByHead) 比例="<<fabs(distance - distanceByHead) / std::max<>(distance, distanceByHead)<<std::endl;
	//		if(!polePercepts[p].headVisible && headOverHorizon && ((distanceByHead - distance) / std::max<>(distance, distanceByHead)) <  -allowedDistanceNoise)
	//		{
	//			polePercepts[p].discarded2 = true;
	//			//std::cout<<"head not see,above horizon,比例="<<(distanceByHead - distance) / std::max<>(distance, distanceByHead)<<std::endl;

	//		}
	//               	//std::cout<<"(distanceByHead - distance) / std::max<>(distance, distanceByHead) 比例="<<(distanceByHead - distance) / std::max<>(distance, distanceByHead)<<std::endl;

	//	/*	if(polePercepts[p].footVisible && !footUnderHorizon)
	//		{
	//			polePercepts[p].discarded2 = true;
	//			std::cout<<"foot see,but 不在水平线下"<<std::endl;
	//		}
	//		if(polePercepts[p].footVisible && footUnderHorizon && (fabs(distance - distanceByFoot) / std::max<>(distance, distanceByFoot)) > allowedDistanceNoise)
	//		{	polePercepts[p].discarded2 = true;
	//		std::cout<<"foot see,在水平线下，比例="<<fabs(distance - distanceByFoot) / std::max<>(distance, distanceByFoot)<<std::endl;
	//		}
	//              std::cout<<"fabs(distance - distanceByFoot) / std::max<>(distance, distanceByFoot)比例="<<fabs(distance - distanceByFoot) / std::max<>(distance, distanceByFoot)<<std::endl;*/
	//		if(polePercepts[p].discarded2)
	//			polePercepts[p].discarded = true;

	//	}
	//}
	// if we see both poles
	if(polePercepts[0].xImage != 0 && polePercepts[1].xImage != 0 && !polePercepts[0].discarded && !polePercepts[1].discarded)
	{
		polePercepts[0].timeWhenLastSeen = theFrameInfo->time;
		polePercepts[1].timeWhenLastSeen = theFrameInfo->time;
		polePercepts[0].seenInThisFrame = true;
		polePercepts[1].seenInThisFrame = true;

		if(polePercepts[0].xImage < polePercepts[1].xImage)
		{
			polePercepts[0].poleSide = blob::LEFT_;
			polePercepts[1].poleSide = blob::RIGHT_;
		}
		else
		{
			polePercepts[0].poleSide = blob::RIGHT_;
			polePercepts[1].poleSide =  blob::LEFT_;
		}

	}
	// if we see only one pole
	else if((polePercepts[0].xImage != 0 && !polePercepts[0].discarded))
	{
		polePercepts[0].timeWhenLastSeen=theFrameInfo->time;
		polePercepts[0].seenInThisFrame=true;
		bool poleIdentified = false;
		int left  = 0;
		int right = 0;

		// crossbar detection
		if(polePercepts[0].headVisible)//在head可见情况下做crossbar detection
		{
			//bool crossbarDetected = false;
			int xScanOffset = 150;
			int yScanTop = polePercepts[0].yImageTop - 30;
			if(yScanTop<0) yScanTop=0;
			int yScanBottom = polePercepts[0].yImageTop + 30;
			if(yScanBottom>IMAGE_HEIGHT-1) yScanBottom=IMAGE_HEIGHT-1;
			if(polePercepts[0].xImage < xScanOffset)
				xScanOffset = polePercepts[0].xImage;
			if(polePercepts[0].xImage >= IMAGE_WIDTH - xScanOffset)
				xScanOffset = IMAGE_WIDTH- polePercepts[0].xImage - 1;

			if(xScanOffset > 25)//如果单根门柱在图片中央可以直接用verticalRun进行横梁判断
			{

				// new crossbar detection
				for(unsigned int i=0; i<theRegionpercept->VerticalRun.size(); i++)
				{
					Run segment = theRegionpercept->VerticalRun[i];

					if(segment.colour == goalColor && segment.length > 3 && segment.length < 50 && abs(polePercepts[0].xImageTop - segment.start.x) <= xScanOffset
						&& ((segment.start.y >= yScanTop && segment.start.y <= yScanBottom) || (segment.end.y>= yScanTop && segment.end.y <= yScanBottom)))
					{
						if(segment.start.x > polePercepts[0].xImageTop)
							right++;
						else
							left++;
					}
				}
			}

			// if the crossbar has not been identified clearly
			if(abs(right - left) > 3)
			{
				poleIdentified = true;
			}

			// try to find small part of crossbar用verticalRun判断失败后可能是柱子太靠图片边缘，横梁上的verticalRun太少
			//所以需要更局部的扫面才能确定柱子类型
			else
			{
				left = 0;
				right = 0;
				int xls = polePercepts[0].xImageTop - polePercepts[0].width ;//确定柱子在边界的多少就不做局部扫描
				if(xls<=0) xls=0;
				int xrs = polePercepts[0].xImageTop + polePercepts[0].width ;
				if(xrs>IMAGE_WIDTH-1) xrs=IMAGE_WIDTH-1;
				if(polePercepts[0].headVisible)//if(xls >= 0 && xrs < IMAGE_WIDTH&& polePercepts[0].headVisible)
				{
					int yss = polePercepts[0].yImageTop - 40;
					int yse = polePercepts[0].yImageTop + 40;
					if(yss < 0) yss = 0;
					if(yse >= IMAGE_HEIGHT) yse = IMAGE_HEIGHT- 1;

					for(int y = yss; y <= yse; y++)
					{
						if(convertColorAt(xls, y) == goalColor)
							left++;
						if(convertColorAt(xrs, y) == goalColor)
							right++;
					}


					if(left + right != 0)
					{
						poleIdentified = true;

					}
				}
			}
		}

		// if the pole was identified update pole position
		if(poleIdentified)
		{
			if(right > left)
			{
				polePercepts[0].poleSide = blob::LEFT_;
			}
			else
			{
				polePercepts[0].poleSide = blob::RIGHT_;
			}
		}
		else
			polePercepts[0].poleSide = blob::UNDEFINED;
	}

	else if((polePercepts[1].xImage != 0 && !polePercepts[1].discarded))
	{
		polePercepts[1].timeWhenLastSeen=theFrameInfo->time;
		polePercepts[1].seenInThisFrame=true;
		bool poleIdentified = false;
		int left  = 0;
		int right = 0;

		// crossbar detection
		if(polePercepts[1].headVisible)
		{
			//bool crossbarDetected = false;
			int xScanOffset = 150;
			int yScanTop = polePercepts[1].yImageTop - 30;
			int yScanBottom = polePercepts[1].yImageTop + 30;
			if(polePercepts[1].xImage < xScanOffset)
				xScanOffset = polePercepts[1].xImage;
			if(polePercepts[1].xImage >= IMAGE_WIDTH - xScanOffset)
				xScanOffset = IMAGE_WIDTH- polePercepts[1].xImage - 1;

			if(xScanOffset > 25)
			{

				// new crossbar detection
				for(unsigned int i=0; i<theRegionpercept->VerticalRun.size(); i++)
				{
					Run segment = theRegionpercept->VerticalRun[i];

					if(segment.colour == goalColor && segment.length > 3 && segment.length < 50 && abs(polePercepts[1].xImageTop - segment.start.x) <= xScanOffset
						&& ((segment.start.y >= yScanTop && segment.start.y <= yScanBottom) || (segment.end.y>= yScanTop && segment.end.y <= yScanBottom)))
					{
						if(segment.start.x > polePercepts[1].xImageTop)
							right++;
						else
							left++;
					}
				}
			}

			// if the crossbar has not been identified clearly
			if(abs(right - left) > 3)
			{
				poleIdentified = true;

			}
			// try to find small part of crossbar
			else
			{
				left = 0;
				right = 0;
				int xls = polePercepts[1].xImageTop - polePercepts[1].width * 2;
				int xrs = polePercepts[1].xImageTop + polePercepts[1].width * 2;
				if(xls >= 0 && xrs < IMAGE_WIDTH&& polePercepts[0].headVisible)
				{
					int yss = polePercepts[1].yImageTop - 40;
					int yse = polePercepts[1].yImageTop + 40;
					if(yss < 0) yss = 0;
					if(yse >= IMAGE_HEIGHT) yse = IMAGE_HEIGHT- 1;

					for(int y = yss; y <= yse; y++)
					{
						if(convertColorAt(xls, y) == goalColor)
							left++;
						if(convertColorAt(xrs, y) == goalColor)
							right++;
					}

					if(left + right != 0)
					{
						poleIdentified = true;
					}
				}
			}
		}

		// if the pole was identified update pole position
		if(poleIdentified)
		{
			if(right > left)
			{
				polePercepts[1].poleSide = blob::LEFT_;
			}
			else
			{
				polePercepts[1].poleSide = blob::RIGHT_;
			}
		}
		else
			polePercepts[1].poleSide = blob::UNDEFINED;
	}


}
void GoalSpecialist::update(GoalPercept*theGoalPercept)
 {


    preExecution(theGoalPercept);
	MotionUpdate(theGoalPercept);
	detectGoal(YELLOW,goalpercepts);
	//updateSideByDirection();
	updateSide();
	updateRepresentation(theGoalPercept,oppgoalpercepts, owngoalpercepts);
	calculateShot(*theGoalPercept);
	PostExecution();
	outGoalPercept();
	//std::cout<<" ****after goalspecialist****    "<<std::endl;

}
void GoalSpecialist::inTJImage()
{

}

void GoalSpecialist::inCameraMatrix()
{

}
void GoalSpecialist::outGoalPercept()
{
#ifdef TJPIC
	sendGoalResult();
#endif
}
void GoalSpecialist::sendGoalResult()
{
	netGoalpost * netvision;
	netvision = new netGoalpost();

	netvision->ownleftgoalSeen = (uint8)(theGoalPercept->getownleftgoalseen()?1:0);
	netvision->ownrightgoalSeen = (uint8)(theGoalPercept->getownrightgoalseen()?1:0);
	netvision->unknowowngoalSeen = (uint8)(theGoalPercept->getunowngoalseen()?1:0);
	netvision->oppleftgoalSeen=(uint8)(theGoalPercept->getoppleftgoalseen()?1:0);
	netvision->opprightgoalSeen=(uint8)(theGoalPercept->getopprightgoalseen()?1:0);
	netvision->unknowoppgoalSeen = (uint8)(theGoalPercept->getunoppgoalseen()?1:0);
	// netvision->HorizenInImage = (uint8)(theRegionpercept->horizonImage ?1:0);
	//for some reason , mutilp 100, when use the datas,remeber to divide 100

	netvision->ownleftGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OWN].positionOnField.x);
	netvision->ownleftGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OWN].positionOnField.y);
	netvision->ownrightGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OWN].positionOnField.x);
	netvision->ownrightGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OWN].positionOnField.y);
	netvision->unknowownGoalX = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OWN].positionOnField.x);
	netvision->unknowownGoalY = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OWN].positionOnField.y);
	netvision->oppleftGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OPPONENT].positionOnField.x);
	netvision->oppleftGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::LEFT_OPPONENT].positionOnField.y);
	netvision->opprightGoalX = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OPPONENT].positionOnField.x);
	netvision->opprightGoalY = (int32)100*(theGoalPercept->posts[GoalPercept::RIGHT_OPPONENT].positionOnField.y);
	netvision->unknowoppGoalX = (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OPPONENT].positionOnField.x);
	netvision->unknowoppGoalY= (int32)100*(theGoalPercept->unknownPosts[GoalPercept::UNKNOWN_OPPONENT].positionOnField.y);
	netvision->horizonStartX = (int32)theRegionpercept->horizonStart.x;
	netvision->horizonStartY = (int32)theRegionpercept->horizonStart.y;
	netvision->horizonEndX = (int32)theRegionpercept->horizonEnd.x;
	netvision->horizonEndY = (int32)theRegionpercept->horizonEnd.y;
	netvision->HorizenInImage=(uint8)theRegionpercept->horizonImage;

	selfMessageQueue->SetMessageData(idGoalPercept,idRobotThread,idNetworkThread,
		sizeof(netGoalpost),(char*)netvision);
}
void GoalSpecialist::preExecution(GoalPercept*goalpercept)
{
	//owncolour=getOurTeamNumber()==0?CYAN:YELLOW;
	//oppcolour=getOurTeamNumber()==0?YELLOW:CYAN;
	odometryOffset = *theOdometryData - lastOdometry;
	for(unsigned int i=0; i<GoalPercept::NUMBER_OF_UNKNOWN_GOAL_POSTS; ++i)
	{
		goalpercept->unknownPosts[i].perceptionType = GoalPost::NEVER_SEEN;
		goalpercept->unknownPosts[i]. positionInImage = Vector2<int>(0,0);
		goalpercept->unknownPosts[i].positionOnField = Vector2<int>(0,0);
		goalpercept->unknownPosts[i].distanceType =GoalPost::BEARING_BASED;
	}
	goalpercept->LeftShotAngle=Vector2<double>(0.0,0.0);
	goalpercept->RightShotAngle=Vector2<double>(0.0,0.0);
	goalpercept->MiddleShotAngle=Vector2<double>(0.0,0.0);
	goalpercept->LeftOwnAngle=Vector2<double>(0.0,0.0);
	goalpercept->RightOwnAngle=Vector2<double>(0.0,0.0);

}
void GoalSpecialist::MotionUpdate(GoalPercept*goalpercept)
{

	  for(int i=0; i<GoalPercept::NUMBER_OF_GOAL_POSTS; i++)
	{  if(theFrameInfo->time- goalpercept->posts[i].timeWhenLastSeen<motionUpdatecounter&&goalpercept->posts[i].timeWhenLastSeen!=0)
	  {
	    goalpercept->posts[i].positionOnField -= Vector2<int>((int) odometryOffset.translation.x, (int) odometryOffset.translation.y);
		goalpercept->posts[i].positionOnField.rotate(-odometryOffset.rotation);
		goalpercept->posts[i].perceptionType = GoalPost::CALCULATED;

	}
	else{
		goalpercept->posts[i].positionOnField = Vector2<int>(0,0);
		goalpercept->posts[i].perceptionType=GoalPost::NEVER_SEEN;
		goalpercept->posts[i].distanceType = GoalPost::BEARING_BASED;
		goalpercept->posts[i].positionInImage=Vector2<int>(0,0);
	}

	}
	  //int npink=0,ncyan=0;
	  //for(unsigned int i=0; i<theRegionpercept->HorizontalRun.size(); i++)
	  //{
		 // Run segment = theRegionpercept->HorizontalRun[i];
		 // if (segment.length>100||segment.length<5)
			//  continue;
		 // if(segment.colour ==CYAN||segment.colour ==RED)
		 // {
			//  npink++;
			//  int x,y,y1,y2,num=0;
			//  x=(segment.start.x+segment.end.x)/2;
			//  y=(segment.start.y+segment.end.y)/2;
			//  double a=0,h=0;
			//  if (y>15&&y<225)
			//  {
			//	  for (int i=segment.start.x;i<=segment.end.x;i++)
			//	  {
			//		  //DRAW_BIG_DOT(x,y+50,Drawings::black,Drawings::red);
			//		  if (convertColorAt(i,y-15)==WHITE)
			//		  {
			//			  a++;
			//		  }
			//		  if (convertColorAt(i,y+15)==WHITE)
			//		  {
			//			  a++;
			//		  }
			//	  }
			//	  a=(double)a/segment.length/2;
			//  }
			//  else if (y<16)
			//  {
			//	  for (int i=segment.start.x;i<=segment.end.x;i++)
			//	  {
			//		  if (convertColorAt(i,y+15)==WHITE)
			//			  a++;
			//	  }
			//	  a=(double)a/segment.length;
			//  }
			//  else
			//  {
			//	  for (int i=segment.start.x;i<=segment.end.x;i++)
			//	  {
			//		  if (convertColorAt(i,y-15)==WHITE)
			//			  a++;
			//	  }
			//	  a=(double)a/segment.length;
			//  }

			//  if (a<0.6)
			//	  continue;
			//  if (y<26)
			//	  y1=findDown(x, 0,segment.colour, IMAGE_HEIGHT);
			//  else
			//	  y1=findDown(x, y-25,segment.colour, IMAGE_HEIGHT);
			//  y2=runDown(x,y1,segment.colour,IMAGE_HEIGHT);
			//  y=(y1+y2)/2;
			//  h=y2-y1;
			//  if(h>50||h<2)
			//	  continue;
			//  //for (int j=0;j<5;j++)
			//  //{
			//  //	y1=findDown(j+x-2, y-18,segment.colour, IMAGE_HEIGHT);
			//  //	y2=runDown(j+x-2,y1,segment.colour,IMAGE_HEIGHT);
			//  //	h+=(y2-y1);
			//  //	h=h/5.0;
			//  //}
			//  if (segment.colour==
			//	  RED)
			//  {
			//	  cout<<"****pink****"<<endl;
			//	  DRAW_LINE(segment.start.x,y,segment.end.x,y,3,Drawings::ps_SolidLine,Drawings::yellow);
			//  }
			//  else
			//  {
			//	  cout<<"****blue****"<<endl;
			//	  DRAW_LINE(segment.start.x,y,segment.end.x,y,3,Drawings::ps_SolidLine,Drawings::red);
			//  }
			//  std::cout<<"**********ratio************"<<a<<std::endl;
			//  std::cout<<"**********hight************"<<h<<std::endl;
			//  ncyan++;
			//  DRAW_BIG_DOT(x,y+5*h,Drawings::black,Drawings::red);//下蹲为5，站立为7
			//  Vector2<int>onfield;
			//  Geometry::calculatePointOnField(x,y+5*h,*theCameraMatrix,onfield);
			//  std::cout<<"**********onfield************"<<onfield.x<<";"<<onfield.y<<endl;
		 // }
	  //}
	  //std::cout<<"**********a$b************"<<npink<<"$"<<ncyan<<std::endl;
  }

void GoalSpecialist::PostExecution()
{
	lastOdometry=*theOdometryData;
}



void GoalSpecialist::createKnownGoalPercept( GoalPercept * goalPercept, int index, blob percept)
{
	goalPercept->posts[index].distanceType     = percept.distanceType;
	goalPercept->posts[index].perceptionType   = GoalPost::SEEN_IN_IMAGE;
	goalPercept->posts[index].positionInImage  = Vector2<int> (percept.xImageBottom, percept.yImageBottom);
	goalPercept->posts[index].positionOnField  = Vector2<int> ((int) percept.onField.x, (int) percept.onField.y);
	std::cout<<"AAAAAAAAAAAAAAAAAAAAAAAAgoal distance is              "<<percept.onField.abs()<<std::endl;
	goalPercept->posts[index].timeWhenLastSeen = percept.timeWhenLastSeen;
	DRAW_BIG_DOT( goalPercept->posts[index].positionInImage.x , goalPercept->posts[index].positionInImage.y, Drawings::red ,Drawings::red);

}



void GoalSpecialist::createUnknownGoalPercept(GoalPercept* goalPercept, int index, blob percept)
{
	goalPercept->unknownPosts[index].distanceType     = percept.distanceType;
	goalPercept->unknownPosts[index].perceptionType   = GoalPost::SEEN_IN_IMAGE;
	goalPercept->unknownPosts[index].positionInImage  = Vector2<int> (percept.xImageBottom, percept.yImageBottom);
	goalPercept->unknownPosts[index].positionOnField  = Vector2<int> ((int) percept.onField.x, (int) percept.onField.y);
	goalPercept->unknownPosts[index].timeWhenLastSeen = percept.timeWhenLastSeen;
	DRAW_BIG_DOT( goalPercept->unknownPosts[index].positionInImage.x , goalPercept->unknownPosts[index].positionInImage.y,Drawings::black,Drawings::black);
	//std::cout<<"没认出的门柱坐标="<<index<<"\t"<<goalPercept->posts[index].positionOnField.x<<"\t"<<goalPercept->posts[index].positionOnField.y<<std::endl;
}

int GoalSpecialist::runDown(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y+=2; y < yEnd; y+=2)
	{
		if(convertColorAt(x,y) != col)
		{
			tmp = y - 2;
			for(--y; y > tmp; y--)
				if(convertColorAt(x,y) == col)
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


//计算射门角度，将守门员考虑进去了
void GoalSpecialist::calculateShot(GoalPercept &goalpercept)
{
	Vector2<double>LforAddGoal=Vector2<double>(0.0,0.0);
	Vector2<double>RforAddGoal=Vector2<double>(0.0,0.0);
	Vector2<double>MforAddGoal=Vector2<double>(0.0,0.0);

	Vector2<double>Mpoint = Vector2<double>(0.0,0.0);
	//int scanH=0,goalw;//scanH is the height of scanline
	//bool oppstartFromLeftP=false,oppstartFromRightP=false;
	//Vector2<int> start;
	if((goalpercept.posts[GoalPercept::LEFT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
		&&(goalpercept.posts[GoalPercept::RIGHT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE))
	{
		Vector2<int> shotL,shotR,shotM;
		std::pair<int,int> KeeperCount;
		shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
		shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
		//KeeperCount=scanGoalkeeper(shotL.x+15,shotR.x-15,min<>(shotL.y,shotR.y)-ScanforKeeperAbove,max<>(shotL.y,shotR.y)-ScanforKeeperBelow,opprobocolor);
		//if(KeeperCount.first==-1&&KeeperCount.second==-1)
		//{
			shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
			shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
			shotM.x=(shotL.x+shotR.x)/2;
			shotM.y=(shotL.y+shotR.y)/2;
			goalpercept.MiddleOppPointImage = shotM;
		//}
		//else{
		//	int left=KeeperCount.first-shotL.x;
		//	int right=shotR.x-KeeperCount.second;
		//	if(left>=right)
		//	{
		//		shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
		//		shotR.x=KeeperCount.first;
		//		shotR.y=shotL.y;
		//	}
		//	else
		//	{
		//		shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
  //              shotL.x=KeeperCount.second;
		//		shotL.y=shotR.y;
		//    }
		//}
		Geometry::calculateAnglesForPoint(shotL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(shotR,*theCameraMatrix,RforAddGoal);
		Geometry::calculateAnglesForPoint(shotM,*theCameraMatrix,MforAddGoal);
		goalpercept.LeftShotAngle=LforAddGoal;
		goalpercept.RightShotAngle=RforAddGoal;
		goalpercept.MiddleShotAngle=MforAddGoal;
		/*goalw=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.x-goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-1;
		else scanH=sanRobotHeight;
		oppstartFromLeftP=true;
		start.x=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-gapStepSize;*/
	}
	else if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
	{
		Vector2<int> shotL,shotR,shotM;
		shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
		shotR.x=IMAGE_WIDTH-1;
		shotR.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y;
		std::pair<int,int> KeeperCount;
		//KeeperCount=scanGoalkeeper(shotL.x+15,shotR.x-15,min<>(shotL.y,shotR.y)-ScanforKeeperAbove,max<>(shotL.y,shotR.y)-ScanforKeeperBelow,opprobocolor);
		//if(KeeperCount.first==-1&&KeeperCount.second==-1)
		//{
			shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
			shotR.x=IMAGE_WIDTH-1;
			shotR.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y;
			shotM.x=(shotL.x+shotR.x)/2;
			shotM.y=(shotL.y+shotR.y)/2;
			goalpercept.MiddleOppPointImage = shotM;
		//}
		//else{
		//	int left=KeeperCount.first-shotL.x;
		//	int right=shotR.x-KeeperCount.second;
		//	if(left>=right)
		//	{
		//		shotL=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage;
		//		shotR.x=KeeperCount.first;
		//		shotR.y=shotL.y;
		//	}
		//	else
		//	{
		//		shotR.x=IMAGE_WIDTH-1;
		//		shotR.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y;
		//		shotL.x=KeeperCount.second;
		//		shotL.y=shotR.y;
		//	}
		//}
		Geometry::calculateAnglesForPoint(shotL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(shotR,*theCameraMatrix,RforAddGoal);
		Geometry::calculateAnglesForPoint(shotM,*theCameraMatrix,MforAddGoal);
		goalpercept.LeftShotAngle=LforAddGoal;
		goalpercept.RightShotAngle=RforAddGoal;
		goalpercept.MiddleShotAngle=MforAddGoal;
		/*	goalw=IMAGE_WIDTH-1-goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y -1;
		else scanH=sanRobotHeight;
		oppstartFromLeftP=true;
		start.x=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-gapStepSize;*/
	}
	else if(goalpercept.posts[GoalPercept::RIGHT_OPPONENT].perceptionType==GoalPost::SEEN_IN_IMAGE)
	{
		Vector2<int> shotL,shotR,shotM;
		shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
		shotL.x=1;
		shotL.y=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y;
		std::pair<int,int> KeeperCount;
		//KeeperCount=scanGoalkeeper(shotL.x+15,shotR.x-15,min<>(shotL.y,shotR.y)-ScanforKeeperAbove,max<>(shotL.y,shotR.y)-ScanforKeeperBelow,opprobocolor);
		//if(KeeperCount.first==-1&&KeeperCount.second==-1)
		//{
			shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
			shotL.x=1;
			shotL.y=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y;
			shotM.x=(shotL.x+shotR.x)/2;
			shotM.y=(shotL.y+shotR.y)/2;
			goalpercept.MiddleOppPointImage = shotM;
		//}
		//else{
		//	int left=KeeperCount.first-shotL.x;
		//	int right=shotR.x-KeeperCount.second;
		//	if(left>=right)
		//	{
		//		shotL.x=1;
		//		shotL.y=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y;
		//		shotR.x=KeeperCount.first;
		//		shotR.y=shotL.y;
		//	}
		//	else
		//	{
		//		shotR=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage;
		//		shotL.x=KeeperCount.second;
		//		shotL.y=shotR.y;
		//	}
		//}
		Geometry::calculateAnglesForPoint(shotL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(shotR,*theCameraMatrix,RforAddGoal);
		Geometry::calculateAnglesForPoint(shotM,*theCameraMatrix,MforAddGoal);
		goalpercept.LeftShotAngle=LforAddGoal;
		goalpercept.RightShotAngle=RforAddGoal;
		goalpercept.MiddleShotAngle=MforAddGoal;
		/*	goalw=IMAGE_WIDTH-1-goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y -1;
		else scanH=sanRobotHeight;
		oppstartFromLeftP=true;
		start.x=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-gapStepSize;*/
	}
	if((goalpercept.posts[GoalPercept::LEFT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
		&&(goalpercept.posts[GoalPercept::RIGHT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE))
	{
		Vector2<int> ownL,ownR;
		ownL=goalpercept.posts[GoalPercept::LEFT_OWN].positionInImage;
		ownR=goalpercept.posts[GoalPercept::RIGHT_OWN].positionInImage;
		Geometry::calculateAnglesForPoint(ownL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(ownR,*theCameraMatrix,RforAddGoal);
		goalpercept.LeftOwnAngle=LforAddGoal;
		goalpercept.RightOwnAngle=RforAddGoal;
		/*goalw=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.x-goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-1;
		else scanH=sanRobotHeight;
		oppstartFromLeftP=true;
		start.x=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-gapStepSize;*/
	}
	else if(goalpercept.posts[GoalPercept::LEFT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
	{
		Vector2<int> ownL,ownR;
		ownL=goalpercept.posts[GoalPercept::LEFT_OWN].positionInImage;
		ownR.x=IMAGE_WIDTH-1;
		ownR.y=goalpercept.posts[GoalPercept::LEFT_OWN].positionInImage.y;
		Geometry::calculateAnglesForPoint(ownL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(ownR,*theCameraMatrix,RforAddGoal);
		goalpercept.LeftOwnAngle=LforAddGoal;
		goalpercept.RightOwnAngle=RforAddGoal;
		/*	goalw=IMAGE_WIDTH-1-goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		if(goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y -1;
		else scanH=sanRobotHeight;
		oppstartFromLeftP=true;
		start.x=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::LEFT_OPPONENT].positionInImage.y-gapStepSize;*/
	}
	else if(goalpercept.posts[GoalPercept::RIGHT_OWN].perceptionType==GoalPost::SEEN_IN_IMAGE)
	{
		Vector2<int> ownL,ownR;
		ownR=goalpercept.posts[GoalPercept::RIGHT_OWN].positionInImage;
		ownL.x=1;
		ownL.y=goalpercept.posts[GoalPercept::RIGHT_OWN].positionInImage.y;
		Geometry::calculateAnglesForPoint(ownL,*theCameraMatrix,LforAddGoal);
		Geometry::calculateAnglesForPoint(ownR,*theCameraMatrix,RforAddGoal);
		goalpercept.LeftOwnAngle=LforAddGoal;
		goalpercept.RightOwnAngle=RforAddGoal;
		/*	goalw=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.x-1;
		if(goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y < sanRobotHeight)
		scanH=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y -1;
		else scanH=sanRobotHeight;
		oppstartFromRightP=true;
		start.x=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.x;
		start.y=goalpercept.posts[GoalPercept::RIGHT_OPPONENT].positionInImage.y-gapStepSize;*/
	}


}

int GoalSpecialist::calculateExpectedPixelWidth(Vector2<int> posImg,const CameraMatrix&cameraMatrix)
{
	Vector2<double> pos;
	if(!Geometry::calculatePointOnField(posImg.x, posImg.y, cameraMatrix, pos))
		return -1;
	Vector2<double> p1 = pos + Vector2<double>(0, GoalPostWidthInMM/2.0);
	Vector2<double> p2 = pos - Vector2<double>(0, GoalPostWidthInMM/2.0);

	Vector2<int> p1Img, p2Img;

	if(!Geometry::calculatePointInImage(Vector2<int>((int)p1.x, (int)p1.y), cameraMatrix, p1Img))
		return -1;
	if(!Geometry::calculatePointInImage(Vector2<int>((int)p2.x, (int)p2.y), cameraMatrix,p2Img))
		return -1;

	///Vector2<double> p1Uncor = theImageCoordinateSystem.fromCorrectedApprox(p1Img);
	//Vector2<double> p2Uncor = theImageCoordinateSystem.fromCorrectedApprox(p2Img);

	return  abs(p1Img.x - p2Img.x);
	//Vector2<double> pos;
	//Geometry::calculatePointOnField(posImg.x, posImg.y, cameraMatrix, pos);
	///*if(!Geometry::calculatePointOnField(posImg.x, posImg.y, cameraMatrix, pos))
	//	return -1;*/
	//Vector2<double> offset(pos.x - cameraMatrix.translation.x,
	//	pos.y - cameraMatrix.translation.y);
	//Vector2<double> lAddon(GoalPostWidthInMM/2.0, 0);
	//lAddon.rotate(offset.angle());
	//   offset+= lAddon;
	//
	// double distance =offset.abs(),
	//	 //height = cameraMatrix.translation.z - ballRadius,
	//	 cameraDistance = sqrt(sqr(cameraMatrix.translation.z) + sqr(distance));
	// double tiltdistance=sqrt(sqr(cameraDistance)-sqr(GoalPostWidthInMM/2.0)),
	// leftalpha=atan2(offset.y,offset.x)-asin(GoalPostWidthInMM/(2.0*distance)),
	// rightalpha=atan2(offset.y,offset.x)+asin(GoalPostWidthInMM/(2.0*distance)),
	//  belta=-asin(cameraMatrix.translation.z/tiltdistance);
	//	 Vector2<int> p1Img,
	//		p2Img;
	//	 Geometry::calculatePointByAngles(Vector2<double>(leftalpha,belta),cameraMatrix, p1Img);
	//	 Geometry::calculatePointByAngles(Vector2<double>(rightalpha,belta),cameraMatrix, p2Img);
	////Vector2<double> p1Uncor = theImageCoordinateSystem.fromCorrectedApprox(p1Img);
	////Vector2<double> p2Uncor = theImageCoordinateSystem.fromCorrectedApprox(p2Img);
	//	 DRAW_LINE(p1Img.x,p1Img.y,p2Img.x,p2Img.y,1,Drawings::ps_SolidLine,Drawings::darkMagenta);
	//return (int) abs(p1Img.x - p2Img.x);
}
void GoalSpecialist::updateRepresentation(GoalPercept*goalPercept, blob oppPercepts[],blob ownPercepts[])
{

	// if we see two complete goals -> discard both
	if(oppPercepts[0].seenInThisFrame && !oppPercepts[0].discarded && oppPercepts[0].poleSide != blob ::UNDEFINED
		&& oppPercepts[1].seenInThisFrame && !oppPercepts[1].discarded && oppPercepts[1].poleSide != blob ::UNDEFINED
		&& ownPercepts[0].seenInThisFrame && !ownPercepts[0].discarded && ownPercepts[0].poleSide != blob ::UNDEFINED
		&& ownPercepts[1].seenInThisFrame && !ownPercepts[1].discarded && ownPercepts[1].poleSide != blob ::UNDEFINED)
	{
	}
	// if we see opp goal complete -> discard own goal
	else if(oppPercepts[0].seenInThisFrame && !oppPercepts[0].discarded && oppPercepts[0].poleSide != blob ::UNDEFINED
		&& oppPercepts[1].seenInThisFrame && !oppPercepts[1].discarded && oppPercepts[1].poleSide != blob ::UNDEFINED)
	{
		if(oppPercepts[0].poleSide == blob ::LEFT_)
		{
			createKnownGoalPercept(goalPercept, 0, oppPercepts[0]);
			createKnownGoalPercept(goalPercept, 1, oppPercepts[1]);
		}
		else
		{
			createKnownGoalPercept(goalPercept, 1, oppPercepts[0]);
			createKnownGoalPercept(goalPercept, 0, oppPercepts[1]);
		}
	}
	// if we see own goal complete -> discard opp goal
	else if(ownPercepts[0].seenInThisFrame && !ownPercepts[0].discarded && ownPercepts[0].poleSide != blob ::UNDEFINED
		&& ownPercepts[1].seenInThisFrame && !ownPercepts[1].discarded && ownPercepts[1].poleSide !=blob ::UNDEFINED)
	{
		if(ownPercepts[0].poleSide == blob::LEFT_)
		{
			createKnownGoalPercept(goalPercept, 2, ownPercepts[0]);
			createKnownGoalPercept(goalPercept, 3, ownPercepts[1]);
		}
		else
		{
			createKnownGoalPercept(goalPercept, 3, ownPercepts[0]);
			createKnownGoalPercept(goalPercept, 2, ownPercepts[1]);
		}
	}
	// if we see opp goal nearly complete -> discard own goal
	else if(oppPercepts[0].seenInThisFrame && !oppPercepts[0].discarded
		&& oppPercepts[1].seenInThisFrame && !oppPercepts[1].discarded
		&& (oppPercepts[0].poleSide != blob ::UNDEFINED || oppPercepts[1].poleSide != blob ::UNDEFINED))
	{
		if(oppPercepts[0].poleSide == blob ::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 0, oppPercepts[0]);
		else if(oppPercepts[0].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 0, oppPercepts[0]);
		else
			createKnownGoalPercept(goalPercept, 1, oppPercepts[0]);

		if(oppPercepts[1].poleSide == blob ::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 0, oppPercepts[1]);
		else if(oppPercepts[1].poleSide ==blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 0, oppPercepts[1]);
		else
			createKnownGoalPercept(goalPercept, 1, oppPercepts[1]);
	}
	// if we see own goal nearly complete -> discard opp goal
	else if(ownPercepts[0].seenInThisFrame && !ownPercepts[0].discarded
		&& ownPercepts[1].seenInThisFrame && !ownPercepts[1].discarded
		&& (ownPercepts[0].poleSide != blob ::UNDEFINED || ownPercepts[1].poleSide != blob ::UNDEFINED))
	{
		if(ownPercepts[0].poleSide == blob ::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 1, ownPercepts[0]);
		else if(ownPercepts[0].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 2, ownPercepts[0]);
		else
			createKnownGoalPercept(goalPercept, 3, ownPercepts[0]);

		if(ownPercepts[1].poleSide == blob ::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 1, ownPercepts[1]);
		else if(ownPercepts[1].poleSide ==blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 2, ownPercepts[1]);
		else
			createKnownGoalPercept(goalPercept, 3, ownPercepts[1]);
	}
	// left opp
	else if(oppPercepts[0].seenInThisFrame && !oppPercepts[0].discarded
		&& (!ownPercepts[0].seenInThisFrame || ownPercepts[0].discarded)
		&& (!ownPercepts[1].seenInThisFrame || ownPercepts[1].discarded))
	{
		if(oppPercepts[0].poleSide == blob::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 0, oppPercepts[0]);
		else if(oppPercepts[0].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 0, oppPercepts[0]);
		else
			createKnownGoalPercept(goalPercept, 1, oppPercepts[0]);
	}
	// right opp
	else if(oppPercepts[1].seenInThisFrame && !oppPercepts[1].discarded
		&& (!ownPercepts[0].seenInThisFrame || ownPercepts[0].discarded)
		&& (!ownPercepts[1].seenInThisFrame || ownPercepts[1].discarded))
	{
		if(oppPercepts[1].poleSide == blob::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 0, oppPercepts[1]);
		else if(oppPercepts[1].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 0, oppPercepts[1]);
		else
			createKnownGoalPercept(goalPercept, 1, oppPercepts[1]);
	}
	// left own
	else if(ownPercepts[0].seenInThisFrame && !ownPercepts[0].discarded
		&& (!oppPercepts[0].seenInThisFrame || oppPercepts[0].discarded)
		&& (!oppPercepts[1].seenInThisFrame || oppPercepts[1].discarded))
	{
		if(ownPercepts[0].poleSide == blob::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 1, ownPercepts[0]);
		else if(ownPercepts[0].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 2, ownPercepts[0]);
		else
			createKnownGoalPercept(goalPercept, 3, ownPercepts[0]);
	}
	// right own
	else if(ownPercepts[1].seenInThisFrame && !ownPercepts[1].discarded
		&& (!oppPercepts[0].seenInThisFrame || oppPercepts[0].discarded)
		&& (!oppPercepts[1].seenInThisFrame || oppPercepts[1].discarded))
	{
		if(ownPercepts[1].poleSide == blob::UNDEFINED)
			createUnknownGoalPercept(goalPercept, 1, ownPercepts[1]);
		else if(ownPercepts[1].poleSide == blob ::LEFT_)
			createKnownGoalPercept(goalPercept, 2, ownPercepts[1]);
		else
			createKnownGoalPercept(goalPercept, 3, ownPercepts[1]);
	}

	if(goalPercept->posts[0].timeWhenLastSeen == theFrameInfo->time || goalPercept->posts[1].timeWhenLastSeen == theFrameInfo->time)
		goalPercept->timeWhenOppGoalLastSeen = theFrameInfo->time;

	if(goalPercept->posts[2].timeWhenLastSeen == theFrameInfo->time || goalPercept->posts[3].timeWhenLastSeen == theFrameInfo->time)
		goalPercept->timeWhenOwnGoalLastSeen = theFrameInfo->time;
}
bool GoalSpecialist::isCameraFacedOpp()
{
	double faceAngle = theRobotPose->direction + theCameraMatrix->rotation.getZAngle();
	//double faceAngle = theRobotPose->rotation + theCameraMatrix->rotation.getZAngle();
	//double faceAngle = 0.0 + theCameraMatrix->rotation.getZAngle();
	if(abs(faceAngle) > pi)
	{
		if(faceAngle>0)
			faceAngle = -(2*pi - faceAngle);
		else
			faceAngle = 2*pi + faceAngle;
	}

	if(abs(faceAngle) <= pi_2 )
	{
		std::cout<<"FACE oPPPPPPPPPPPPPPPPPPPPPPPP  SIDE"<<std::endl;
		return true;
	}
	else
	{
		std::cout<<"FACE OWWWWWWWWWWWWWWWWWWWWWWWW   SIDE"<<std::endl;
		return false;
	}
}
void GoalSpecialist::updateSideByDirection()
{
	if (goalpercepts[0].seenInThisFrame||goalpercepts[1].seenInThisFrame)
	{
		if (isCameraFacedOpp())
		{
			oppgoalpercepts[0] = goalpercepts[0];
			oppgoalpercepts[1] = goalpercepts[1];
		}
		else
		{
			owngoalpercepts[0] = goalpercepts[0];
			owngoalpercepts[1] = goalpercepts[1];
		}
	}
	//else if (goalpercepts[0].seenInThisFrame)
	//{
	//	if (isCameraFacedOpp())
	//	{
	//		oppgoalpercepts[0] = goalpercepts[0];
	//		oppgoalpercepts[1] = goalpercepts[1];
	//	}
	//	else
	//	{
	//		owngoalpercepts[0] = goalpercepts[0];
	//		owngoalpercepts[1] = goalpercepts[1];
	//	}
	//}
	//else if (goalpercepts[1].seenInThisFrame)
	//{
	//	if(isCameraFacedOpp())
	//	{
	//		oppgoalpercepts[1] = goalpercepts[1];
	//		oppgoalpercepts[0] = goalpercepts[0];
	//	}
	//	else
	//	{
	//		owngoalpercepts[1] = goalpercepts[1];
	//		owngoalpercepts[0] = goalpercepts[0];
	//	}
	//}
	else
		return;
}
void GoalSpecialist::updateSideByLoc()
{
	if (goalpercepts[0].seenInThisFrame)
	{
		double disOpp = sqrt(sqr(theRobotPose->translation.x - 3000) + sqr(theRobotPose->translation.y - 700));
		double disOwn = sqrt(sqr(theRobotPose->translation.x + 3000) + sqr(theRobotPose->translation.y + 700));
		double OppErr = abs(disOpp-goalpercepts[0].onField.abs() );
		double OwnErr = abs(disOwn-goalpercepts[0].onField.abs() );
		if (OppErr < OwnErr)
		{
			//std::cout<<"SEEN OPP GOAL"<<std::endl;
			oppgoalpercepts[0] = goalpercepts[0];
			oppgoalpercepts[1] = goalpercepts[1];
		}
		else
		{
			owngoalpercepts[0] = goalpercepts[0];
			owngoalpercepts[1] = goalpercepts[1];
		}
	}

	else if (goalpercepts[1].seenInThisFrame)
	{
		double disOpp = sqrt(sqr(theRobotPose->translation.x - 3000) + sqr(theRobotPose->translation.y + 700));
		double disOwn = sqrt(sqr(theRobotPose->translation.x + 3000) + sqr(theRobotPose->translation.y - 700));
		double OppErr = abs(disOpp-goalpercepts[0].onField.abs() );
		double OwnErr = abs(disOwn-goalpercepts[0].onField.abs() );
		if (OppErr < OwnErr)
		{
			oppgoalpercepts[0] = goalpercepts[0];
			oppgoalpercepts[1] = goalpercepts[1];
		}
		else
		{
			owngoalpercepts[0] = goalpercepts[0];
			owngoalpercepts[1] = goalpercepts[1];
		}
	}
}
void GoalSpecialist::updateSideByDis() // not quite correct
{
	if (theRobotPose->ownSide)
	{
		if(goalpercepts[0].seenInThisFrame&&goalpercepts[1].seenInThisFrame)
		{
			std::cout<<"SEEN BOTH GOAL"<<std::endl;

			if(goalpercepts[0].onField.abs()<3000||goalpercepts[1].onField.abs()<3000)
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			return;
		}
		else if (goalpercepts[0].seenInThisFrame)
		{
			if(goalpercepts[0].onField.abs()<3100)
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				//oppgoalpercepts[0] = goalpercepts[0];
				//oppgoalpercepts[1] = goalpercepts[1];
			}
		}
		else if (goalpercepts[1].seenInThisFrame)
		{
			if(goalpercepts[1].onField.abs()<3100)
			{
				owngoalpercepts[1] = goalpercepts[1];
				owngoalpercepts[0] = goalpercepts[0];
			}
			else
			{
				//oppgoalpercepts[1] = goalpercepts[1];
				//oppgoalpercepts[0] = goalpercepts[0];
			}
		}
		else
			return;

	}
	else
	{
		if(goalpercepts[0].seenInThisFrame&&goalpercepts[1].seenInThisFrame)
		{
			std::cout<<"SEEN BOTH GOAL"<<std::endl;
			if(goalpercepts[0].onField.abs()<3000||goalpercepts[1].onField.abs()<3000)
			{
				std::cout<<"OPP GOAL"<<std::endl;
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				std::cout<<"OWN GOAL"<<std::endl;
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			return;
		}
		else if (goalpercepts[0].seenInThisFrame)
		{
			if(goalpercepts[0].onField.abs()<3100)
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				//owngoalpercepts[0] = goalpercepts[0];
				//owngoalpercepts[1] = goalpercepts[1];
			}
		}
		else if(goalpercepts[1].seenInThisFrame)
		{
			if(goalpercepts[1].onField.abs()<3100)
			{
				oppgoalpercepts[1] = goalpercepts[1];
				oppgoalpercepts[0] = goalpercepts[0];
			}
			else
			{
				//owngoalpercepts[1] = goalpercepts[1];
				//owngoalpercepts[0] = goalpercepts[0];
			}
		}
		else
			return;
	}
}
void GoalSpecialist::updateSide()
{

	//if (goalpercepts[0].seenInThisFrame&&goalpercepts[1].seenInThisFrame)
	//{
	//	if (theRobotPose->ownSide)
	//	{
	//		if(goalpercepts[0].onField.abs()<2900||goalpercepts[1].onField.abs()<2900)
	//		{
	//			owngoalpercepts[0] = goalpercepts[0];
	//			owngoalpercepts[1] = goalpercepts[1];
	//		}
	//		else
	//		{
	//			oppgoalpercepts[0] = goalpercepts[0];
	//			oppgoalpercepts[1] = goalpercepts[1];
	//		}
	//	}
	//	else
	//	{
	//		if(goalpercepts[0].onField.abs()<2900||goalpercepts[1].onField.abs()<2900)
	//		{
	//			oppgoalpercepts[0] = goalpercepts[0];
	//			oppgoalpercepts[1] = goalpercepts[1];
	//		}
	//		else
	//		{
	//			owngoalpercepts[0] = goalpercepts[0];
	//			owngoalpercepts[1] = goalpercepts[1];
	//		}
	//	}
	//	return;
	//}
	if (goalpercepts[0].seenInThisFrame)
	{
		if( (abs(goalpercepts[0].onField.abs() - 3000) < 700 ) && theRobotPose->validity > 0.65 )
		{
			if (isCameraFacedOpp())
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			return;
		}

		
		if (goalpercepts[0].onField.abs() < 2300)
		{
			if (theRobotPose->ownSide)
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
		}

		if (goalpercepts[0].onField.abs() > 3700)
		{
			if (theRobotPose->ownSide)
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
		}
		
	}
	else if (goalpercepts[1].seenInThisFrame)
	{
		if((abs(goalpercepts[1].onField.abs() - 3000) < 700 ) && theRobotPose->validity > 0.65)
		{
			if (isCameraFacedOpp())
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			return;
		}

		if (goalpercepts[1].onField.abs() < 2300)
		{
			if (theRobotPose->ownSide)
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
		}
		if (goalpercepts[1].onField.abs() > 3700)
		{
			if (theRobotPose->ownSide)
			{
				oppgoalpercepts[0] = goalpercepts[0];
				oppgoalpercepts[1] = goalpercepts[1];
			}
			else
			{
				owngoalpercepts[0] = goalpercepts[0];
				owngoalpercepts[1] = goalpercepts[1];
			}
		}
		
	}
	else
		return;


	//updateSideByDirection();
	//if(abs(theRobotPose->middleTranslationX)<=300)
	//	//updateSideByLoc();
	//	updateSideByDirection();
	//else if(theRobotPose->validity > 0.8 &&abs(theRobotPose->translation.x) > 500)
	//	updateSideByLoc();
	//else
	//	updateSideByDis();
}
int GoalSpecialist::findDown(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y+=SkipOffset; y < yEnd; y+=SkipOffset)
	{
		if(convertColorAt(x,y) == col)
		{
			tmp = y - SkipOffset;
			for(--y; y > tmp; y--)
				if(convertColorAt(x,y) != col)
					break;

			//if(y != tmp)
			++y;
			break;
		}
	}
	if(y > yEnd)
		y = yEnd;
	return y;

}

double GoalSpecialist::getGoalPostDistFromHeight(double heightInPixel,double GoalHeightInMM)
{
	//#if ROBOT(NAO_SIM)
	// return 17826*pow((double) height,-1.0254);
	//#else
	// return 39305*pow((double) height,-0.9245);
	//  return pose->pixHeightToDistance(height, GOAL_POST_CM_HEIGHT);
	return (FOCAL_LENGTH / heightInPixel) * GoalHeightInMM;
	//#endif
}
double GoalSpecialist::getGoalPostDistFromWidth(double postwidthInPixel,double GoalPostWidthInMM)
{
	//#if ROBOT(NAO_SIM)
	//floor distance, seems to be best for the width
	//camera dist - 2585.4*pow(width,-1.0678);//OLD return 100.0*13.0/width;
	//return 2360.1*pow((double) width,-1.0516);
	//#else
	//return 10083*pow((double) width,-1.052);
	//return pose->pixWidthToDistance(width, GOAL_POST_CM_WIDTH);
	return (FOCAL_LENGTH / postwidthInPixel) * GoalPostWidthInMM;
	//#endif
}
/*获得球门柱信息*/
int GoalSpecialist::getOurTeamNumber()
{
	int tjarkNum=(uint32)theRoboCupGameControlData->teams[0].teamNumber==TJArkTeamNum?0:1;
	return tjarkNum;
}

int GoalSpecialist::findDown2(int x, int y, unsigned char col, int yEnd)
{
	const int yStart = y;
	int tmp;

	for(y+=SkipOffset2; y < yEnd; y+=SkipOffset2)
	{
		if(convertColorAt(x,y) == col)
		{
			tmp = y - SkipOffset2;
			for(--y; y > tmp; y--)
				if(convertColorAt(x,y) != col)
					break;

			//if(y != tmp)
			++y;
			break;
		}
	}
	if(y > yEnd)
		y = yEnd;
	return y;

}


////scan goalkeeper
//std::pair<int,int> GoalSpecialist::scanGoalkeeper(int xStart,int xEnd,int yTop,int yBottom,unsigned char curColor)
//{
//	std::vector<std::pair<int,int> > headBandXCoordinate;
//	std::pair<int,int> GoalKeeper;
//	int RobotHaveChecked=0;
//	bool insideGoal=false;//判断门内是否有守门颜色的Run
//	Run singlekeeper;//用来处理守门员的run
//	std::vector<Run>::size_type index=curColor==PINK?theRegionpercept->PinkGoalKeeperRun.size():theRegionpercept->BlueGoalKeeperRun.size();
//	std::vector<Run> &GoalKeeperRun=curColor==PINK?theRegionpercept->PinkGoalKeeperRun:theRegionpercept->BlueGoalKeeperRun;
//	if(index==0)
//	{
//		GoalKeeper.first=-1;
//		GoalKeeper.second=-1;
//		//std::cout<<"goal keeper "<<"  "<<"left"<<"  "<<GoalKeeper.first<<"  "<<"right"<<"  "<<GoalKeeper.second<<std::endl;
//		return GoalKeeper;
//	}
//
//	for(std::vector<Run>::size_type i=0;i!=index;i++)
//	{
//	    if(GoalKeeperRun[i].start.x>RobotHaveChecked&&GoalKeeperRun[i].start.x<xEnd&&GoalKeeperRun[i].start.x>xStart&&GoalKeeperRun[i].start.y>yTop&&GoalKeeperRun[i].end.y<yBottom&&GoalKeeperRun[i].length<=maxHeadBandHeight)
//		{
//		   insideGoal=true;
//           singlekeeper=GoalKeeperRun[i];
//		   std::vector<int>headBandXMin;
//		   std::vector<int>headBandXMax;
//		   for (int j=singlekeeper.start.y;j<=singlekeeper.end.y;j++)
//		   {
//
//			   int xMin=runLeftGoal(singlekeeper.start.x,j,curColor,singlekeeper.start.x-headBand_gridStepSize);
//			   int xMax=runRightGoal(singlekeeper.start.x,j,curColor,xEnd);
//			   if ((xMax-xMin)>minHeadBandWidth)
//			   {
//				   headBandXMin.push_back(xMin);
//				   headBandXMax.push_back(xMax);
//			   }
//		   }
//		  // ASSERT(headBandXMax.size()==headBandXMin.size());
//		   if(headBandXMin.size()>=2&&headBandXMax.size()>=2)
//		   {
//			   int headBandXMaxAver=vectorAver(headBandXMax);
//			   int headBandXMinAver=vectorAver(headBandXMin);
//			   headBandXCoordinate.push_back(std::pair<int,int>(headBandXMinAver,headBandXMaxAver));
//			   DRAW_LINE(headBandXMinAver,singlekeeper.start.y+singlekeeper.length/2,headBandXMaxAver,singlekeeper.start.y+singlekeeper.length/2,2,Drawings::ps_SolidLine,Drawings::black);
//			    RobotHaveChecked =headBandXMaxAver;
//			   continue;
//		   }
//		   else{
//
//			   RobotHaveChecked=singlekeeper.start.x;
//			   continue;
//
//		   }
//
//		}
//	}
//	if(insideGoal==false)
//	{
//		GoalKeeper.first=-1;
//		GoalKeeper.second=-1;
//		//std::cout<<"goal keeper "<<"  "<<"left"<<"  "<<GoalKeeper.first<<"  "<<"right"<<"  "<<GoalKeeper.second<<std::endl;
//		return GoalKeeper;
//	}
//	if(headBandXCoordinate.size()==0)
//	{
//		GoalKeeper.first=-1;
//		GoalKeeper.second=-1;
//		return GoalKeeper;
//	}
//	else
//	{
//		std::vector<std::pair<int,int> >::size_type BandSize=headBandXCoordinate.size();
//		int min=headBandXCoordinate[0].first;
//		int max=headBandXCoordinate[0].second;
//		for (int i=1;i<BandSize;i++)
//		{
//			max=headBandXCoordinate[i].second>max?headBandXCoordinate[i].second:max;
//			min=headBandXCoordinate[i].first<min?headBandXCoordinate[i].first:min;
//		}
//		GoalKeeper.first=min;
//		GoalKeeper.second=max;
//		//std::cout<<"goal keeper "<<"  "<<"left"<<"  "<<GoalKeeper.first<<"  "<<"right"<<"  "<<GoalKeeper.second<<std::endl;
//		return GoalKeeper;
//
//	}
//
//}
//
//
//int GoalSpecialist::vectorMax(std::vector<int>& vec)
//{
//	int max=vec[0];
//	for (int i=1;i<vec.size();i++)
//		max=vec[i]>max?vec[i]:max;
//	return max;
//}
//int GoalSpecialist::vectorAver(std::vector<int>& vec)
//{
//	int max=0;
//	for (int i=0;i<vec.size();i++)
//		max+=vec[i];
//	return max/vec.size();
//}
//int GoalSpecialist::findDown2Goal(int x, int y, unsigned char col1, int yEnd)
//{
//	const int yStart = y;
//	int tmp;
//    unsigned char col;
//	for(y+=SkipOffset; y < yEnd; y+=SkipOffset)
//	{
//		col =convertColorAt(x,y);
//		if(col == col1)
//		{
//			tmp = y - SkipOffset;
//			for(--y; y > tmp; y--)
//				if(convertColorAt(x,y) != col)
//					break;
//
//			if(y != tmp)
//				++y;
//			break;
//		}
//	}
//	if(y > yEnd)
//		y = yEnd;
//
//	return y;
//}
//int GoalSpecialist::runDownGoal(int x, int y, unsigned char col, int yEnd)
//{
//	const int yStart = y;
//	int tmp;
//
//	for(y+=SkipOffset; y < yEnd; y+=SkipOffset)
//	{
//		if(convertColorAt(x,y) != col)
//		{
//			tmp = y - SkipOffset;
//			for(--y; y > tmp; y--)
//				if(convertColorAt(x,y) == col)
//					break;
//
//			if(y == tmp)
//			{
//				y++;
//				break;
//			}
//		}
//	}
//	if(y > yEnd)
//		y = yEnd;
//	return y;
//}
//int GoalSpecialist::runLeftGoal(int x,int y,unsigned char col,int xMin)
//{
//	const int xStart = x;
//	int tmp;
//
//	for(x -= SkipOffset; x > xMin; x -= SkipOffset)
//	{
//		if(convertColorAt(x,y) != col)
//		{
//			tmp = x + SkipOffset;
//			for(++x; x < tmp; x++)
//				if(convertColorAt(x,y) == col)
//					break;
//
//			if(x == tmp)
//			{
//				x--;
//				break;
//			}
//		}
//	}
//	if(x < xMin)
//		x = xMin;
//	return x;
//}
//
//int GoalSpecialist::runRightGoal(int x, int y, unsigned char col, int xMax)
//{
//	const int xStart = x;
//	int tmp;
//
//	for(x += SkipOffset; x < xMax; x += SkipOffset)
//	{
//		if(convertColorAt(x,y) != col)
//		{
//			tmp = x - SkipOffset;
//			for(--x; x > tmp; x--)
//				if(convertColorAt(x,y) == col)
//					break;
//
//			if(x == tmp)
//			{
//				x++;
//				break;
//			}
//		}
//	}
//	if(x > xMax)
//		x = xMax;
//	return x;
//}
void GoalSpecialist:: writeGoalLog()
{
	goaldebugcounter++;
	if(goaldebugcounter>=50&&goaldebugcounter<=4000)
	{
		std::string path = "/home/nao/naoqi/Temp/GoalResult.txt";
		std::ofstream outFile(path.c_str(),ios::app);
		if(theGoalPercept->posts[0].perceptionType==0)
			outFile<<"#G#  "<<"Left Opp" <<"   "<<theGoalPercept->posts[0].distanceType
			<<"   "<<theGoalPercept->posts[0].perceptionType<<"   "<<theGoalPercept->posts[0].positionOnField.x
			<<"   "<<theGoalPercept->posts[0].positionOnField.y<<"   "<<theGoalPercept->posts[0].timeWhenLastSeen<<"\n";
		if(theGoalPercept->posts[1].perceptionType==0)
			outFile<<"#G#  "<<"right Opp" <<"   "<<theGoalPercept->posts[1].distanceType
			<<"   "<<theGoalPercept->posts[1].perceptionType<<"   "<<theGoalPercept->posts[1].positionOnField.x
			<<"   "<<theGoalPercept->posts[1].positionOnField.y<<"   "<<theGoalPercept->posts[1].timeWhenLastSeen<<"\n";
		if(theGoalPercept->posts[2].perceptionType==0)
			outFile<<"#G#  "<<"Left Own" <<"   "<<theGoalPercept->posts[2].distanceType
			<<"   "<<theGoalPercept->posts[2].perceptionType<<"   "<<theGoalPercept->posts[2].positionOnField.x
			<<"   "<<theGoalPercept->posts[2].positionOnField.y<<"   "<<theGoalPercept->posts[2].timeWhenLastSeen<<"\n";
		if(theGoalPercept->posts[3].perceptionType==0)
			outFile<<"#G#  "<<"right Own" <<"   "<<theGoalPercept->posts[3].distanceType
			<<"   "<<theGoalPercept->posts[3].perceptionType<<"   "<<theGoalPercept->posts[3].positionOnField.x
			<<"   "<<theGoalPercept->posts[3].positionOnField.y<<"   "<<theGoalPercept->posts[3].timeWhenLastSeen<<"\n";
		if(theGoalPercept->unknownPosts[0].perceptionType==0)
			outFile<<"#G#  "<<"Unknown Opp" <<"   "<<theGoalPercept->unknownPosts[0].distanceType
			<<"   "<<theGoalPercept->unknownPosts[0].perceptionType<<"   "<<theGoalPercept->unknownPosts[0].positionOnField.x
			<<"   "<<theGoalPercept->unknownPosts[0].positionOnField.y<<"   "<<theGoalPercept->unknownPosts[0].timeWhenLastSeen<<"\n";
        if(theGoalPercept->unknownPosts[1].perceptionType==0)
			outFile<<"#G#  "<<"Unknown Own" <<"   "<<theGoalPercept->unknownPosts[1].distanceType
			<<"   "<<theGoalPercept->unknownPosts[1].perceptionType<<"   "<<theGoalPercept->unknownPosts[1].positionOnField.x
			<<"   "<<theGoalPercept->unknownPosts[1].positionOnField.y<<"   "<<theGoalPercept->unknownPosts[1].timeWhenLastSeen<<"\n";

			outFile<<"***********end this**************"<<"\n";
		outFile.close();

		/*std::string path = "/opt/naoqi/lib/naoqi/GoalResult.txt";
		std::ofstream outFile(path.c_str(),ios::app);
		outFile<<"#G#  "<<"Left Opp" <<"\t"<<theGoalPercept->posts[0].perceptionType<<"\t"<<theGoalPercept->posts[0].positionOnField.x
			<<"\t"<<theGoalPercept->posts[0].positionOnField.y<<"\n"
			<<"#G#  "<<"right Opp" <<"\t"<<theGoalPercept->posts[1].perceptionType<<"\t"<<theGoalPercept->posts[1].positionOnField.x
			<<"\t"<<theGoalPercept->posts[1].positionOnField.y<<"\n"
			<<"#G#  "<<"Unknown Opp" <<"\t"<<theGoalPercept->unknownPosts[0].perceptionType<<"\t"<<theGoalPercept->unknownPosts[0].positionOnField.x
			<<"\t"<<theGoalPercept->unknownPosts[0].positionOnField.y<<"\n"
			<<"***********end this**************"<<"\n";
		outFile.close();*/
	}

}

MAKE_KS(GoalSpecialist)
