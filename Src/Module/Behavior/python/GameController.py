import GameStates
import Tools.Constants as Constants

class GameController(object):
    def __init__(self,brain):
        self.brain = brain
##        self.gamedata=PyGameControl.gamecontrol()
##        self.gamestate = self.gamedata.GameState()
##        self.penalty = self.gamedata.Penalty()
##        self.secstillUnpenal = self.gamedata.SecstillUnpenal()
##        self.Tcolor=self.gamedata.Tcolor()
    def run(self):
        ##print "------------------------------------------------gamestate = ",self.brain.gamestate
        if self.brain.gamestate == Constants.STATE_INITIAL:
            GameStates.gameInitial(self.brain)
        elif self.brain.gamestate == Constants.STATE_READY:
            GameStates.gameReady(self.brain)
        elif self.brain.gamestate == Constants.STATE_SET:
            GameStates.gameSet(self.brain)
        elif self.brain.gamestate == Constants.STATE_PLAYING:
            ##print "#########self.penalty = ",self.brain.penalty
            if self.brain.penalty==Constants.PENALTY_NONE or self.brain.SecstillUnpenal==0:
                GameStates.gamePlaying(self.brain)
            else:
                GameStates.gamePenalised(self.brain)
                
        elif self.brain.gamestate == Constants.STATE_FINISHED:
            GameStates.gameFinished(self.brain)
            
            
        
