from typing import override
from goal import Goal
from third_paper_robot import Third_Paper_Robot
from lattice import Lattice
import numpy as np

class Third_Paper_Lattice (Lattice):
    def __init__(self, rows: int, columns: int, robot: Third_Paper_Robot, goal: Goal, **kwargs):
        super().__init__(rows, columns, robot, goal, **kwargs)

    @override
    def calcPath(self, start_coords, end_coords, max_its):
        [gy, gx] = self.getForces()
        route = np.vstack( [np.array(start_coords), np.array(start_coords)] )
        forces = [[gy, gx]]
        coords = []
        for _ in range(max_its):
            current_point = route[-1,:]
            (found, coordsT) = self.robot.seekObstacles(self.obstacleMap)
            if found:
                [gy, gx] = self.getForces()
                # Gradient returns y-axis and then x-axis

            if self.goals[0].isTouching(self.robot):
                print('Reached the goal !')
                break
            
            # NumPy arrays access grids by (row, column) so it is inverted
            row = np.clip(int(current_point[1]), 0, self.obstacleMap.shape[0] - 1)
            col = np.clip(int(current_point[0]), 0, self.obstacleMap.shape[1] - 1)

            vx = gx[row, col]
            vy = gy[row, col]
            dt = 1 / np.linalg.norm([vx, vy])
            next_point = current_point + dt*np.array([vx, vy])
            self.robot.path.insert(0, next_point)
            self.robot.coords = next_point
            self.robot.direction = np.arctan2(-vy, vx)
            # y values go from top to bottom in NumPy arrays, need to negate to offset it
            if (vx, vy) == (0, 0) or self.robot.isStuck(self.getPotential()):
                print("Got stuck")
                self.robot.digitalMap[row, col] = True
                [gy, gx] = self.getForces()
            route = np.vstack( [route, next_point] )
            forces.append([gy, gx])
            coords.append(coordsT)
        route = route[1:,:]
        return route, forces, coords