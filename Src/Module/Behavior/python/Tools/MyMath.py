# -*- coding: utf-8 -*- 
import math

def getRelativeHeading(destX, destY, x, y):
    destH = -math.atan((x - destX)/(y - destY))
    return destH
    
def DEG_TO_RAD(deg):
    return deg * math.pi/180.0

def RAD_TO_DEG(rad):
    return rad * 180/math.pi

def ABS_MAX(value1, value2):
    if math.abs(value1) > math.abs(value2):
        return value1
    else:
        return value2
def ValueToKey(dict,value):
    items = dict.items()
    length = len(items)
    values = []
    for i in range(length):
        values.append(items[i][1])
        if values[i] == value:
            return items[i][0]
def CLIP(data,min,max):
    if data < min:
        data = min
    elif data > max:
        data = max
    else:
        data = data
    return data
