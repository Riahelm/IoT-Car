from typing import override
import numpy as np
from shapely import Point, Polygon
from sphere import Sphere
from matplotlib import pyplot as plt
from util import bresenham, normalize_radians, getDistanceFromCoordinate
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

    def addMap(self, mapShape):
        if len(mapShape) != 2:
            raise ValueError
        self.digitalMap = np.zeros(mapShape)

    def seek_obstacles(self, obstacleMap):
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
        self.recent_positions = []
    def isStuck(self, Utot) -> bool:
        """
        Determines if the robot is stuck due to zero force, a local minimum,
        or by moving in circles.

        Parameters:
        -----------
        Utot : ndarray
            The total potential field where the robot is navigating.

        Returns:
        --------
        bool
            True if the robot is considered stuck, otherwise False.
        """
        # Get current robot position on the grid
        i = min(max(int(self.coords[1]), 0), Utot.shape[1] - 1)
        j = min(max(int(self.coords[0]), 0), Utot.shape[0] - 1)

        # Check if the robot is in a local minimum by comparing to neighbors
        neighbors = Utot[j-1: j+2, i-1:i+2]
        local_minimum = np.unravel_index(np.argmin(neighbors), neighbors.shape)
        new_i = i - 1 + local_minimum[1]
        new_j = j - 1 + local_minimum[0]

        # If the robot is in a local minimum
        if (i, j) == (new_i, new_j):
            return True

        # Detect circular movement by tracking recent positions
        pos = tuple(self.coords)
        if pos in self.recent_positions:
            return True  # Stuck due to looping

        # Update recent positions with a fixed-length queue
        self.recent_positions.append(pos)
        if len(self.recent_positions) > 10:  # Limit to the last 10 positions
            self.recent_positions.pop(0)

        # Not stuck
        return False


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
        
        scale: float
                    up/down scaling value of step w.r.t vision (default 1, step = vision)
        """
        super().__init__(coords, **kwargs)
        self.sensor_tolerance = np.radians(float(kwargs.get('sensor_tolerance', self.sensor_angle)) % 360)
        self.scale = float(kwargs.get('scale', self.vision))

    @override
    def seek_obstacles(self, obstacleMap):
        found = False
        leftDir  = normalize_radians(self.direction - self.sensor_angle)
        rightDir = normalize_radians(self.direction + self.sensor_angle)
        
        polys = []
        obsList = [Point(col, row) for row, col in np.argwhere(obstacleMap)]
        
        (foundT, polyL) = self.mark_obstacles(obsList, leftDir)
        if foundT:
            found = True
        (foundT, polyC) = self.mark_obstacles(obsList, self.direction)
        if foundT:
            found = True
        (foundT, polyR) = self.mark_obstacles(obsList, rightDir)
        if foundT:
            found = True

        polys.append((polyL, polyC, polyR))
        return found, polys
    
    @override
    def mark_obstacles(self, obsList, angle):
        
        polygon = self.get_cone(self.vision, angle)
        found = False
        containedPoints = [point for point in obsList if polygon.contains(point)]

        if containedPoints:
            best = min(containedPoints, key=lambda p : polygon.distance(p))
            vision = getDistanceFromCoordinate(self.coords[1], self.coords[0], best.y, best.x)
            polygon = self.get_cone(vision, angle)
            
            if not self.digitalMap[int(best.y), int(best.x)]:
                self.digitalMap[int(best.y), int(best.x)] = True
                found = True
        return (found, polygon)
    
    def move(self, vy, vx, current_point):
        dt = (self.vision * self.scale) / np.linalg.norm([vx, vy])
        next_point = current_point + dt*np.array([vx, vy])
        self.coords = next_point
        self.direction = np.arctan2(-vy, vx)
        return next_point
    
    def get_cone(self, vision, angle):

        left_extreme =  normalize_radians(angle + self.sensor_tolerance)
        right_extreme = normalize_radians(angle - self.sensor_tolerance)

        left_point =  self.coords + vision * np.array([ np.cos(left_extreme), -np.sin(left_extreme)])
        right_point = self.coords + vision * np.array([ np.cos(right_extreme), -np.sin(right_extreme)])

        vertices = [tuple(self.coords), tuple(left_point), tuple(right_point)]
        return Polygon(vertices)
    


