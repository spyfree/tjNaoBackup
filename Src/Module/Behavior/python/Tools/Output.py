import Constants
class BehaviorLog(object):
    def __init__(self,brain):
        self.brain = brain
        self.buffer = ''
        self.logDir = "/opt/naoqi/modules/python/BehavLog.log"
        self.logFile = open(self.logDir,'w')
        
    def writeLine(self,string):
        self.buffer = self.buffer + str(string) + "\n"
    def writeLine2(self,string1,string2):
        self.buffer = self.buffer + str(string1) + str(string2) + "\n"
    def saveToFile(self):
        try:
            self.logFile.write(str(self.buffer))
        except Exception,e:
            print "Error when writing to logFile:",e
    def closeLog(self):
        try:
            self.logFile.close()
        except Exception,e:
            print "Erroe when closing logFile:",e
class Output:
    def printf(self,string):
        if Constants.BEHAVPUT:
            print str(string)

    def printf2(self,string,data):
        if Constants.BEHAVPUT:
            print str(string),data



