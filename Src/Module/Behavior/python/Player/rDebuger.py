import PyCamera
class Debuger(object):
    def __init__(self,brain):
        self.brain = brain
        self.findBallCouter = 0

    def doDebuger(self):
        self.perform()

    def perform(self):
        if self.findBallCouter < 16:
            self.brain.setHead(90,-45,0.5)
        elif self.findBallCouter < 32:
            self.brain.setHead(-90,-45,0.5)
    
        self.findBallCouter += 1
        if self.findBallCouter == 31:
            self.findBallCouter = 0
##        if self.brain.common.bumpLeft() == 1:
##            self.brain.leftReset = True
##        if self.brain.common.bumpRight() == 1:
##            self.brain.leftReset = False
##
##        if self.brain.leftReset == True:
##            common = PyCamera.CameraCmd(1,1)
##            self.brain.camera.setCameraConfig(common)
##        if self.brain.leftReset == False:
##            common = PyCamera.CameraCmd(0,1)
##            self.brain.camera.setCameraConfig(common)
            
        
