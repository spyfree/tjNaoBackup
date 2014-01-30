import Tools.MyMath as MyMath
import Tools.Constants as Constants
from Tools.MyMath import RAD_TO_DEG

        
def TrackBall(brain):
    
    bBoxURY = brain.ball.ballCenterY - brain.ball.radius #should be -
    IMAGE_WIDTH = Constants.IMAGE_WIDTH
    IMAGE_HEIGHT = Constants.IMAGE_HEIGHT
    FIELD_VIEW_H = Constants.FIELD_VIEW_H
    FIELD_VIEW_V = Constants.FIELD_VIEW_V
    if brain.ball.confidence > 0:
        brain.trackcounter=1
        ##yaw(left+,right-),pitch(down+,up-)
        delta_yaw  = -1.0 * (FIELD_VIEW_H) * \
                     (brain.ball.ballCenterX - IMAGE_WIDTH/2.0)/IMAGE_WIDTH 
        delta_pitch = 1.0 * (FIELD_VIEW_V) * \
                     (bBoxURY - IMAGE_HEIGHT/2.0)/IMAGE_HEIGHT
    else:
        delta_yaw=0
        delta_pitch =0
    yaw =brain.common.headYaw() + delta_yaw
    pitch = brain.common.headPitch() + delta_pitch
    print '$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$delta_yaw should be ',delta_yaw
    print '^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^delta_pitch should be ',delta_pitch
##    brain.setHead(RAD_TO_DEG(yaw), RAD_TO_DEG(pitch), 0.3)
##    brain.trackcounter-=1
    if abs(delta_yaw) > 0.02 or abs(delta_pitch) > 0.02:
        brain.setHead(RAD_TO_DEG(yaw), RAD_TO_DEG(pitch), 0.3)
    #elif abs(delta_pitch) > 0.02 :
        #brain.setHead(RAD_TO_DEG(brain.common.headYaw()), RAD_TO_DEG(pitch), 0.5)
    else:
        brain.setHead(RAD_TO_DEG(brain.common.headYaw()), RAD_TO_DEG(brain.common.headPitch()), 0)
            
    brain.trackcounter-=1
            
        

