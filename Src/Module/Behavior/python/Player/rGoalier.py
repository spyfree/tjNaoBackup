#import Tools.Indicator as Indicator
from math import cos
from Skills.sFindBall import FindBall
import time
import Tools.Constants as Constants
from Tools.MyMath import RAD_TO_DEG

class Goalier(object):
    def __init__(self,brain):
        self.brain = brain
        self.gScanFindBallCounter = 0
    
    def doGoalier(self):
        self.perform()
            
    def perform(self):
        self.brain.setHead(0,-45,0.1)
##        if self.brain.ball.confidence > 0:
##            self.brain.output.printf("------------I see the Ball")
##            yaw =self. brain.common.headYaw()
##            pitch = self.brain.common.headPitch()
##            self.brain.setHead(yaw,pitch,0)
##            #self.TrackBall()
##        else :
##            self.brain.output.printf("----------I haven't seen the ball")
##            self.FindBall()
      
                    
    def TrackBall(self):
        bBoxURY = self.brain.ball.ballCenterY + self.brain.ball.radius
        IMAGE_WIDTH = Constants.IMAGE_WIDTH
        IMAGE_HEIGHT = Constants.IMAGE_HEIGHT
        FIELD_VIEW_H = Constants.FIELD_VIEW_H
        FIELD_VIEW_V = Constants.FIELD_VIEW_V
        if self.brain.ball.confidence > 0:
            delta_yaw  = -1.0 * (FIELD_VIEW_H) * \
                     (self.brain.ball.ballCenterX - IMAGE_WIDTH/2.0)/IMAGE_WIDTH 
            delta_pitch = 1.0 * (FIELD_VIEW_V) * \
                     (bBoxURY - IMAGE_HEIGHT/2.0)/IMAGE_HEIGHT
        else:
            delta_yaw=0
            delta_pitch =0
        yaw =self. brain.common.headYaw() + delta_yaw
        pitch = self.brain.common.headPitch() + delta_pitch
        print 'tarck ball over',delta_yaw,yaw
        self.brain.setHead(RAD_TO_DEG(yaw), RAD_TO_DEG(pitch), 1.0)
        
    def FindBall(self):
##        self.brain.output.writeLine2("gScanFindBallCounter=",self.gScanFindBallCounter)
        self.brain.output.printf2("gScanFindBallCounter=",self.gScanFindBallCounter)
        self.gScanFindBallCounter += 1

        if self.gScanFindBallCounter < 5:
            self.brain.setHead(0,-45,0.5)
        elif self.gScanFindBallCounter < 10:
            self.brain.setHead(70,-45,0.5)
        elif self.gScanFindBallCounter < 15:
            self.brain.setHead(0,-45,0.5)
        elif self.gScanFindBallCounter < 20:
            self.brain.setHead(-70,-45,0.5)     

                       
        if self.gScanFindBallCounter == 20:
            self.gScanFindBallCounter = 0
           
##        if self.gScanFindBallCounter == 39:
##        
##        if self.gScanFindBallCounter < 5:
##            self.brain.setHead(0,-15,0.5)
##        elif self.gScanFindBallCounter < 10:
##            self.brain.setHead(70,15,0.5)
##        elif self.gScanFindBallCounter < 15:
##            self.brain.setHead(0,-30,0.5)
##        elif self.gScanFindBallCounter < 20:
##            self.brain.setHead(-70,15,0.5)     
##        elif self.gScanFindBallCounter < 25:
##            self.brain.setHead(0,-20,0.5)
##        elif self.gScanFindBallCounter < 30:
##            self.brain.setHead(90,20,0.5)
##        elif self.gScanFindBallCounter < 35:
##            self.brain.setHead(0,-40,0.5)
##        else:
##            self.brain.setHead(-90,20,0.5)
##           
##        if self.gScanFindBallCounter == 39:
##            self.gScanFindBallCounter = 0
        if self.brain.ball.confidence > 0:
            return
        self.brain.setWalk(1,0,0,0)
        
