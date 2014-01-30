# -*- coding: utf-8 -*- 
import string
import os
import sys

import Tools.Constants as Constants
import Tools.MyMath as MyMath
from Tools.MyMath import DEG_TO_RAD
import Tools.Output as Output

##get information from C++
import PyCommon

##send information to C++
import PyMotion



class Behavior(object):
    """
    class brings all of information together and runs the behaviors
    """
    def __init__(self):
        self.common = PyCommon.common()
        self.motion = PyMotion.motion()
        self.output = Output.BehaviorLog(self)
        self.hardwareStop = False

        	
    def run(self):
        """
        Main control loop called every TIME_STEP milliseconds
        """
        #get the playnumber and teamnumber
##        self.output.writeLine("##########################begin to run Behavior Code")
##        self.output.writeLine2("chestButton = ",self.common.chestButton())
        self.output.printf("##########################begin to run Behavior Code")
        self.output.printf2("chestButton = ",self.common.chestButton())

        if self.common.chestButton() == 1:
            self.hardwareStop = True
##            self.output.writeLine("I am reset now for crazying!!!!")
            self.output.printf("I am reset now for crazying!!!!")
            self.resetPerform()
            
    def resetPerform(self):
        self.setStand()
        self.setHead(0,0,0)
            
    ######### send motion command to Actuator #########
    def setStand(self):
        command = PyMotion.WalkCmd(0,1,(0,0,0))
        self.motion.setWalkCmd(command)
        
    def setWalk(self, walkType, forward, left, turnCCW):
        command = PyMotion.WalkCmd(1,walkType,(forward, left, turnCCW))
        self.motion.setWalkCmd(command)      

    def setSpecialAction(self, specActionID):
        command = PyMotion.SpecialAction(specActionID)
        self.motion.setSpecialAction(command)
        
    def setHead(self, yaw, pitch, speed): 
        command = PyMotion.HeadCmd(DEG_TO_RAD(yaw), DEG_TO_RAD(pitch), speed)
        self.motion.setHeadCmd(command)
    ##################### End ########################

  
