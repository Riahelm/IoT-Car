import numpy as np
import math
from sphere import Sphere
from matplotlib import pyplot as plt
class Robot(Sphere):
    def __init__(self, coords, **kwargs):
        """
        Robot of spherical shape
        
        Optional parameters
        -------------------
        
        radius : float
                 radius of the robot (default 1)
                 
        vision : float
                 sensor max distance of the robot (default 6 * radius)

        tolerance: int
                   radius of newly added obstacles for increased performance (default 2)
        
        direction: radians
                   initial direction of the robot (default 0)

        sensor_angle: degrees
                      fixed angular distance of sensors (default 45)
        """
        super().__init__(float(kwargs.get('radius', 1)), coords)
        self.vision = float(kwargs.get('vision', self.radius * 6))
        self.tol = int(kwargs.get('tolerance', 2))
        self.direction = float(kwargs.get('radians', 0))
        self.sensor_angle = float(kwargs.get('degrees', 45))
        self.path = []

    def addMap(self, mapShape):
        if len(mapShape) != 2:
            raise ValueError
        self.digitalMap = np.zeros(mapShape)

    def isStuck(self) -> bool:
        return False

    def seekObstacles(self, obstacleMap):
        found = False
        leftDir  = self.direction - math.radians(self.sensor_angle)
        rightDir = self.direction + math.radians(self.sensor_angle)

        if  self.mark_obstacles(obstacleMap, leftDir)        or \
            self.mark_obstacles(obstacleMap, self.direction) or \
            self.mark_obstacles(obstacleMap, rightDir):
                found = True
        return found
    
    def mark_obstacles(self, obstacleMap, angle):
        found = False
        (x, y) = self.coords
        (cx, cy) = (x + round(self.vision * np.cos(angle)), y + round(self.vision * np.sin(angle)))

        # Bresenham's Line Algorithm
        dx = abs(cx - x)
        dy = abs(cy - y)
        sx = 1 if x < cx else -1
        sy = 1 if y < cy else -1
        if dx > dy:
            err = dx / 2.0
            while x != cx and found == False:
                if 0 <= x < obstacleMap.shape[0] and 0 <= y < obstacleMap.shape[1]:
                    if self.digitalMap[x, y] != obstacleMap[x, y] == True :  # Check if there's an obstacle
                        self.digitalMap[x - self.tol : x + self.tol, y - self.tol : y + self.tol] = True
                        found = True
                err -= dy
                if err < 0:
                    y += sy
                    err += dx
                x += sx
        else:
            err = dy / 2.0
            while y != cy and found == False:
                if 0 <= x < obstacleMap.shape[0] and 0 <= y < obstacleMap.shape[1]:
                    if self.digitalMap[x, y] != obstacleMap[x, y] == True:  # Check if there's an obstacle
                        self.digitalMap[x - self.tol : x + self.tol, y - self.tol : y + self.tol] = True
                        found = True 
                err -= dx
                if err < 0:
                    x += sx
                    err += dy
                y += sy
        return found
    