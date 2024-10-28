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
        self.direction = np.radians(float(kwargs.get('direction', 0)) % 360)
        self.sensor_angle = float(kwargs.get('sensor_angle', 45))
        self.path = [self.coords]

    def addMap(self, mapShape):
        if len(mapShape) != 2:
            raise ValueError
        self.digitalMap = np.zeros(mapShape)

    def seekObstacles(self, obstacleMap):
        found = False
        leftDir  = self.direction + np.radians(self.sensor_angle)
        rightDir = self.direction - np.radians(self.sensor_angle)

        coords = []
        (foundT, coordsL) = self.mark_obstacles(obstacleMap, leftDir)
        if foundT:
            found = True
        (foundT, coordsC) = self.mark_obstacles(obstacleMap, self.direction)
        if foundT:
            found = True
        (foundT, coordsR) = self.mark_obstacles(obstacleMap, rightDir)
        if foundT:
            found = True
        coords.append(coordsL)
        coords.append(coordsC)
        coords.append(coordsR)
        return found, coords
    
    def mark_obstacles(self, obstacleMap, angle):
        found = False
        x0, y0 = int(self.coords[0]), int(self.coords[1])
        x1, y1 = (x0 + round(self.vision * np.cos(angle)), y0 - round(self.vision * np.sin(angle)))
        coords = []
        for (x, y) in bresenham((x0, y0), (x1, y1)):
            coords.append((x, y))
            if 0 <= x < obstacleMap.shape[1] and 0 <= y < obstacleMap.shape[0] \
                and obstacleMap[y, x] and not self.digitalMap[y, x]:
                self.digitalMap[y - self.tol : y + self.tol, x - self.tol : x + self.tol] = True
                found = True                 
                break
        return found, coords
    
class Third_Paper_Robot (Robot):
    def __init__(self, coords, **kwargs):
        super().__init__(coords, **kwargs)

    def isStuck(self, Utot) -> bool:
        res = False
        i = min(max(int(self.coords[0]), 0), Utot.shape[1] - 1)
        j = min(max(int(self.coords[1]), 0), Utot.shape[0] - 1)
        neighbours = Utot[i-1: i+2, j-1:j+2]
        local_minimum = np.unravel_index(np.argmin(neighbours), neighbours.shape)
        new_i = i - 1 + local_minimum[0]  # New global i (y)
        new_j = j - 1 + local_minimum[1]  # New global j (x)
        if (i, j) == (new_j, new_i):
            res = True
        return res
    
class ComplexRobot(Third_Paper_Robot):
    def __init__(self, coords, **kwargs):
        super().__init__(coords, **kwargs)
        self.sensor_tolerance = kwargs.get('sensor_tolerance', self.sensor_angle)