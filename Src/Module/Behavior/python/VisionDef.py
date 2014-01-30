# -*- coding: utf-8 -*- 
import time
 
import Tools.Constants as Constants
import math

'''类MyInfo暂时不用，加入定位模块后再使用'''
class MyInfo:
    """
    Class just to store important information about me
    """
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.h = 0.0
        self.penalized = False
        self.kicking = False
 
    def updateLoc(self, loc):
        if self.teamColor == Constants.TEAM_BLUE:
            self.x = loc.x
            self.y = loc.y
            self.h = loc.h
        else:
            self.x = Constants.FIELD_GREEN_WIDTH - loc.x
            self.y = Constants.FIELD_GREEN_HEIGHT - loc.y
            self.h = sub180Angle(loc.h - 180)
 
 
class Ball:
    """
    Class for holding all current Ball information
    """
    def __init__(self, visionBall):
        
        self.ballCenterX = 0 # ball x coordinate in the image
        self.ballCenterY = 0 # ball y coordinate in the image
        self.radius = 0
        self.ballPer = 0
        self.localX = 0
        self.localY = 0
        self.localZ = 0
        self.confidence = 0
        self.dist = 0
        self.deg = 0
        self.framesOn = 0
        self.framesOff = 0
        
        self.x = 0
        self.y = 0
        self.velX = 0
        self.velY = 0
         
        self.lastVisionDist = 0
        self.lastVisionDeg = 0
        self.lastVisionCenterX = 0
        self.lastVisionCenterY = 0
        self.lastLocalX = 0
        self.lastLocalY = 0
        self.lastLocalZ = 0
        self.lastradius = 0
        self.lastballper= 0
        self.lastTimeSeen = 0

        self.ballseen = 0
 
        self.updateVision(visionBall)
 
    def updateVision(self,visionBall):
        '''update method gets list of vision updated information'''
        self.ballcf = visionBall.ballCf()
        
        if self.ballcf == 1 :
            '''看到球之后的ballseen帧数任然认为看到球了'''
            self.ballseen = 2 
            # Now update to the new stuff
            self.ballCenterX = visionBall.ballCenterX()
            self.ballCenterY = visionBall.ballCenterY()
            self.localX = visionBall.ballX()
            self.localY = visionBall.ballY()
            self.localZ = visionBall.ballZ()
            self.dist = math.sqrt(self.localX*self.localX + self.localY*self.localY)
            self.ballper = visionBall.ballPer()
            self.radius = visionBall.radius()
            if self.localY == 0:
                self.deg = 0
            elif self.localY > 0:
                self.deg = math.atan(self.localY / self.localX)
            else:
                self.deg = math.atan(self.localY / self.localX)
        
            # Hold our history
            self.lastVisionDist = self.dist
            self.lastVisionDeg = self.deg
            self.lastVisionCenterX = self.ballCenterX
            self.lastVisionCenterY = self.ballCenterY
            self.lastLocalX = self.localX
            self.lastLocalY = self.localY
            self.lastLocalZ = self.localZ
            self.lastradius = self.radius
            self.lastballper = self.ballper

            self.confidence = 1

            
        else:
            if self.ballseen > 0 :
                self.ballseen = self.ballseen - 1
                self.ballCenterX = self.lastVisionCenterX
                self.ballCenterY = self.lastVisionCenterY
                self.localX = self.lastLocalX
                self.localY = self.lastLocalY
                self.localZ = self.lastLocalZ
                self.deg = self.lastVisionDeg
                self.dist = self.lastVisionDist
                self.ballper = self.lastballper
                self.radius = self.lastradius

                self.confidence = 1
            else:
                self.confidence = 0
            

 	
        if self.confidence > 0:
            self.framesOn += 1
            self.framesOff =0
        else:
            self.framesOff += 1
            self.framesOn =0
 
            self.reportBallSeen()

 
    def reportBallSeen(self):
        """
        Reset the time since seen.  Happens when we see a ball or when
        a teammate tells us he did.
        """
        self.lastTimeSeen = time.time()
 
    def timeSinceSeen(self):
        """
        Update the time since we last saw a ball
        """
        return time.time() - self.lastTimeSeen
 
 	'''定位模块加入了再使用'''
    def updateLoc(self, loc, teamColor):
        """
        Update all of our inforamtion pased on the newest localization info
        """
        # Get latest estimates
        if teamColor == Constants.TEAM_BLUE:
            self.x = loc.ballX
            self.y = loc.ballY
            self.velX = loc.ballVelX
            self.velY = loc.ballVelY
        else:
            self.x = Constants.FIELD_WIDTH - loc.ballX
            self.y = Constants.FIELD_HEIGHT - loc.ballY
            self.velX = -loc.ballVelX
            self.velY = -loc.ballVelY

 
 
class YGoal:
    def __init__(self, visionGoal):
        '''initialization all values for FieldObject() class'''
        self.goalcolor = None
        self.dist = 0
        self.distCert = 0
        self.leftDeg = 0
        self.rightDeg = 0
        self.confidence = 0
        self.IdCert = 0
        self.framesOn = 0
        self.framesOff = 0
               

        # Setup the data from vision
        self.updateVision(visionGoal)
 
    def updateVision(self, visionGoal):
        '''updates class variables with new vision information'''
        self.dist = visionGoal.ygdist()
        self.leftDeg = visionGoal.ygldeg()
        self.rightDeg = visionGoal.ygrdeg()
        self.confidence = visionGoal.ygoalcf()
        self.goalcolor = visionGoal.ygoalColor()
        self.distCert = visionGoal.ygdistCert()
        self.IdCert = visionGoal.yIdCert()
        
 
        # obj is in this frame
        if self.confidence > 0:
            self.framesOn += 1
            self.framesOff = 0
        # obj not in this frame
        else:
            self.framesOff += 1
            self.framesOn = 0

class BGoal:
    def __init__(self, visionGoal):
        '''initialization all values for FieldObject() class'''
        self.goalcolor = None
        self.dist = 0
        self.distCert = 0
        self.leftDeg = 0
        self.rightDeg = 0
        self.confidence = 0
        self.IdCert = 0
        self.framesOn = 0
        self.framesOff = 0
               

        # Setup the data from vision
        self.updateVision(visionGoal)
 
    def updateVision(self, visionGoal):
        '''updates class variables with new vision information'''
        self.dist = visionGoal.bgdist()
        self.leftDeg = visionGoal.bgldeg()
        self.rightDeg = visionGoal.bgrdeg()
        self.confidence = visionGoal.bgoalcf()
        self.goalcolor = visionGoal.bgoalColor()
        self.distCert = visionGoal.bgdistCert()
        self.IdCert = visionGoal.bIdCert()
 
        
 
        # obj is in this frame
        if self.confidence > 0:
            self.framesOn += 1
            self.framesOff = 0
        # obj not in this frame
        else:
            self.framesOff += 1
            self.framesOn = 0    

  
    


