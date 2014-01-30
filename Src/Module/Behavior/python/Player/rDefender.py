from math import cos, sin, atan, pi,sqrt
from Skills.sFindBall import FindBall
import Skills.sTrackBall as sTrackBall
from Skills.sFindBall import FindBall
import Tools.Constants as Constants
from Tools.MyMath import RAD_TO_DEG
from Tools.MyMath import DEG_TO_RAD
from Tools.MyMath import CLIP

class Defender(object):
    def __init__(self,brain):
        self.brain = brain
        self.gScanFindBallCounter = 0
        self.R_X = 0#R means robot
        self.R_Y = 0
        self.R_Theta = 0
        self.B_X = self.brain.ball.ballCenterX#B means ball
        self.B_Y = self.brain.ball.ballCenterY
        self.F_X = 0#F means field(pitch)
        self.XM = m#m is the x-line which the robot should go,which is needed to be set
        self.X_M = 0
        self.Radius = r#r need to be set
        self.turnDeg = t#t need to be test and set
        self.findball = 0
        self.penalty = self.gcData.Penalty()
        self.vision = PyVision.vision()
        self.findBallCounter = 0
        
    self.R_X = self.brain.x
    self.R_Y = self.brain.y
    self.R_Theta = self.brain.theta#visiondef?
    

    def Run(self):
        if self.gcData.Tcolor() == Constants.TEAM.BLUE:
            self.F_X = Constants.FIELD_LENGTH / 2
            self.X_M = Constants.FIELD_LENGTH /2 - XM
        else:
            self.F_X = -Constants.FIELD_LENGTH / 2
            self.X_M = -(Constants.FIELD_LENGTH /2 - XM)
        
            
    def doDefender(self):
        self.brain.output.printf("begin to do defender")
        if self.brain.ball.confidence >0:
            self.brain.output.printf("doDefender---------see ball!")
        if self.penalized == False:#and Goalier.self = True
            self.brain.output.printf("doDefender---------begin to run perform")
            self.perform()

    def GoToPoint(self):
        self.brain.output.printf("go to point")
        PointY = self.B_Y + self.B_Y * (self.B_X - self.X_M)/(self.B_X - self.F_X)
        deltaX = self.R_X - self.X_M
        deltaY = abs(PointY - self.R_Y)
        deltaTheta = R_Theta - (pi - atan(deltaY/deltaX)) 
        dist = sqrt(deltaY * deltaY + deltaX * deltaX)
        forward = 0
        left = 0
        turnCCW = 0
        if dist > self.Radius:
            if R_Theta < pi - atan((deltaY + self.Radius)/deltaX):
                turnCCW = deltaTheta * sin(deltaTheta)#turnCCW=CLIP(deltaTheta,DEG_TO_RAD(-20),DEG_TO_RAD(20))
                self.brain.output.printf2('GoToPoint------turnCCW:::::::',turnCCW)
                self.brain.setWalk(1,0,0,turnCCW)
            elif R_Theta > pi - atan((deltaY - self.Radius)/deltaX):
                turnCCW = -deltaTheta * sin(deltaTheta)#rurnCCW=CLIP(-deltaTheta,DEG_TO_RAD(-20),DEG_TO_RAD(20))
                self.brain.output.printf2('GoToPoint------turnCCW:::::::',turnCCW)
                self.brain.setWalk(1,0,0,turnCCW)
            else:
                self.brain.setWalk(1,0,0,0)
            forward = dist * sin(dist)#or forward = 0.02 can be adopt
            left = dist * sin(dist)#or left = 0.02 can be adopt
            turnCCW = 0
            self.brain.setWalk(1,forward,left,turnCCW)

    def TurnToBall(self):
        self.brain.output.printf("TurnToBall")
        if self.brain.ball.confidence > 0:
            if self.gcData.Tcolor() == Constants.TEAM.RED:
                alpha = self.R_Theta - atan((self.B_Y - self.R_Y)/(self.B_X - self.R_X))
                if abs(alpha) > self.turnDeg:#self.turnDeg is the minimum degree that needs to turn
                    if alpha < 2*pi - alpha:
                        turnCCW = -alpha * sin(alpha)
                        self.brain.setWalk(1,0,0,turnCCW)
                    else:
                        turnCCW = (2*pi - alpha)*sin(2*pi - alpha)
                        self.brain.setWalk(1,0,0,turnCCW)

            else:
                alpha = pi - self.R_Theta -atan((self.B_Y - self.R_Y)/(self.R_X - self.R_X - self.B_X))
                if abs(alpha) >self.turnDeg:
                    if alpha <2*pi - alpha:
                        turnCCW = alpha * sin(alpha)
                        self.brain.setWalk(1,0,0,turnCCW)
                    else:
                        turnCCW = -alpha*sin(-alpha)
                        self.brain.setWalk(1,0,0,turnCCW)

        else:
            FindBall(self.brain)

    def Squat(brain):
        brain.setSpecialAction(Constants.SPECACTIONID['Squat'])#have not realized yet

    def scanFindBall(brain):
        if self.findBallCounter < 16:
            self.brain.setHead(90,-15,0.5)#pitch=-15? need test
        elif self.findBallCounter < 32:
            self.brain.setHead(-90,-15,0.5)

        self.findBallCounter += 1
        if self.findBallCounter == 31:
            self.findBallCounter = 0
        

    def perform(self):
        scanFindBall(brain)
        if self.brain.ball.confidence > 0:
            self.brain.output.printf("------go to point------")
            self.GoToPoint(self)
        else:
            self.brain.output.printf("ball is not seen,begin to find ball")
            FindBall(self.brain)
                               
        if abs(self.R_X - self.X_M) <= self.Radius:
            self.brain.output.printf("--------turn to ball--------")
            TurnToBall(self)
            Squat(brain)
                               
    

            
            

    
