
def GoToBall(brain):
    if brain.ball.dist > 30:
        forward = brain.ball.dist * cos(brain.ball.deg)
        left = brain.ball.dist * sin(brain.ball.deg)
        turnCCW = 0#brain.ball.deg
        print '-----turnCCW:::::::::::',turnCCW
        brain.setWalk(1,forward,left,turnCCW)
    elif brain.ball.dist > 10:
        forward = 0.5*brain.ball.dist * cos(brain.ball.deg)
        left = brain.ball.dist * sin(brain.ball.deg)
        turnCCW = 0#brain.ball.deg
        print '-----turnCCW:::::::::::>10',turnCCW
        brain.setWalk(1,forward,left,turnCCW)
    else:
        print '-----turnCCW:::::::::::else',turnCCW
        forward = 0
        left = brain.ball.dist * sin(brain.ball.deg)
        turnCCW = 0#brain.ball.deg
        brain.setWalk(1,forward,left,turnCCW)
        
def getBehindBall(brain):
    ballX = brain.ball.dist * cos(brain.ball.deg)
    if ballX > 0: ## the robor is located in the left of the ball, so we choose ClockWise to walk torwards the ball
        forward = brain.ball.dist * cos(brain.ball.deg - 50)
        left = brain.ball.dist * sin(brain.ball.deg - 50)
        brain.setWalk(1, forward, left,0)
    else:
        forward = brain.ball.dist * cos(brain.ball.deg + 50)
        left = brain.ball.dist * sin(brain.ball.deg + 50)
        brain.setWalk(1, forward, left,0)
