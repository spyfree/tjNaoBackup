# -*- coding: utf-8 -*- 
import string
import os
import sys

import Skills.sStandUp as sStandUp

import Tools.Constants as Constants
import Tools.MyMath as MyMath
from Tools.MyMath import DEG_TO_RAD
import Tools.Output as Output

import VisionDef
import GameStates
import GameController

import Player.rGoalier as rGoalier
import Player.rAttacker as rAttacker
import Player.rSupporter as rSupporter
import Player.rDebuger as rDebuger

##get information from C++
import PyVision
import PyCommon
import PyGameControl
##send information to C++
import PyMotion
import PyTeamCfg
import PyCamera


class Behavior(object):
    """
    class brings all of information together and runs the behaviors
    """
    def __init__(self):
	self.teamnumber = Constants.TEAM_BLUE
	self.playnumber = 0
        self.seegoal=0
        self.seeballcounter=0
        self.ballSwitchGoal = False
        self.AlignGoalToFindBallMode = False
        self.AlianGoalAndBallCounter = 0
        ##global variable for sFindBall.py
	self.scanFindBallCounter = 0
	self.beginRotFindCounter=0
        ##global variable for HeadMode.py
	self.HMSearchBallCounter = 0
        ##for specialAction, When specialAction is running, it must run about 28 times
        
        self.findgoal=0
	self.vtGoal = None
        self.voGoal = None
        self.standswitch=0
        self.readyKick=0
        
        self.vision = PyVision.vision()
        self.common = PyCommon.common()
        
        self.motion = PyMotion.motion()
        self.teamCfg = PyTeamCfg.TeamCfg()
        self.camera = PyCamera.camera()
        self.gcData = PyGameControl.gamecontrol()
        
        self.gamestate = 0
        self.penalty = 0
        self.SecstillUnpenal = 0

        self.ball = VisionDef.Ball(self.vision)
        self.yGoal = VisionDef.YGoal(self.vision)
        self.bGoal = VisionDef.BGoal(self.vision)
        
        self.rGoalier = rGoalier.Goalier(self)
        self.rAttacker = rAttacker.Attacker(self)
        self.rSupporter = rSupporter.Supporter(self)
        self.rDebuger = rDebuger.Debuger(self)

        self.gameController = GameController.GameController(self)
        self.output = Output.Output()

        self.fallDownCheck = True
        self.hardwareStop = False
        self.leftReset = False
        
        self.specialcounter=0
        self.trackcounter=0
        self.roundcounter=0
        self.fallendown=0

        self.motionID = -1
        self.walkType = -1
        self.forward = 0
        self.left = 0
        self.turnCCW = 0
        self.specActionID = -1

        	
    def run(self):
        """
        Main control loop called every TIME_STEP milliseconds
        """
        #get the playnumber and teamnumber
        self.output.printf("##########################begin to run Behavior Code")
        self.teamnumber,self.playnumber = self.getTeamcfg()
        #self.output.printf2("motionID = ",self.motionID)
        #self.output.printf2("hardwareStop = ",self.hardwareStop)
        self.setTeamcfg()
        self.gamestate = 3 #self.gcData.GameState()
        self.penalty = self.gcData.Penalty()
        self.SecstillUnpenal = self.gcData.SecstillUnpenal()
        if self.common.chestButton() == 1:
            self.hardwareStop = True
            self.output.printf("I am reset now for crazying!!!!")
            self.resetPerform()
        if self.hardwareStop == False:
            self.gameController.run()

    def perform(self):
        ###### update Vision information
        self.ball.updateVision(self.vision)
        self.yGoal.updateVision(self.vision)
        self.bGoal.updateVision(self.vision)
        self.updateGoalObjects()
       
        # the entrance of the behavior procedure
        if self.playnumber == Constants.ROLE_GOALIER:
            self.output.printf("========================I am  Goalier")
            if sStandUp.FallDownCheck(self) and self.fallDownCheck:
                self.output.printf("Goalier is falling down")
                self.specialcounter = 28
                self.fallDownCheck = False
                return
            elif self.fallDownCheck == False:
                self.doFallDownSpecaical()
            elif sStandUp.FallDownCheck(self) == False:
                self.rGoalier.doGoalier()
            else:
                self.output.printf("doGoalier Error!!!!!!!!!")
            

        elif self.playnumber == Constants.ROLE_ATTACKER:         
            self.output.printf("==================I am Attacker")
            if self.specActionID==0 or self.specActionID==-1:
                if sStandUp.FallDownCheck(self):
                    self.output.printf("Attacker is falling down")
                    self.specialcounter=28
                    return
                else :
                    self.rAttacker.doAttacker()
                    self.output.printf2("----forward = ",self.forward)
                    self.output.printf2("----left = ",self.left)
                    self.output.printf2("----turnCCW = ",self.turnCCW)
                    self.output.printf2("----ballX = ",self.ball.localX)
                    self.output.printf2("----ballY = ",self.ball.localY)
                    self.output.printf2("----ballDist = ",self.ball.dist)
                    self.output.printf2("----ballDeg = ",self.ball.deg*180/3.14159)
            else:
                self.doSpecaical()
        elif self.playnumber == Constants.ROLE_SUPPORTER:
            self.rSupporter.doSupporter()
        elif self.playnumber == Constants.ROLE_DEBUG:
            self.rDebuger.doDebuger()
        else:
            self.output.printf("No such role!!!")
            
    def resetPerform(self):
        self.setStand()
        self.setHead(0,0,0)
        self.seegoal=0
        self.seeballcounter=0
        self.scanFindBallCounter = 0
        self.beginRotFindCounter=0
        self.findgoal=0
        self.fallDownCheck = True
        self.ballSwitchGoal = False
        self.AlignGoalToFindBallMode = False
        self.AlianGoalAndBallCounter = 0
            
    def updateGoalObjects(self):
        if self.gcData.Tcolor() == Constants.TEAM_RED :#Constants.TEAM_BLUE:
            self.voGoal = self.bGoal
            self.vtGoal = self.yGoal
        else:
            self.voGoal = self.yGoal
            self.vtGoal = self.bGoal
            
    ##########get teamNumber,playNumber form team.cfg in "/opt/Config/team.cfg" of naoqi's directory      
    def getTeamcfg(self):
        '''get teamNumbet and playNumbet'''
        teamcfgFile = open("/opt/Config/team.cfg")
        lines = teamcfgFile.readlines()
        tNString, pNString = lines[0].split() # the variables is only the string type
        teamN = string.atoi(tNString) # the string convert to int
        palyN = string.atoi(pNString) # the string convert to int
        return teamN,palyN

    #########send teamNumber,playNumber to C++
    def setTeamcfg(self):
        teamcfg = PyTeamCfg.TeamData(self.teamnumber, self.playnumber)
        self.teamCfg.setTeamCfg(teamcfg)

    def setBehavProcess(self,processID):
        behavProcess = PyBehavID.BehavData(processID)
        self.behavProcess.setProcessID(behavProcess)
    
    ######### send motion command to Actuator #########
    def setStandCmd(self):
        self.motionID = 0
        self.forward = 0
        self.left = 0
        self.turnCCW = 0
        self.walkType = 1

    def setStand(self):
        self.setStandCmd()
        command = PyMotion.WalkCmd(self.motionID,self.walkType,(self.forward, self.left, self.turnCCW))
        self.motion.setWalkCmd(command)
        
    def setWalkCmd(self, walkType, forward, left, turnCCW):
        self.motionID = 1
        self.walkType = walkType
        self.forward = forward
        self.left = left
        self.turnCCW = turnCCW

    def setWalk(self, walkType, forward,left ,turnCCW):
        self.setWalkCmd(walkType,forward,left ,turnCCW)
        command = PyMotion.WalkCmd(self.motionID,self.walkType,(self.forward, self.left, self.turnCCW))
        self.motion.setWalkCmd(command)
        

    def setSpecialActionCmd(self, specActionID):
        self.motionID = 2
        self.forward = 0
        self.left = 0
        self.turnCCW = 0
        self.specActionID = specActionID

    def setSpecialAction(self, specActionID):
        self.setSpecialActionCmd(specActionID)
        command = PyMotion.SpecialAction(self.motionID,self.specActionID)
        self.motion.setSpecialAction(command)
        
    def setHead(self, yaw, pitch, speed):
        command = PyMotion.HeadCmd(DEG_TO_RAD(yaw), DEG_TO_RAD(pitch), speed)
        self.motion.setHeadCmd(command)
             
    ##################### End ########################

    def doSpecaical(self):
        self.output.printf("do Specaial")
        if self.specialcounter>0:
           self.specialcounter-=1
           self.output.printf2("do Speing",self.specialcounter)
        else:
            self.output.printf('special finish')
            self.specialcounter=0
            self.specActionID=-1
            self.setWalk(1,self.forward,self.left,self.turnCCW)
    
    def doFallDownSpecaical(self):
        self.output.printf("do Specaial")
        if self.specialcounter > 0:
           self.specialcounter -= 1
           self.output.printf2("do Speing",self.specialcounter)
        else:
            self.output.printf('special finish')
            self.fallDownCheck = True

            
    def standupSpecaial(self):
         if self.specialcounter>0:
           self.specialcounter-=1
           self.output.printf2("do Speing",self.specialcounter)
        
        
        
