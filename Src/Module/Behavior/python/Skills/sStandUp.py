import Tools.Constants as Constants

def FallDownCheck(brain):
    fallDownState = brain.common.fallState()
    if fallDownState == Constants.FALLDOWNSTATE['upright']:
        return False
    else:
        if fallDownState == Constants.FALLDOWNSTATE['lyingOnFront']:
            brain.setSpecialAction(Constants.SPECACTIONID['StandUpFrontNao'])
        elif fallDownState == Constants.FALLDOWNSTATE['lyingOnBack']:
            print 'do stand back'
            brain.setSpecialAction(Constants.SPECACTIONID['StandUpBackNao'])
        else:
            brain.setSpecialAction(Constants.SPECACTIONID['StandUpFrontNao'])
        return True

    
        
