from typing import override
import numpy as np
import math
from shapely.affinity import rotate, translate
from shapely import Polygon
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
        self.sensor_angle = np.radians(float(kwargs.get('sensor_angle', 45)) % 360)
        self.path = [self.coords]

    def addMap(self, mapShape):
        if len(mapShape) != 2:
            raise ValueError
        self.digitalMap = np.zeros(mapShape)

    def seekObstacles(self, obstacleMap):
        found = False
        leftDir  = self.direction + self.sensor_angle
        rightDir = self.direction - self.sensor_angle

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
    
class Polygon_Robot(Third_Paper_Robot):
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
        
        sensor_tolerance: degrees
                          fixed angular cone of vision (default sensor_angle)
        """
        super().__init__(coords, **kwargs)
        self.sensor_tolerance = np.radians(float(kwargs.get('sensor_tolerance', self.sensor_angle)))

    @override
    def seekObstacles(self, obstacleMap):
        found = False
        leftDir  = self.direction + self.sensor_angle
        rightDir = self.direction - self.sensor_angle

        coords = []
        polys = []
        polysL = self.get_cone(leftDir)
        polysC = self.get_cone(self.direction)
        polysR = self.get_cone(rightDir)

        (foundT, coordsL) = super().mark_obstacles(obstacleMap, leftDir)
        if foundT:
            found = True
        (foundT, coordsC) = super().mark_obstacles(obstacleMap, self.direction)
        if foundT:
            found = True
        (foundT, coordsR) = super().mark_obstacles(obstacleMap, rightDir)
        if foundT:
            found = True

        polys.append(polysL)
        polys.append(polysC)
        polys.append(polysR)
        coords.append(coordsL)
        coords.append(coordsC)
        coords.append(coordsR)
        return found, coords, polys
    

    def get_cone(self, angle):
        left_extreme = angle - self.sensor_tolerance
        right_extreme = angle + self.sensor_tolerance

        left_point = self.coords + self.vision * np.array([np.cos(left_extreme), -np.sin(left_extreme)])
        right_point = self.coords + self.vision * np.array([np.cos(right_extreme), -np.sin(right_extreme)])

        vertices = [tuple(self.coords), tuple(left_point), tuple(right_point)]
        return Polygon(vertices)
    
    def plot_cone(self, cone):
        x, y = cone.exterior.xy
        x0, y0 = int(self.coords[0]), int(self.coords[1])

        plt.plot(x0, y0, 'bo', markersize = 10, label = "Robot")
        plt.fill(x, y, alpha = 0.3, fc='blue', label='Vision Cone')
        plt.show()


