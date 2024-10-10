import numpy as np

def getDistanceFromCoordinate(x, y, x2, y2) -> float:
    return np.sqrt((x - x2)**2 + (y - y2)**2)
