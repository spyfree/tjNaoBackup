import yapgvb
import re
import os

class Graph:
    def __init__(self,fileName):
        self.state = []
        self.indenty_state = []
        self.fileName = fileName
        self.imstate = []
        self.direct = []
        self.number = []
        self.numberq = 0
        self.numberOfState = []
        self.NumberOfRun = 0
        self.Run_imstate = []
        self.Run_state = []
        self.nextState = None
        self.deepth = 0
        
    def SearchState(self):
        self.numberOfState = 0
        f = open(self.fileName,'r')
        allLines = f.readlines()
        f.close()
        i=-1
        charge = "empty"
        for eachLine in allLines:
            j=0
            eachLine=eachLine[:-1]
            while 1:
                n1 = re.match('    ',eachLine)
                if n1 is not None:
                    j = j + 1
                    eachLine=eachLine[4:]
                else:
                    break
                
            m_def = re.match('def',eachLine)
            if m_def is not None:
                i = i + 1
                words = re.split('\(',eachLine[4:-1])
                self.state.append([words[0]])
                self.imstate.append([j])
                self.number.append(1)
                self.numberOfState = self.numberOfState + 1
            else:
                m_main = re.match('if __name',eachLine)
                if m_main is not None:
                    i = i + 1
                    word = eachLine[:-1]
                    self.state.append([word])
                    self.imstate.append([j])
                    self.number.append(1)
                    self.NumberOfState = NumberOfState + 1


            m_state = re.search('##state',eachLine)
            if charge=="found":
                m_brackets = re.search('\(',eachLine)
                if m_brackets is not None:
                    m_tail = re.split('\(',eachLine)
                    eachLine = m_tail[0]
                m_dot = re.search('\.',eachLine)
                if m_dot is not None:
                    m_tail = re.split('\.',eachLine)
                    eachLine = m_tail[1]
                m_blank = re.search(' ',eachLine)
                if m_blank is not None:
                    m_tail = re.split(' ',eachLine)
                    eachLine = m_tail[0]
                m_equl = re.search('=',eachLine)
                if m_equl is not None:
                    m_tail = re.split('=',eachLine)
                    eachLine = m_tail[0]
                charge = eachLine
                if j > self.imstate[i][0]:
                    self.state[i].append(charge)
                    self.imstate[i].append(j)
                    self.number[i] = self.number[i] + 1

            if m_state is not None:
                charge = "found"
                                 
        self.numberq = self.numberOfState


    def CompareState(self):
        for i in range(self.numberOfState):
            for k in range(self.number[i]):
                if k!=0:
                    for a in range(self.numberOfState):
                        if self.state[a][0]==self.state[i][k]:
                            a = -1
                            break
                    if a != -1:
                        self.state.append([self.state[i][k]])
                        self.imstate.append([0])
                        self.number.append(1)
                        self.numberOfState = self.numberOfState + 1
                        self.CompareState()
                        break

                    
    def FindIntialState(self):
        f = open(self.fileName,'r')
        allLines = f.readlines()
        f.close()
        charge = "empty"
        for eachLine in allLines:
            while 1:
                n1 = re.match('    ',eachLine)
                if n1 is not None:
                    eachLine=eachLine[4:]
                else:
                    break
            m_begin = re.search('##initial state',eachLine)
            if charge=="found":
                words = re.split('\(',eachLine[4:-2])
                charge = words[0]
                break
            if m_begin is not None:
                charge = "found"
        self.Run_state.append(charge)
        self.Run_imstate.append(0)

    def RunState(self,next):
        for i in range(self.numberOfState):
            if self.state[i][0]==next:
                if self.number[i]>1:
                    for k in range(self.number[i]-1):
                        charge = "run"
                        for a in range(self.NumOfRun):
                            if  self.Run_state[a]==self.state[i][k+1]:
                                charge = "break"
                                break
                        self.Run_state.append(self.state[i][k+1])
                        self.Run_imstate.append(self.deepth+self.imstate[i][k+1])
                        self.NumOfRun = self.NumOfRun + 1
                        if charge=="break":
                            continue
                        else:
                            self.deepth = self.deepth + 1
                            self.RunState(self.state[i][k+1])
                            self.deepth = self.deepth - 1

    
    def DoGraph(self):
        g = yapgvb.Digraph()
        self.SearchState()
        self.FindIntialState()
        self.CompareState()
        self.deepth = 0
        self.nextState = self.Run_state[0]
        self.NumOfRun = 0
        self.RunState(self.nextState)
##        print self.state
##        print self.imstate
##        print "run",self.Run_state
##        print "run_j",self.Run_imstate
##        print self.NumOfRun
        lastword1 = None
        lastword2 = None
        i = 0
        numberOfDirect = 0
        for i in range(self.numberq):
            for a in range(self.NumOfRun):
                if self.state[i][0]==self.Run_state[a]:
                    self.direct.append(g.add_node(self.state[i][0],label = self.state[i][0],shape = "circle"))
                    self.indenty_state.append(self.state[i][0])
                    numberOfDirect=numberOfDirect+1

        i = self.numberq
        while i < self.numberOfState:
            for a in range(self.NumOfRun):
                if self.state[i][0]==self.Run_state[a]:
                    self.direct.append(g.add_node(self.state[i][0],label = self.state[i][0],shape = "box"))
                    self.indenty_state.append(self.state[i][0])
                    numberOfDirect=numberOfDirect+1

            i = i+1
        i = 0
        for i in range(self.NumOfRun):
            a = 0
            for a in range(numberOfDirect):
                if self.indenty_state[a]==self.Run_state[i]:
                    head = self.direct[a]
            k=i+1
            if self.Run_imstate[i] < self.Run_imstate[k]:
                n=self.Run_imstate[k] - self.Run_imstate[i]
            while self.Run_imstate[i] < self.Run_imstate[k]:
                if self.Run_imstate[i] == self.Run_imstate[k] - n:
                    for a in range(numberOfDirect):
                        if self.indenty_state[a]==self.Run_state[k]:
                            tail = self.direct[a]
                            edge = head >> tail

                k=k+1
                if k > self.NumOfRun-1:
                    break
        print g.layout(yapgvb.engines.dot)
        g.layout('dot')

        base_name = re.split('\.',self.fileName)      
        m_basename = re.search('/',base_name[0])
        if m_basename is not None:
            base_name = re.split('/',base_name[0])

        svgPath = os.getcwd()+"/Src/Module/Behavior/python/SvgGraph/" + base_name[len(base_name)-1] + ".svg"
        g.render(svgPath)
       
##if __name__ == "__main__":
##    g = Graph("rAttacker.py")
##    g.DoGraph()

