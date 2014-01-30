#include "FreePartOfOpponentGoalModelProvider.h"
#include "Tools/Math/Range.h"
#include "Module/Common/Geometry.h"

void FreePartOfOpponentGoalProvider::update(FreePartOfOpponentGoalModel* theFreePartOfOpponentGoalModel)
{   
	//std::cout<<" *** in FreePartOfOpponentGoalProvider ***"<<std::endl;
	//�Է������ڳ����ϵľ���λ��
	Vector2<double> LeftOppGoalPostAbs=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline),double(theFieldDimensions->yPosLeftGoal));
	Vector2<double> RightOppGoalPostAbs=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline),double(theFieldDimensions->yPosRightGoal));
	//�ſ�Χ
	Range<double> oppGoallineRange = Range<double>(RightOppGoalPostAbs.y, LeftOppGoalPostAbs.y);
	int widthOfOppGoal = theFieldDimensions->yPosLeftGoal - theFieldDimensions->yPosRightGoal;
	double cellOnOppGoallineWidth = (double) widthOfOppGoal / NUM_OF_CELLS;//ÿ��cell����
	
	typedef std::vector<RobotPercept::Robot>::const_iterator CROBOT_IT;
	for (CROBOT_IT robot_it=theRobotPercept->robots.begin();robot_it!=theRobotPercept->robots.end();++robot_it)
	{ 
             Vector2<double> BallPositionAbs=relative2FieldCoord(*theRobotPose,theBallModel->estimate.position);
			 Vector2<double> RobotOnField;
			 //std::cout<<"** in for FreePartOfOpponentGoalProvider **"<<std::endl;
			  Geometry::calculatePointOnField(robot_it->lowestPx.x,robot_it->lowestPx.y,*theCameraMatrix,RobotOnField);
			 /*if(!Geometry::calculatePointOnField(robot_it->lowestPx.x,robot_it->lowestPx.y,*theCameraMatrix,RobotOnField));
			 {
				 continue;
			 } */
			 //������������������ϵ�µ�λ��
			 Vector2<double> RobotOnFieldDouble=Vector2<double>(RobotOnField.x,RobotOnField.y);
             //�����������ڳ�������ϵ�ľ���λ��			
			 Vector2<double> RobotOnFieldAbs=relative2FieldCoord(*theRobotPose,RobotOnFieldDouble);
			 std::cout<<"%%%%  RobotOnFieldAbs %%%%"<<RobotOnFieldAbs.x<<"  "<<RobotOnFieldAbs.y<<std::endl;
			 //��������������ڳ��صľ���λ��
			 Vector2<double> LeftEndRobotAbs=Vector2<double>(RobotOnFieldAbs.x,RobotOnFieldAbs.y+ROBOT_WIDTH/2);
			 //�����������Ҳ��ڳ��صľ���λ��
			 Vector2<double> RightEndRobotAbs=Vector2<double>(RobotOnFieldAbs.x,RobotOnFieldAbs.y-ROBOT_WIDTH/2);
			 // intersection of lines to these ends with opponent groundline
			 Vector2<double> leftIntersectOppGroundline, rightIntersectOppGroundline;
			
			 //������ľ���λ������
			 if(
				 Geometry::getIntersectionOfLines(Geometry::GTLine(theRobotPose->translation, LeftEndRobotAbs - theRobotPose->translation),  Geometry::GTLine((double) theFieldDimensions->xPosOpponentGroundline, 0, 0, 1), leftIntersectOppGroundline) &&
				 Geometry::getIntersectionOfLines(Geometry::GTLine(theRobotPose->translation, RightEndRobotAbs - theRobotPose->translation), Geometry::GTLine((double) theFieldDimensions->xPosOpponentGroundline, 0, 0, 1), rightIntersectOppGroundline)
				)
			 {
				 if(oppGoallineRange.isInside(leftIntersectOppGroundline.y)||oppGoallineRange.isInside(rightIntersectOppGroundline.y))
				 {
					 // clip both into the inside of the opponent goal, in case one of them is outside of it
					double leftIntersectOppGroundlineY  = oppGoallineRange.limit(leftIntersectOppGroundline.y);
					 double rightIntersectOppGroundlineY = oppGoallineRange.limit(rightIntersectOppGroundline.y);
					 // check for each cell if it lies within the line between the intersections
					 for(int i = 0; i < NUM_OF_CELLS -1; i++)
					 {
						 double leftCellEndY  = theFieldDimensions->yPosRightGoal + (i+1)*cellOnOppGoallineWidth;
						 double rightCellEndY = theFieldDimensions->yPosRightGoal + i*cellOnOppGoallineWidth;
						 if(
							 (Range<double>(rightIntersectOppGroundlineY, leftIntersectOppGroundlineY).isInside(leftCellEndY) ||
							 Range<double>(rightIntersectOppGroundlineY, leftIntersectOppGroundlineY).isInside(rightCellEndY)) &&
							 cellsOnOppGoalline[i] < MAX_VALUE
							 )
						 {
							 cellsOnOppGoalline[i] += RENEW_VALUE;
						 }
					 }//end for int i=0
				 }//end isInside

			 }//end if Geometry::getIntersectionOfLines
       
	}//end for CROBOT_IT robot_it
     
	//�������Ҹ��Ӳ���
	 cellsOnOppGoalline[0]=MAX_VALUE;
	 cellsOnOppGoalline[NUM_OF_CELLS-1]=MAX_VALUE;
	 //˥��δ��ռ�õĸ���
	 for(int i=0;i<NUM_OF_CELLS;i++) cellsOnOppGoalline[i] *= AGING_FACTOR; 
	 //�ҳ����ŵ�free part
     int CurrentFreeCellSize=0,CurrentFreeCellHigh=0,CurrentFreeCellLow=0;
	 int HithertoLargestFreeCellSize=-1,HithertoLargestFreeCellHigh=-1,HithertoLargestFreeCellLow=-1;
	 int OverlapCellsize=0;
	 int side=(theRobotPose->translation.y<=0)? 1:-1;
	 for (int i=side>0 ? 0:NUM_OF_CELLS-1;(side>0) ? (i<NUM_OF_CELLS):(i>=0);i+=side)
	 {
		 if(cellsOnOppGoalline[i]<IS_FREE_THRESHOLD)
		 {
			 CurrentFreeCellSize++;
			 if(Range<double>(lastFreePartLow,lastFreePartHigh).isInside(i)) OverlapCellsize++;
			 (side>0)? CurrentFreeCellHigh=i:CurrentFreeCellLow=i;
		 }
		 else
		 {   //�����һ֡������free part �뵱ǰ֡�нϴ��ص�����increase an bonus to currentFreeCellsize
			 if((OverlapCellsize>CurrentFreeCellSize/2 ? CurrentFreeCellSize+NEW_MAX_FREE_THRESHOLD:CurrentFreeCellSize)>HithertoLargestFreeCellSize+NEW_MAX_FREE_THRESHOLD/2)
			 {
				 HithertoLargestFreeCellSize=CurrentFreeCellSize;
				 HithertoLargestFreeCellHigh=CurrentFreeCellHigh;
				 HithertoLargestFreeCellLow=CurrentFreeCellLow;
			 }
			 CurrentFreeCellSize=0;
			 OverlapCellsize=0;
			 (side>0) ? (CurrentFreeCellLow=i+1):(CurrentFreeCellHigh=i-1);
		 }
	 }  
	 //�ҵ��˺��ʵ�free part
	if(HithertoLargestFreeCellSize>=MIN_LARGEST_SIZE)
	{
	    largestFreePartHigh=HithertoLargestFreeCellHigh;
		largestFreePartLow=HithertoLargestFreeCellLow;
		theFreePartOfOpponentGoalModel->IsFree=true;//������û�б�����
	}
	//δ�ҵ����ʵ�free part�������ű���ס������־λ��false����largestFreePart����Ϊ���������ҵ�cell
	else
	{
		largestFreePartHigh=NUM_OF_CELLS-1;
		largestFreePartLow=0;
		theFreePartOfOpponentGoalModel->IsFree=false;
	}
	   lastFreePartHigh=largestFreePartHigh;
	   lastFreePartLow=largestFreePartLow; //������һ֡��Ҫ��ֵ
	   
   //�������λ��
   Vector2<double> LeftFreePartAbs=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline),
	                                               double(theFieldDimensions->yPosRightGoal+(largestFreePartHigh+1)*cellOnOppGoallineWidth));
   Vector2<double> RightFreePartAbs=Vector2<double>(double(theFieldDimensions->xPosOpponentGroundline),
	                                                theFieldDimensions->yPosRightGoal+largestFreePartLow*cellOnOppGoallineWidth);
  theFreePartOfOpponentGoalModel->LeftEnd=LeftFreePartAbs;
  theFreePartOfOpponentGoalModel->RightEnd=RightFreePartAbs;
   //theFreePartOfOpponentGoalModel->LeftEnd=FieldCoord2relative(*theRobotPose,LeftFreePartAbs);
   //theFreePartOfOpponentGoalModel->RightEnd=FieldCoord2relative(*theRobotPose,RightFreePartAbs);

   //Geometry::calculateAnglesForPoint(theFreePartOfOpponentGoalModel->LeftEnd,*theCameraMatrix,theFreePartOfOpponentGoalModel->LeftShotAngle);
  // Geometry::calculateAnglesForPoint(theFreePartOfOpponentGoalModel->RightEnd,*theCameraMatrix,theFreePartOfOpponentGoalModel->RightShotAngle);
#ifdef NAO_DEBUG
   drawingOnField(*theFreePartOfOpponentGoalModel);
#endif
}
Vector2<double> FreePartOfOpponentGoalProvider::relative2FieldCoord(const RobotPose& rp,const Vector2<double>& realcoordOnField)
{
	return rp*realcoordOnField;
}
Vector2<double> FreePartOfOpponentGoalProvider::FieldCoord2relative(const RobotPose& rp, const Vector2<double>& FieldCoord)
{
	Vector2<double> relativeCoord;
	double distance = Geometry::distanceTo(rp, FieldCoord);
	double angle = Geometry::angleTo(rp, FieldCoord);
	relativeCoord.x = distance * cos(angle);
	relativeCoord.y = distance * sin(angle);
	return relativeCoord;
}
void FreePartOfOpponentGoalProvider::drawingOnField(const FreePartOfOpponentGoalModel& freePartOfOpponentGoalModel)
{
	// draw goal line black
	LINE(theFieldDimensions->xPosOpponentGroundline,
		theFieldDimensions->yPosRightGoal,
		theFieldDimensions->xPosOpponentGroundline,
		theFieldDimensions->yPosLeftGoal,
		50,
		LocDrawings::ps_solid,
		ColorRGBA(0,0,0)
		);//��ɫ
	// draw free parts
	int widthOfOppGoal = theFieldDimensions->yPosLeftGoal - theFieldDimensions->yPosRightGoal;
	double cellOnOppGoallineWidth = (double) widthOfOppGoal / NUM_OF_CELLS;
	for(int i = 0; i < NUM_OF_CELLS; i++)
	{
		// draw all free parts blue
		if(cellsOnOppGoalline[i] < IS_FREE_THRESHOLD)
		{
			LINE(
				theFieldDimensions->xPosOpponentGroundline,
				theFieldDimensions->yPosRightGoal + i*cellOnOppGoallineWidth,
				theFieldDimensions->xPosOpponentGroundline,
				theFieldDimensions->yPosRightGoal + (i+1)*cellOnOppGoallineWidth,
				50,
				LocDrawings::ps_solid,
			   ColorRGBA(5,36,250)
				);//��ɫ
		}
		// draw largest free part green
		if(Range<int>(largestFreePartLow, largestFreePartHigh).isInside(i))
		{
			LINE(
				theFieldDimensions->xPosOpponentGroundline,
				theFieldDimensions->yPosRightGoal + i*cellOnOppGoallineWidth,
				theFieldDimensions->xPosOpponentGroundline,
				theFieldDimensions->yPosRightGoal + (i+1)*cellOnOppGoallineWidth,
				30,
				LocDrawings::ps_solid,
				ColorRGBA(18,237,23)
				);//��ɫ
		}
	}
}
void FreePartOfOpponentGoalProvider::update(FreePartData *theFreePartData)
{
	theFreePartData->largestFreePartLow=largestFreePartLow;
	theFreePartData->largestFreePartHigh=largestFreePartHigh;
	for(int i=0;i<NUM_OF_CELLS;i++)
	{
		theFreePartData->cellsOnOppGoalline[i]=cellsOnOppGoalline[i];
	}
}
MAKE_KS(FreePartOfOpponentGoalProvider)