from math import cos, sin, atan
from Skills.sFindBall import FindBall
import Skills.sTrackBall as sTrackBall
from Tools.MyMath import RAD_TO_DEG
from Tools.MyMath import DEG_TO_RAD
from Tools.MyMath import CLIP

class Attacker(object):
    def __init__(self,brain):
        self.brain = brain
        self.stopforgoal=0
        
    def doAttacker(self):
        self.brain.output.printf("begin to do attacker!")
        self.brain.output.printf2("doAttacker----ballSwitchGoal",self.brain.ballSwitchGoal)
        self.brain.output.printf2("doAttacker----AlignGoalToFindBallMode",self.brain.AlignGoalToFindBallMode)
        if self.brain.ball.confidence >0:
            self.brain.output.printf("doAttacker-------see ball!!!")
        
        if self.brain.ballSwitchGoal == True:
            self.brain.output.printf("doAttacker------AlignWithBallAndGoal")
            self.AlignWithBallAndGoal()
        else:
            self.brain.output.printf("doAttacker-------begin to run perform!!!")
            self.perform()

    def resetperform(self):
        self.brain.behavCounter = 0

    def perform(self):
        if self.brain.ball.confidence == 1:
            self.brain.output.printf("-------track for ball-------------")
            self.brain.beginRotFindCounter = 0
            self.TrackBall()
        else:
            self.brain.output.printf("ball is not seen, begin to find ball")
            FindBall(self.brain)
            self.stopforgoal=0

    def TrackBall(self):
        sTrackBall.TrackBall(self.brain)
        self.GoToBall()
                      
    def GoToBall(self):
        self.brain.output.printf(')))))))))))))))))))))))))GotoBall(((((((((((((((((((((((')
        self.brain.output.printf2('do-----ball.deg:::::::::::',RAD_TO_DEG(self.brain.ball.deg))
        forward = 0
        left = 0
        turnCCW = 0
        if abs(RAD_TO_DEG(self.brain.ball.deg)) < 15:
            if self.brain.ball.localX >400:
                forward = 0.02
            elif self.brain.ball.localX >250:
                forward = 0.015
            else:
                #self.brain.hardwareStop = True
                self.ApproachBall()
        else:
            turnCCW = CLIP(self.brain.ball.deg,DEG_TO_RAD(-13),DEG_TO_RAD(13))
            self.brain.output.printf2('~~~~~~~~~~~~~~~~~~~~~~~~~DEG is',RAD_TO_DEG(self.brain.ball.deg))
            self.brain.output.printf2('GoToBall-----turnCCW:::::::::::',turnCCW)
        self.brain.setWalk(1,forward,left,turnCCW)
            
	        
    def ApproachBall(self):
        self.brain.output.printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ApproachBall!!!!!!!!!!!!!!!")
        self.brain.output.printf2('########################################################approach local y is ',self.brain.ball.localY)
        if (self.brain.ball.localY<50 and self.brain.ball.localY>30) or\
               (self.brain.ball.localY>-50 and self.brain.ball.localY<-30):
            self.AlignWithBallAndGoal()
        else :
            if self.brain.ball.localY<-50:
                self.brain.setWalk(1,0,-0.005,0)
            elif self.brain.ball.localY>50:
                self.brain.setWalk(1,0,0.005,0)
            elif (self.brain.ball.localY>-30 and self.brain.ball.localY<0):
                self.brain.setWalk(1,0,0.005,0)
            else :
                self.brain.setWalk(1,0,-0.005,0)
	            
    def AlignWithBallAndGoal(self):
        self.brain.ballSwitchGoal = True
        self.brain.output.printf("AlignWithBallAndGoal")
        if self.brain.vtGoal.leftDeg * self.brain.vtGoal.rightDeg < 0:
            if self.brain.voGoal.confidence==0:
                nothing=0
            else :
                self.brain.seegoal=0
                return 
                
        
            if self.brain.ball.confidence>0:
                self.brain.ballSwitchGoal = False
                self.brain.setHead(0,-20,0.2)
                if self.brain.ball.localX>185:
                    self.brain.setWalk(1,0.02,0,0)
                else :
                    self.KickGoal()
                    return
            else:
                self.brain.seeballcounter+=1
                if self.brain.seeballcounter <5:
                    self.brain.setHead(0,30,0.5)
                else:
                    self.brain.setHead(0,0,0.5)
                self.brain.setWalk(1,0,0,0)
                if self.brain.seeballcounter>10:
                    self.brain.seeballcounter=0
                    self.brain.seegoal=0
        else:
            self.AlignWithGoal()
            
    def AlignWithGoal(self):
        self.brain.setHead(0,-45,0.2)
        self.brain.output.printf2("AlignWithGoal-----vtGoal",self.brain.vtGoal.confidence)
        if self.brain.vtGoal.confidence ==0:
            self.brain.roundcounter+=1
            if self.brain.roundcounter<30:
                self.brain.setWalk(1,0,0,0.226)
            elif self.brain.roundcounter<60:
                self.brain.setWalk(1,0,0,-0.226)
            if self.brain.roundcounter  == 59:
                self.brain.roundcounter=0
        else:
            if self.brain.vtGoal.leftDeg > 0 and self.brain.vtGoal.rightDeg > 0:
                self.brain.setWalk(1,0,0.015,0)
            elif self.brain.vtGoal.leftDeg < 0 and self.brain.vtGoal.rightDeg < 0:
                self.brain.setWalk(1,0,-0.015,0)

    
    def AvoidOwnGoal(self):
        pass
        
    def KickGoal(self):
        self.brain.output.printf("KickGoal")
        if self.brain.ball.localY>0:
            self.brain.setSpecialAction(7)
        else :
            self.brain.setSpecialAction(8)
        self.brain.specialcounter= 25
        self.brain.output.printf("end KickBall----Finish all Behavior")
       
##    def getShootingPos(self, dist = 50):
##        """
##        return the forward where to position from ball position
##        puts the robot in line with ball and goal center
##        dist for HipOffsetY, usually is 50mm
##        """
##        ## destX, destY, destZ is the global coordinate
##        destX, destY, destH = 0,0,0
##        k = (Constants.OPP_GOALBOX_TOP_Y - self.brain.ball.y)/(Constants.MIDFIELD_X - self.brain.ball.x)
##        destX = self.brain.ball.x - dist * cos(atan(k))
##        destY = self.brain.ball.y - dist * sin(atan(k))
##        if k == 0:
##            destH = 0
##        else:
##            destH = MyMath.getRelativeHeading(destX, destY, self.brain.ball.x, self.brain.ball.y)
##        return destX, destY, destH

	                
	  
