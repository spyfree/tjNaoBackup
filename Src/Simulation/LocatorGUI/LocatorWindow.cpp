#include "LocatorWindow.h"
#include "Platform/GetSetting.h"
#include "Module/Modeling/ReadLogData.h"
#include <iostream>
#include <fstream>

LocatorWindow::LocatorWindow(QMdiArea *mdiArea)
:SubWindow(mdiArea,"WorldState"),speedValue(300),FlagOfBallSample(false),FlagOfRobotSample(false)
{
	theFieldDrawing = new FieldDrawing;
	theDrawManager = new DrawManager;
	createWindow();
	createConnect();
	
	//每隔speedValue时间刷新视图
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(updateLocView()));
	timer->start(speedValue);
}

LocatorWindow::~LocatorWindow()
{
	if (theDrawManager != NULL)
	{
		delete theDrawManager;
		theDrawManager = NULL;
	}
	if(theFieldDrawing != NULL)
	{
		delete theFieldDrawing;
		theFieldDrawing = NULL;
	}
}

void LocatorWindow::createWindow()
{
	speedLabel = new QLabel(tr("Update Speed (ms)"));
	speedSlider = new QSlider();
	speedSlider->setRange(300,3000);
	speedSlider->setSingleStep(1);
	speedSlider->setTracking(true);
	speedSlider->setOrientation(Qt::Horizontal);
	speedEdit = new QLineEdit();
	speedEdit->setText(tr("%1").arg(speedSlider->value()));
	QGridLayout* speedLayout = new QGridLayout();
	speedLayout->addWidget(speedLabel,0,0);
	speedLayout->addWidget(speedSlider,0,1);
	speedLayout->addWidget(speedEdit,0,2);
	QGridLayout* fieldLayout = new QGridLayout();
	fieldLayout->addWidget(theFieldDrawing);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(fieldLayout);
	mainLayout->addLayout(speedLayout);
	widget = new QWidget;
	widget->setLayout(mainLayout);
	setWidget(widget);
	setMinimumSize(QSize(570,820));

}

void LocatorWindow::createConnect()
{
	connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedSliderChanged()));
}

void LocatorWindow::Show()
{
	if(!isVisible())
	{	
		setVisible(!isVisible());
		widget->show();
	}
}

void LocatorWindow::updateLocView()
{
	theFieldDrawing->updateLocWidget();
}

void LocatorWindow::speedSliderChanged()
{
	//控制刷新视图速度的快慢
	speedValue = speedSlider->value();
	speedEdit->setText(tr("%1").arg(speedValue));
	timer->start(speedValue);
}
void LocatorWindow::getLocResult(char*buffer,int size)
{
	Robotsamplebuffer robotsamples;
	Ballsamplebuffer ballsamples;
	FreePartData freepartdata;
	//int i=size;
	memcpy(&robotsamples,buffer,sizeof(Robotsamplebuffer));
	memcpy(&ballsamples,buffer+sizeof(Robotsamplebuffer),sizeof(Ballsamplebuffer));
	memcpy(&freepartdata,buffer+sizeof(Robotsamplebuffer)+sizeof(Ballsamplebuffer),sizeof(FreePartData));
	if(FlagOfRobotSample)
	{ drawRobotSamples(robotsamples);
	  drawFreePart(freepartdata);
	}
	if(FlagOfBallSample)
	   drawBallSamples(ballsamples);
	
	theFieldDrawing->theLocMessageData->ellipses.clear();
	theFieldDrawing->theLocMessageData->ellipses = DrawBuffer::getInstance()->ellipses;
	theFieldDrawing->theLocMessageData->lines.clear();
	theFieldDrawing->theLocMessageData->lines = DrawBuffer::getInstance()->lines;
	theFieldDrawing->theLocMessageData->polygons.clear();
	theFieldDrawing->theLocMessageData->polygons = DrawBuffer::getInstance()->polygons;
	
	DrawBuffer::getInstance()->ellipses.clear();
	DrawBuffer::getInstance()->polygons.clear();
	DrawBuffer::getInstance()->lines.clear();
}
void LocatorWindow::drawRobotSamples(Robotsamplebuffer ss)
{
	const int numberOfSamples(100);
	const double maxWeighting = 2*ss.Robotsample[0].totalWeighting / numberOfSamples;
	for(int i = 0; i < numberOfSamples; ++i)
	{
		const Robotsamplebuffer::singleSample& s=ss.samples[i];
		const Pose2D pose(s.angle, s.translationX, s.translationY);
		unsigned char weighting = (unsigned char) (s.weighting / maxWeighting * 255);
		Vector2<double> bodyPoints[4] = {Vector2<double>(55,90),
			Vector2<double>(-55,90),
			Vector2<double>(-55,-90),
			Vector2<double>(55,-90)};
		for(int j = 0; j < 4; ++j)
		{
			bodyPoints[j] = Geometry::rotate(bodyPoints[j], pose.rotation);
			bodyPoints[j] += pose.translation;
		}
		Vector2<double> headPos(30,0);
		headPos = Geometry::rotate(headPos, pose.rotation);
		headPos += pose.translation;
		ColorRGBA color = s.weighting ? ColorRGBA(weighting,weighting,weighting) : ColorRGBA(255,0,0);
		if(s.InbestCluster == true)
			color = ColorRGBA(200,0,200);

		bool drawlikeGT=true;
		if(!drawlikeGT)
		{Vector2<double> start(pose.translation.x,pose.translation.y);
		Vector2<double> end(pose.translation.x+60*cos(pose.rotation),pose.translation.y+60*sin(pose.rotation));
		CIRCLE(pose.translation.x,pose.translation.y,30,20,LocDrawings::ps_solid,color,LocDrawings::bs_null,color);
		ARROW(start,end,10,LocDrawings::ps_solid,color);}
		else
		{
			POLYGON(4,bodyPoints,15,LocDrawings::ps_solid,color,LocDrawings::bs_solid,color);
			CIRCLE(headPos.x, headPos.y, 30, 1,
				LocDrawings::ps_solid, color, LocDrawings::bs_solid, color);
		}
	}

//画机器人的位置 selflocator的

	Vector2<double> bodyPoints[4] = {Vector2<double>(55,90),
		Vector2<double>(-55,90),
		Vector2<double>(-55,-90),
		Vector2<double>(55,-90)};
	for(int i=0; i<4; i++)
	{
		bodyPoints[i] = Geometry::rotate(bodyPoints[i], ss.Robotsample[0].Robotrotation);
		bodyPoints[i].x +=ss.Robotsample[0].RobotTranslationX;
		bodyPoints[i].y +=ss.Robotsample[0].RobotTranslationY;

	}
	ColorRGBA ownTeamColorForDrawing=ColorRGBA(ss.Robotsample[0].color.r,ss.Robotsample[0].color.g,ss.Robotsample[0].color.b);
	Vector2<double> dirVec(200,0);
	dirVec.rotate(ss.Robotsample[0].Robotrotation);
	dirVec.x += ss.Robotsample[0].RobotTranslationX;
	dirVec.y += ss.Robotsample[0].RobotTranslationY;
	LINE(ss.Robotsample[0].RobotTranslationX,ss.Robotsample[0].RobotTranslationY, dirVec.x, dirVec.y, 
		20, LocDrawings::ps_solid, ColorClasses::white);
	POLYGON( 4, bodyPoints, 20, LocDrawings::ps_solid, 
		ownTeamColorForDrawing, LocDrawings::bs_solid, ColorClasses::white);
	CIRCLE( ss.Robotsample[0].RobotTranslationX,ss.Robotsample[0].RobotTranslationY, 42, 1, 
		LocDrawings::ps_solid,ownTeamColorForDrawing, LocDrawings::bs_solid,ownTeamColorForDrawing);
	
	
	//画RobotPosevalidator后的
	Vector2<double> bodyPoints1[4] = {Vector2<double>(55,90),
		Vector2<double>(-55,90),
		Vector2<double>(-55,-90),
		Vector2<double>(55,-90)};
	for(int i=0; i<4; i++)
	{
		bodyPoints1[i] = Geometry::rotate(bodyPoints1[i], ss.Robotsample[1].Robotrotation);
		bodyPoints1[i].x +=ss.Robotsample[1].RobotTranslationX;
		bodyPoints1[i].y +=ss.Robotsample[1].RobotTranslationY;

	}
	ownTeamColorForDrawing=ColorRGBA(ss.Robotsample[1].color.r,ss.Robotsample[1].color.g,ss.Robotsample[1].color.b);
     dirVec=Vector2<double>(200,0);
	dirVec.rotate(ss.Robotsample[1].Robotrotation);
	dirVec.x += ss.Robotsample[1].RobotTranslationX;
	dirVec.y += ss.Robotsample[1].RobotTranslationY;
	LINE(ss.Robotsample[1].RobotTranslationX,ss.Robotsample[1].RobotTranslationY, dirVec.x, dirVec.y, 
		20, LocDrawings::ps_solid, ColorClasses::white);
	POLYGON( 4, bodyPoints1, 20, LocDrawings::ps_solid, 
		ownTeamColorForDrawing, LocDrawings::bs_solid, ColorClasses::white);
	CIRCLE( ss.Robotsample[1].RobotTranslationX,ss.Robotsample[1].RobotTranslationY, 42, 1, 
		LocDrawings::ps_solid,ownTeamColorForDrawing, LocDrawings::bs_solid,ownTeamColorForDrawing);

}
void LocatorWindow::getRobotsampleflag(int flag)
{
	if(flag)
		FlagOfRobotSample=true;
	else
		FlagOfRobotSample=false;
}
void LocatorWindow::getBallsampleflag(int flag)
{
	if(flag)
		FlagOfBallSample=true;
	else
		FlagOfBallSample=false;
}
void LocatorWindow::drawBallSamples(Ballsamplebuffer ss)
{
	//画BallModel
	Vector2<double> robotPose(ss.RobotTranslationX,ss.RobotTranslationY);

		Vector2<double> position(ss.BallModelToPc.estimateGlobe.positionX,ss.BallModelToPc.estimateGlobe.positionY);
		//position+=robotPose;
		Vector2<double> velocity(ss.BallModelToPc.estimate.velocityX,ss.BallModelToPc.estimate.velocityY);
		CIRCLE(
			position.x, 
			position.y, 
			40, 
			1, // pen width 
			LocDrawings::ps_solid, 
			ColorClasses::red,
			LocDrawings::bs_solid, 
			ColorClasses::red);
		ARROW(position, position + velocity.rotate(ss.RobotRotation), 5, 1, ColorClasses::red);

		Vector2<double> lastPosition(ss.BallModelToPc.lastSeenEstimate.positionX,ss.BallModelToPc.lastSeenEstimate.positionY);
		lastPosition=lastPosition.rotate(ss.RobotRotation)+robotPose;
		Vector2<double> lastVelocity(ss.BallModelToPc.lastSeenEstimate.velocityX,ss.BallModelToPc.lastSeenEstimate.velocityY);
		CIRCLE(
			lastPosition.x, 
			lastPosition.y, 
			45, 
			1, // pen width 
			LocDrawings::ps_solid, 
			ColorRGBA(0, 0, 0, 220),
			LocDrawings::bs_solid, 
			ColorRGBA(255, 128, 128, 220));
		ARROW(lastPosition, lastPosition + lastVelocity.rotate(ss.RobotRotation), 5, 1, ColorRGBA(255, 128, 0, 220));


		//画global info ball 


		CIRCLE(
			ss.BallParticleSamples[0].posX, 
			ss.BallParticleSamples[0].posY, 
			40, 
			1, // pen width 
			LocDrawings::ps_solid, 
			ColorClasses::black,
			LocDrawings::bs_solid, 
			ColorClasses::black);

        //画粒子
		//ColorRGBA sampleColor(255,255,255);
		////Determine alpha offset for nicer drawing ;-)
		//double alphaOffset(1.0);
		//for(int i = 0; i <40; ++i)
		//{
		//	const double offset(1.0 - ss.BallParticleSamples[i].weighting);
		//	if(offset < alphaOffset)
		//		alphaOffset = offset;
		//}
		//for(int i = 0; i < 40; ++i)
		//{
		//	const Ballsamplebuffer::singleBallSample s(ss.BallParticleSamples[i]);
		//	Vector2<double> p(s.posX,s.posY);
		//	Vector2<double> v(s.velocityX,s.velocityY);
		//	p=p.rotate(ss.RobotRotation)+robotPose;
		//	v=v.rotate(ss.RobotRotation);
		//	if(s.weighting != -1.0)
		//		sampleColor.a = (unsigned char)((s.weighting + alphaOffset)*255.0);
		//	else //Draw new samples in a different color:
		//		sampleColor = ColorRGBA(255,255,0);
		//	CIRCLE(p.x,p.y,40,10,LocDrawings::ps_solid,sampleColor,LocDrawings::bs_solid,sampleColor);
		//	if(s.state == BallSample::ROLLING)
		//	{
		//		Vector2<double> start(p.x,p.y);
		//		Vector2<double> end(p.x+v.x,p.y+v.y);
		//		ARROW( start, end,5,LocDrawings::ps_solid,sampleColor);
		//	}
		//}

}
void LocatorWindow::drawFreePart(FreePartData freepartdata)
{
	LINE(3000,
		-750,
		3000,
		750,
		50,
		LocDrawings::ps_solid,
		ColorRGBA(0,0,0)
		);//黑色
	// draw free parts
	int widthOfOppGoal = 1500;
	double cellOnOppGoallineWidth = (double) widthOfOppGoal / 14;
	for(int i = 0; i < 14; i++)
	{
		// draw all free parts blue
		if(freepartdata.cellsOnOppGoalline[i] < 0.2)
		{
			LINE(
				3000,
				-750 + i*cellOnOppGoallineWidth,
				3000,
				-750 + (i+1)*cellOnOppGoallineWidth,
				50,
				LocDrawings::ps_solid,
				ColorRGBA(5,36,250)
				);//蓝色
		}
		// draw largest free part green
		if(Range<int>(freepartdata.largestFreePartLow, freepartdata.largestFreePartHigh).isInside(i))
		{
			LINE(
				3000,
				-750 + i*cellOnOppGoallineWidth,
				3000,
				-750 + (i+1)*cellOnOppGoallineWidth,
				30,
				LocDrawings::ps_solid,
				ColorRGBA(18,237,23)
				);//绿色
		}
	}
}