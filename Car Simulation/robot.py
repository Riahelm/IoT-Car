import numpy as np
import math
from sphere import Sphere
from matplotlib import pyplot as plt
from util import bresenham
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
        
        direction: degrees
                   initial direction of the robot (default 0)

        sensor_angle: degrees
                      fixed angular distance of sensors (default 45)
        """
        super().__init__(float(kwargs.get('radius', 1)), coords)
        self.vision = float(kwargs.get('vision', self.radius * 6))
        self.tol = int(kwargs.get('tolerance', 2))
        self.direction = float(kwargs.get('direction', 0)) % 360
        self.sensor_angle = float(kwargs.get('sensor_angle', 45))
        self.path = []

    def addMap(self, mapShape):
        if len(mapShape) != 2:
            raise ValueError
        self.digitalMap = np.zeros(mapShape)

    def isStuck(self) -> bool:
        return False

    def seekObstacles(self, obstacleMap):
        found = False
        leftDir  = math.radians(self.direction + self.sensor_angle)
        rightDir = math.radians(self.direction - self.sensor_angle)
        if self.mark_obstacles(obstacleMap, leftDir):
            #print("Found left")
            found = True
        if self.mark_obstacles(obstacleMap, self.direction):
            #print("Found center")
            found = True
        if self.mark_obstacles(obstacleMap, rightDir):
            #print("Found right")
            found = True
        return found
    
    def mark_obstacles(self, obstacleMap, angle):
        found = False
        x0, y0 = int(self.coords[1]), int(self.coords[0])
        x1, y1 = (x0 + round(self.vision * np.cos(angle)), y0 + round(self.vision * np.sin(angle)))
        #print(f"Marking from: {x0, y0}, to {x1, y1}" )
        for (x, y) in bresenham((x0, y0), (x1, y1)):
            if 0 <= x < obstacleMap.shape[0] and 0 <= y < obstacleMap.shape[1] and obstacleMap[x, y]:
            #if obstacleMap[x, y]: #and not self.digitalMap[x, y]:
                #print(f"Found here: {x},{y}")
                self.digitalMap[x - self.tol : x + self.tol, y - self.tol : y + self.tol] = True
                found = True                 
                break
        return found
    


    #def mark_obstacles(self, obstacleMap, angle):
    #    found = False
    #    (x, y) = int(self.coords[0]), int(self.coords[1])
    #    (cx, cy) = (x + round(self.vision * np.cos(angle)), y + round(self.vision * np.sin(angle)))
#
    #    # Bresenham's Line Algorithm
    #    dx = abs(cx - x)
    #    dy = abs(cy - y)
    #    sx = 1 if x < cx else -1
    #    sy = 1 if y < cy else -1
    #    if dx > dy:
    #        err = dx / 2.0
    #        while x != cx and found == False:
    #            if 0 <= x < obstacleMap.shape[0] and 0 <= y < obstacleMap.shape[1]:
    #                if obstacleMap[x, y] and not self.digitalMap[x, y]:  # Check if there's an obstacle
    #                    self.digitalMap[x - self.tol : x + self.tol, y - self.tol : y + self.tol] = True
    #                    found = True
    #            err -= dy
    #            if err < 0:
    #                y += sy
    #                err += dx
    #            x += sx
    #    else:
    #        err = dy / 2.0
    #        while y != cy and found == False:
    #            if 0 <= x < obstacleMap.shape[0] and 0 <= y < obstacleMap.shape[1]:
    #                if obstacleMap[x, y] and not self.digitalMap[x, y]:  # Check if there's an obstacle
    #                    self.digitalMap[x - self.tol : x + self.tol, y - self.tol : y + self.tol] = True
    #                    found = True 
    #            err -= dx
    #            if err < 0:
    #                x += sx
    #                err += dy
    #            y += sy
    #    return found
    