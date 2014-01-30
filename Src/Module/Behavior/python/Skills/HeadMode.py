# -*- coding: utf-8 -*- 
from math import cos, sin, atan
from Tools.MyMath import DEG_TO_RAD, ABS_MAX
import Tools.Constants as Constants
import sTrackBall

def headMode(brain, headControlMode):
    if headControlMode == Constants.SEARCH_BALL:
        searchBall(brain)
##    elif headControlMode == Constants.LOOK_UP_AND_DOWN:
##        lookUpAndDown(brain)
##    elif headControlMode == Constants.LOOK_STRAIGHTA_HEAD:
##        lookStraightAhead(brain)
##    elif headControlMode == Constants.LOOK_DOWN:
##        lookDown(brain)
##    elif headControlMode == Constants.LOOK_AT_BALL_AND_GOAL:
##        lookAtBallAndGoal(brain)
##    elif headControlMode == Constants.LOOK_AROUND:
##        lookAround(brain)



def searchBall(brain):
    brain.ball.updateVision(brain.vision)
    print '***************************************************************ball.confidence',brain.ball.confidence
    if brain.ball.confidence == 0 :

        if brain.HMSearchBallCounter < 5:
            brain.setHead(0,-15,0.5)
        elif brain.HMSearchBallCounter < 10:
            brain.setHead(0,15,0.5) 
        elif brain.HMSearchBallCounter < 15:
            brain.setHead(70,-15,0.5)
        elif brain.HMSearchBallCounter < 20:
            brain.setHead(0,-30,0.5)
        
        elif brain.HMSearchBallCounter < 25:
            brain.setHead(-70,-15,0.5)       
        elif brain.HMSearchBallCounter < 30:
            brain.setHead(0,20,0.5)
        elif brain.HMSearchBallCounter < 35:
            brain.setHead(80,-20,0.5)
        
        elif brain.HMSearchBallCounter < 40:
            brain.setHead(0,-40,0.5)
        elif brain.HMSearchBallCounter < 45:
            brain.setHead(-80,-20,0.5)
        elif brain.HMSearchBallCounter < 50:
            brain.setHead(-60,35,0.5)
        elif brain.HMSearchBallCounter < 55:
            brain.setHead(60,35,0.5)
        
        brain.HMSearchBallCounter +=1
        if brain.HMSearchBallCounter == 54:
            brain.HMSearchBallCounter = 0
    else:
        print '*****************************************************************************************trackball'
        sTrackBall.TrackBall(brain)
        
        
    return

