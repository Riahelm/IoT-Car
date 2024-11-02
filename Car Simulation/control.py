from typing import override
import numpy as np
from matplotlib import pyplot as plt
from shapely.geometry import Point, LineString
from util import normalize_radians, get_bounded_indexes
from robot import Polygon_Robot
from lattice import Polygon_Lattice
from goal import Goal

class Control_Robot(Polygon_Robot):
    def __init__(self, coords, **kwargs):
        super().__init__(coords, **kwargs)

    @override
    def seek_obstacles(self, obstacleMap):
        found = False
        leftDir  = normalize_radians(self.direction - self.sensor_angle)
        rightDir = normalize_radians(self.direction + self.sensor_angle)
        
        obsList = [Point(col, row) for row, col in np.argwhere(obstacleMap)]
        
        foundT = self.mark_obstacles(obsList, leftDir)
        if foundT:
            found = True
        foundT = self.mark_obstacles(obsList, self.direction)
        if foundT:
            found = True
        foundT = self.mark_obstacles(obsList, rightDir)
        if foundT:
            found = True

        return found
    
    @override
    def mark_obstacles(self, obs_list, angle):
        
        polygon = self.get_cone(self.vision, angle)
        found = False
        containedPoints = [point for point in obs_list if polygon.contains(point)]

        if containedPoints:
            best = min(containedPoints, key=lambda p : Point(self.coords).distance(p))
            
            if not self.digitalMap[int(best.y), int(best.x)]:
                y, x = int(best.y), int(best.x)
                self.digitalMap[y - self.tol : y + self.tol, x - self.tol : x + self.tol] = True
                found = True
        return found
    
    def move(self, vy, vx, current_point, obstacleMap):
        # Calculate the distance to move
        dt = ((self.vision - self.radius) * self.scale) / np.linalg.norm([vx, vy])
        next_point = current_point + dt * np.array([vx, vy])
    
        # Bound the coordinates to stay within map limits

        boundedX = np.clip(round(next_point[1]), self.radius, self.digitalMap.shape[0] - self.radius - 1)
        boundedY = np.clip(round(next_point[0]), self.radius, self.digitalMap.shape[1] - self.radius - 1)
        boundedNext = (boundedY, boundedX)
    
        # Create a line path from current position to target with a buffer
        line = LineString([self.coords, boundedNext]).buffer(self.radius)
        obs_in_path = [Point(col, row) for row, col in np.argwhere(obstacleMap) if line.contains(Point(col, row))]
        
        if obs_in_path:
            closest = min(obs_in_path, key=lambda x: Point(self.coords).distance(x))
            
            # Calculate the direction vector towards the obstacle
            direction_vector = np.array([closest.x - self.coords[1], closest.y - self.coords[0]])
            direction_vector = direction_vector / np.linalg.norm(direction_vector)
            
            # Set a distance to stop before the obstacle
            stop_distance = self.radius + self.tol  # Distance to stop before the obstacle
            boundedNext = (self.coords[0] + direction_vector[1] * (Point(self.coords).distance(closest) - stop_distance),
                           self.coords[1] + direction_vector[0] * (Point(self.coords).distance(closest) - stop_distance))
            
        self.coords = boundedNext
        
        self.direction = np.arctan2(-vy, vx)
        return boundedNext    

class Control_Lattice(Polygon_Lattice):
    def __init__(self, rows: int, columns: int, robot: Control_Robot, goal: Goal, **kwargs):
        super().__init__(rows, columns, robot, goal, **kwargs)
    
    @override
    def plotPath(self, alpha, **kwargs):
        """
        Plots to screen path taken by robot.
        
        \n Optional parameters
        -------------------
        skip: int
              up/down scaling of the quiver plot for animating

        max_its: int
                 max number of Euler steps to generate (default 400)
        
        full: bool
              changes function to use whole potential field
        """
        skip = kwargs.get('skip', 10)
        max_its = kwargs.get('max_its', 400)
        full = bool(kwargs.get('full', False))
        start = self.robot.coords
        goal = self.goals[0].coords
        
        if full:
            reached, route = self.calcFullPath(max_its)
            fig, ax = self.drawFullForce(skip)
            savefile = f"Car Simulation/Tests/Results/Full/"
        else:
            reached, route = self.calcPath(max_its)
            fig, ax = self.draw(skip)
            savefile = f"Car Simulation/Tests/Results/Partial/"
        plt.plot(route[:,0], route[:,1], linestyle = 'dashed', linewidth=3)
        plt.title(f"alpha = {alpha}")
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.savefig(savefile + f"R_{reached}_α_{alpha}.png", bbox_inches = 'tight')
        plt.close(fig)
        return reached

    @override
    def calcPath(self, max_its) -> tuple[bool, list]:
        [gy, gx] = self.getForces()
        route = np.vstack([np.array(self.robot.coords), np.array(self.robot.coords)])

        for _ in range(max_its):
            current_point = route[-1,:]

            found = self.robot.seek_obstacles(self.obstacleMap)
            if found:
                [gy, gx] = self.getForces()

            if self.goals[0].isTouching(self.robot):
                return True, route

            # NumPy arrays access grids by (row, column) so it is inverted
            row, col = get_bounded_indexes(current_point, self.obstacleMap.shape[::-1])

            # Gradient returns y-axis and then x-axis
            # y values go from top to bottom in NumPy arrays, need to negate to offset it
        
            if (gx[row, col], gy[row, col]) == (0, 0) or self.robot.isStuck(self.getPotential()):
                x, y = get_bounded_indexes((col + np.random.randint(0,3) - 1, row + np.random.randint(0,3) - 1), self.robot.digitalMap.shape)
                self.robot.digitalMap[x, y] = True
                [gy, gx] = self.getForces()

            
            next_point = self.robot.move(gy[row, col], gx[row, col], current_point, self.obstacleMap)

            route = np.vstack( [route, next_point] )

        return False, route

    def calcFullPath(self, max_its) -> bool:
        [gy, gx] = np.gradient(-self.getWholePotential())
        route = np.vstack([np.array(self.robot.coords), np.array(self.robot.coords)])

        for _ in range(max_its):
            current_point = route[-1,:]

            if self.goals[0].isTouching(self.robot):
                return True, route

            # NumPy arrays access grids by (row, column) so it is inverted
            row, col = get_bounded_indexes(current_point, self.obstacleMap.shape[::-1])

            # Gradient returns y-axis and then x-axis
            # y values go from top to bottom in NumPy arrays, need to negate to offset it
        
            if (gx[row, col], gy[row, col]) == (0, 0) or self.robot.isStuck(self.getPotential()):
                x, y = get_bounded_indexes((col + np.random.randint(0,3) - 1, row + np.random.randint(0,3) - 1), self.robot.digitalMap.shape)
                self.robot.digitalMap[x, y] = True
                [gy, gx] = self.getForces()

            
            next_point = self.robot.move(gy[row, col], gx[row, col], current_point, self.obstacleMap)

            route = np.vstack( [route, next_point] )
        return False, route
