import time
import Tools.Constants as Constants
from Tools.MyMath import RAD_TO_DEG

def FindBall(brain):
    if brain.ball.confidence > 0:
        return
    else:
        if brain.AlignGoalToFindBallMode == True:
            AlignBallAndGoalMode(brain)
        else:
            scanFindBall(brain)

def AlignBallAndGoalMode(brian):
    if brain.AlianGoalAndBallCounter < 5:
        brain.setHead(0,-15,0.5)
    else:
        brain.setHead(0,-5,0.5)
    brain.AlianGoalAndBallCounter += 1
    if brain.AlianGoalAndBallCounter == 9:
        brain.AlianGoalAndBallCounter = 0
    if brain.ball.confidence > 0:
        return  
    
def scanFindBall(brain):
    '''
    State to move the head to find the ball. If we find the ball, we
    move to align on it. If we don't find it, we rot to keep looking
    '''
    brain.output.printf2('~~~~~~~~~~~~~~~~~~~~begin to scan ball===',brain.scanFindBallCounter)
    if brain.scanFindBallCounter < 5:
        brain.output.printf('@@@@@@@@@@@@@@@@@@ssssssssssssssssssssssssssssssssss')
        brain.setHead(0,-15,0.3)
    elif brain.scanFindBallCounter < 10:
        brain.output.printf('22222222222222222222222222222222222222222222222222222')
        brain.setHead(0,15,0.3) 
    elif brain.scanFindBallCounter < 15:
        brain.setHead(70,-15,0.3)
    elif brain.scanFindBallCounter < 20:
        brain.setHead(0,-30,0.3)
        
    elif brain.scanFindBallCounter < 25:
        brain.setHead(-70,-15,0.3)       
    elif brain.scanFindBallCounter < 30:
        brain.setHead(0,20,0.3)
    elif brain.scanFindBallCounter < 35:
        brain.setHead(80,-20,0.3)
        
    elif brain.scanFindBallCounter < 40:
        brain.setHead(0,-40,0.3)
    elif brain.scanFindBallCounter < 45:
        brain.setHead(-80,-20,0.3)
    elif brain.scanFindBallCounter < 50:
        brain.setHead(-60,35,0.3)
    elif brain.scanFindBallCounter < 55:
        brain.setHead(60,35,0.3)
        
    brain.scanFindBallCounter += 1
    brain.beginRotFindCounter += 1
    if brain.scanFindBallCounter == 54:
        brain.scanFindBallCounter = 0
    if brain.ball.confidence > 0:
        brain.beginRotFindCounter = 0
        brain.scanFindBallCounter = 0
        return
    if brain.beginRotFindCounter > 40:
        rotFindBall(brain)
    else:
        brain.setWalk(1,0,0,0 )
       
        
    

def rotFindBall(brain):
    '''
    State to rot to find the ball. If we find the ball, we
    move to align on it. If we don't find it, we go to a garbage state
    '''
    ##first turnCCW
##    brain.setWalk(1,0,0,0.226)                                
    if brain.ball.confidence > 0:
        return


 
 
