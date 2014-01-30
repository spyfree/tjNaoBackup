import Tools.Constants as Constants
import Tools.Output as Output
import Skills.HeadMode as HeadMode
    

def gameInitial(brain):
    brain.setStand()
    brain.output.printf("#################################GameStateis**STATE_INITIAL**********")
    
        
def gameReady(brain):
    brain.output.printf("#################################GameStateis**STATE_READY**********")
    pass

def gameSet(brain):
    brain.output.printf("#################################GameStateis**STATE_SET**********")
    HeadMode.headMode(brain,Constants.SEARCH_BALL)
    
    
def gamePlaying(brain):
    brain.output.printf("#################################GameStateis**GamePlaying**********")
    brain.perform()
    
def gameFinished(brain):
    brain.output.printf("#################################GameStateis**STATE_FINISHED**********")
    brain.resetPerform()

def gamePenalised(brain):
    brain.output.printf("#################################GameStateis**PENALISED**********")
    brain.resetPerform()
    
        
    
    
