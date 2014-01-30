# -*- coding: utf-8 -*-
import math

#Field Constants Variable
FIELD_WIDTH = 4050
FIELD_LENGTH = 6050
GOAL_WIDTH = 1800
GOAL_LENGTH = 650
GOALBOX_WIDTH = 1400
CIRCLE_DIAMETER = 1250
LINE_WIDTH = 50

# Setup team colors
NUM_GAME_TEAM_COLORS = 2
teamColorDict = dict(zip(range(NUM_GAME_TEAM_COLORS),\
                      ("TEAM_BLUE","TEAM_RED")))
(TEAM_BLUE,TEAM_RED) = range(NUM_GAME_TEAM_COLORS)

# Setup goal colors
NUM_GAME_GOAL_COLORS = 2
goalColorDict = dict(zip(range(NUM_GAME_GOAL_COLORS),\
                      ("GOAL_BLUE","GOAL_YELLOW")))
(GOAL_BLUE, GOAL_YELLOW) = range(NUM_GAME_GOAL_COLORS)


#gameControl state
STATE_INITIAL = 0
STATE_READY = 1
STATE_SET = 2
STATE_PLAYING = 3
STATE_FINISHED = 4

#penalty
PENALTY_NONE = 0
PENALTY_BALL_HOLDING = 1
PENALTY_GOALIE_PUSHING = 2
PENALTY_PLAYER_PUSHING = 3
PENALTY_ILLEGAL_DEFENDER = 4
PENALTY_ILLEGAL_DEFENSE = 5
PENALTY_OBSTRUCTION = 6
PENALTY_REQ_FOR_PICKUP = 7
PENALTY_LEAVING = 8
PENALTY_DAMAGE = 9
PENALTY_MANUAL = 10

#role
ROLE_GOALIER = 1
ROLE_ATTACKER = 2
ROLE_SUPPORTER = 3
ROLE_DEFENDER = 4
ROLE_DEBUG = 12

#used in sTrackBall.py
IMAGE_WIDTH = 320
IMAGE_HEIGHT = 240
FIELD_VIEW_H = 46.4*math.pi/180
FIELD_VIEW_V = 34.8*math.pi/180

#used in HeadMode.py
SEARCH_BALL = 0
LOOK_UP_AND_DOWN = 1
LOOK_STRAIGHTA_HEAD = 2
LOOK_DOWN = 3
LOOK_AT_BALL_AND_GOAL = 4
LOOK_AROUND = 5

#used for MotionID,WalkType,SpecialActionID
MOTIONID = {'Stand':0,'Walk':1,'SpecialAction':2}
WALKTYPE = {'LIPMWalk':0,'CartTableWalk':1}
SPECACTIONID = {'playDead':0,'Stand':1,'StandUpBack':2,'StandUpBackNao':3,\
                'StandUpFront':4,'StandUpFrontNao':5,'grandPaLeft':6,\
                'kickLeftNao':7,'lookAtFloorInFront':8,'lookStraightInFront':9,\
                'demoJesus':10,'kickRightNao':11,'demoUpright':12}

#used for fallDownState
FALLDOWNSTATE = {'undefined':0,'upright':1,\
                 'lyingOnFront':2,'lyingOnBack':3,\
                 'lyingOnLeftSide':4,'lyingOnRightSide':5}

BEHAVPUT = True



