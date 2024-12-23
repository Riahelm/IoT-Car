import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation
from typing import override
from   IPython.display import HTML 
from   scipy.ndimage import distance_transform_edt as bwdist
from   random import randint as rd
from   tqdm.auto import tqdm
from robot import * 
from goal import Goal 
from obstacle import Obstacle 
from util import get_bounded_indexes
class Lattice:
    def __init__(self, rows: int, columns: int, robot: Robot, goal: Goal, **kwargs):
        """
        APF-Enriched Lattice
        Mandatory parameters
        --------------------
        rows: int
              number of rows of the lattice

        columns: int
              number of columns of the lattice

        robot: Robot
               sphere that moves throughout the lattice, to the goal

        goal: Goal
              sphere that is the basis of the APF

        Optional parameters
        -------------------
        goal_attraction: float
                        attraction multiplier of the goals (default 1/700)
                        
        obstacle_repulsion: float
                           repulsion multiplier of the obstacles (default 800)
        
        safe_distance: float
                      mandatory distance of the robot to the obstacles (default is the robot's diameter)
        """
        if not isinstance(rows, int):
            print("Row value incorrect. Please rectify input")
            raise ValueError
        if not isinstance(columns, int):
            print("Columns value incorrect. Please rectify input")
            raise ValueError
        if not isinstance(robot, Robot):
            print("Robot value incorrect. Please rectify input")
            raise ValueError
        if not isinstance(goal, Goal):
            print("Goal value incorrect. Please rectify input")
            raise ValueError
        
        self.num_rows = rows
        self.num_cols = columns
        self.robotStartCoords = robot.coords
        self.robotStartDirection = robot.direction
        self.robot = robot
        [self.X, self.Y] = np.meshgrid(np.arange(columns), np.arange(rows))
        robot.addMap(self.X.shape)
        self.goals = [goal]
        self.obstacleMap = np.zeros_like(self.X, dtype=bool)
        self.goal_attraction = float(kwargs.get('goal_attraction', 1/700.))
        self.obstacle_repulsion = float(kwargs.get('obstacle_repulsion', 800))
        self.safe_distance = float(kwargs.get('safe_distance', robot.radius * 2))

    def addGoal(self, g):
        if isinstance(g, Goal):
            self.goals.append(g)

    def addObstacle(self, o):
        (x, y) = o.coords
        (xs, ys) = self.obstacleMap.shape
        if isinstance(o, Obstacle) and 0 <= x < xs and 0 <= y < ys:
            t = ((self.X - o.coords[0])**2 + (self.Y - o.coords[1])**2) < o.radius**2
            self.obstacleMap [t] = True

    def resetRobot(self):
        self.robot.coords = self.robotStartCoords
        self.robot.direction = self.robotStartDirection
        self.robot.addMap(self.X.shape)

    def randomize(self, num):

        for _ in range(num):
            self.addObstacle(Obstacle(rd(1, 2), (rd(0, self.num_cols), rd(0, self.num_rows))))

    def getPotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goal_attraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)
    
      d = bwdist(self.robot.digitalMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacle_repulsion * ((1./rescaleD - 1/self.safe_distance)**2)
      Urep [rescaleD > self.safe_distance] = 0
      return Uatt + Urep
    
    def getForces(self):
        return np.gradient(-self.getPotential())
    

    def calcPath(self, max_its):
          [gy, gx] = self.getForces()
          route = np.vstack( [np.array(self.robot.coords), np.array(self.robot.coords)] )
          forces = [[gy, gx]]
          coords = []
          for _ in range(max_its):
            current_point = route[-1,:]
            (found, coordsT) = self.robot.seek_obstacles(self.obstacleMap)
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
            self.robot.coords = next_point
            self.robot.direction = np.arctan2(-vy, vx)
            # y values go from top to bottom in NumPy arrays, need to negate to offset it
            route = np.vstack( [route, next_point] )
            forces.append([gy, gx])
            coords.append(coordsT)
          route = route[1:,:]
          return route, forces, coords

    def movePatches(self, ax):
        
        # Update the robot's position
        if hasattr(self, "robot") and isinstance(self.robot, Robot):
            # Remove the previous robot circle if it exists
            if hasattr(self, 'robot_patch'):
                self.robot_patch.remove()
            # Add the updated robot circle at the new position
            self.robot_patch = plt.Circle(self.robot.coords, self.robot.radius, color='b', alpha=0.5)
            ax.add_patch(self.robot_patch)

        #Update obstacles
        (obsY, obsX) = np.where(self.obstacleMap == True)
        for x, y in zip(obsX, obsY):
            obstacle_patch = plt.Circle((x, y), 0.65, color='r', alpha=1)
            ax.add_patch(obstacle_patch)
        (obsY, obsX) = np.where(self.robot.digitalMap == True)
        for x, y in zip(obsX, obsY):
            obstacle_patch = plt.Circle((x, y), 1, color='y', alpha=0.2)
            ax.add_patch(obstacle_patch)        
        # Update goals
        for goal in self.goals:
            if hasattr(goal, 'patch'):
                goal.patch.remove()  # Remove old patch
            goal.patch = plt.Circle(goal.coords, goal.radius, color='g', alpha=0.5)
            ax.add_patch(goal.patch)

    def draw(self, skip):

        fig, ax = plt.subplots(figsize = (10,10))
        # Calculate the gradients of the potential field
        [fy, fx] = self.getForces()

        # Plot the quiver plot, skipping elements as specified
        plt.quiver(self.X[::skip, ::skip], self.Y[::skip, ::skip], fx[::skip, ::skip], fy[::skip, ::skip], pivot='mid')

        # Move patches to the axes (for robot, obstacles, etc.)
        self.movePatches(ax)

        return fig, ax

    def plotPath(self, i, alpha, **kwargs):
        """
        Plots to screen path taken by robot.
        
        \n Optional parameters
        -------------------
        skip: int
              up/down scaling of the quiver plot for animating

        max_its: int
                 max number of Euler steps to generate (default 400)
        

        """
        skip = kwargs.get('skip', 10)
        max_its = kwargs.get('max_its', 400)
        start = self.robot.coords
        goal = self.goals[0].coords
        route, _, _ = self.calcPath(max_its)
        self.draw(skip)
        plt.plot(start[0], start[1], 'bo', markersize=10)
        plt.plot(goal[0], goal[1], 'go', markersize=10)
        plt.plot(route[:,0], route[:,1], linestyle = 'dashed', linewidth=3)
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.savefig(f"Car Simulation/Tests/Results/Test_{i}, alpha: {alpha}", format='png', bbox_inches = 'tight')

    def animate(self, **kwargs):
        """
        Animates the robot moving through the Lattice with the APF algorithm
        Optional parameters
        -------------------
        
        skip: int
              up/down scaling of the quiver plot for animating

        max_its: int
                 max number of Euler steps to generate (default 400)
        
        drop: int
              number of discarded frames (default 1 [None])
        """
        skip = int(kwargs.get('skip', 10))
        max_its = kwargs.get('max_its', 400)
        fig, ax = self.draw(skip)
    
        # Calculate the path from the robot's start to the goal using calcPath
        path, forces, coords = self.calcPath(max_its)
        
        drop = kwargs.get('drop', 1)
        
        quiver = ax.quiver(self.X[::skip, ::skip], self.Y[::skip, ::skip], forces[0][1][::skip, ::skip], forces[0][0][::skip, ::skip], pivot='mid', color='gray', alpha=0.5)

        def init():
            self.movePatches(ax)
            return [quiver]
        
        def generate():
          for i in tqdm(range(len(path) - 1)):
             if i % drop == 0:
                yield i

        def update(i):
            
            ax.clear()

            if i < len(path):
                self.robot.coords = path[i]
                for obs_coords in coords[i]:
                    if obs_coords:
                        (x_vals, y_vals) = zip(*obs_coords)
                        plt.plot(x_vals, y_vals, 'g--')

            
            self.movePatches(ax)
            quiver = ax.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], forces[i][1][::skip,::skip], forces[i][0][::skip,::skip], pivot = 'mid')

            row, col = get_bounded_indexes(self.robot.coords, self.obstacleMap.shape[::-1])

            vx = forces[i][1][row, col]
            vy = forces[i][0][row, col]
            
            direction_vector = np.array([vx, vy])
            
            ax.arrow(self.robot.coords[0], self.robot.coords[1], direction_vector[0], direction_vector[1],
                     head_width=self.robot.radius, head_length=self.robot.radius, fc='blue', ec='blue')
            
            return [quiver]

        matplotlib.rcParams['animation.embed_limit'] = 2**128
        ani = matplotlib.animation.FuncAnimation(fig, update, frames=generate, init_func= init, cache_frame_data=False, blit = True, repeat = False)
        writervideo = matplotlib.animation.FFMpegWriter(fps = 60)
        ani.save('Car.mp4', writer = writervideo)

    
    ## DEBUGGING METHODS ##
    def getWholePotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goal_attraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)

      d = bwdist(self.obstacleMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacle_repulsion * ((1./rescaleD - 1/self.safe_distance)**2)
      Urep [rescaleD > self.safe_distance] = 0
      
      return self.getPotential() + Urep 
    
    def drawFullForce(self, skip):
      
      fig, ax = plt.subplots(figsize=(10, 10))
      [fy, fx] = np.gradient(-self.getWholePotential())
      plt.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], fx[::skip,::skip], fy[::skip,::skip], pivot = 'mid')
      self.movePatches(ax)
      return fig, ax


class Second_Paper_Lattice(Lattice):
    def __init__(self, rows: int, columns: int, robot: Robot, goal: Goal, **kwargs):
        self.kV = kwargs.get('virtual_repulsion', 800)
        super().__init__(rows, columns, robot, goal, **kwargs)

    def getPotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goal_attraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)
    
      d = bwdist(self.robot.digitalMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacle_repulsion * ((1./rescaleD - 1/self.safe_distance)**2)
      Uvir = self.kV / rescaleD
      Urep [rescaleD > self.safe_distance] = 0
      Uvir [rescaleD > self.safe_distance] = 0
      return Uatt + Urep + Uvir
    


class Third_Paper_Lattice (Lattice):
    def __init__(self, rows: int, columns: int, robot: Third_Paper_Robot, goal: Goal, **kwargs):
        super().__init__(rows, columns, robot, goal, **kwargs)

    @override
    def calcPath(self, max_its):
        [gy, gx] = self.getForces()
        route = np.vstack( [np.array(self.robot.coords), np.array(self.robot.coords)] )
        forces = [[gy, gx]]
        coords = []
        for _ in range(max_its):
            current_point = route[-1,:]
            (found, coordsT) = self.robot.seek_obstacles(self.obstacleMap)
            if found:
                [gy, gx] = self.getForces()
                # Gradient returns y-axis and then x-axis

            if self.goals[0].isTouching(self.robot):
                print('Reached the goal !')
                break
            
            # NumPy arrays access grids by (row, column) so it is inverted
            row, col = get_bounded_indexes(current_point, self.obstacleMap.shape[::-1])

            vx = gx[row, col]
            vy = gy[row, col]
            
            dt = 1 / np.linalg.norm([vx, vy])
            next_point = current_point + dt*np.array([vx, vy])
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
    
class Polygon_Lattice(Third_Paper_Lattice):
    def __init__(self, rows: int, columns: int, robot: Polygon_Robot, goal: Goal, **kwargs):
        super().__init__(rows, columns, robot, goal, **kwargs)

    @override
    def calcPath(self, max_its):
        [gy, gx] = self.getForces()
        route = np.vstack([np.array(self.robot.coords), np.array(self.robot.coords)])
        forces = []
        polys = []


        for _ in range(max_its):
            current_point = route[-1,:]

            (found, polysT) = self.robot.seek_obstacles(self.obstacleMap)
            if found:
                [gy, gx] = self.getForces()

            if self.goals[0].isTouching(self.robot):
                print('Reached the goal !')
                forces.append([gy, gx])
                polys.append(polysT)
                break

            # NumPy arrays access grids by (row, column) so it is inverted
            row, col = get_bounded_indexes(current_point, self.obstacleMap.shape[::-1])

            # Gradient returns y-axis and then x-axis
            # y values go from top to bottom in NumPy arrays, need to negate to offset it
        
            if (gx[row, col], gy[row, col]) == (0, 0) or self.robot.isStuck(self.getPotential()):
                print("Got stuck")
                x, y = get_bounded_indexes((col + np.random.randint(0,3) - 1, row + np.random.randint(0,3) - 1), self.robot.digitalMap.shape)
                self.robot.digitalMap[x, y] = True
                [gy, gx] = self.getForces()

            
            next_point = self.robot.move(gy[row, col], gx[row, col], current_point, self.obstacleMap)

            route = np.vstack( [route, next_point] )
            forces.append([gy, gx])
            polys.append(polysT)
        route = route[1:,:]
        return route, forces, polys    

    @override
    def animate(self, i, **kwargs):
        """
        Animates the robot moving through the Lattice with the APF algorithm
        Optional parameters
        -------------------
        
        skip: int
              up/down scaling of the quiver plot for animating

        max_its: int
                 max number of Euler steps to generate (default 400)
        
        drop: int
              number of discarded frames (default 1 [None])
        """
        skip = int(kwargs.get('skip', 10))
        max_its = kwargs.get('max_its', 400)
        fig, ax = self.draw(skip)
    
        # Calculate the path from the robot's start to the goal using calcPath
        path, forces, polygons = self.calcPath(max_its)
        
        drop = kwargs.get('drop', 1)
        
        quiver = ax.quiver(self.X[::skip, ::skip], self.Y[::skip, ::skip], forces[0][1][::skip, ::skip], forces[0][0][::skip, ::skip], pivot='mid', color='gray', alpha=0.5)

        def init():
            self.movePatches(ax)
            return [quiver]
        
        def generate():
          for i in tqdm(range(len(path))):
             if i % drop == 0:
                yield i

        def update(i):
            
            ax.clear()
            self.robot.coords = path[i]
            if i < (len(path) - 1):
                x, y = LineString([self.robot.coords, path[i+1]]).buffer(self.robot.radius).exterior.xy
                plt.fill(x, y, alpha = 0.5, fc='green', label='Path taken')
            
            row, col = get_bounded_indexes(self.robot.coords, self.obstacleMap.shape[::-1])
            vx = forces[i][1][row, col]
            vy = forces[i][0][row, col]
            
            self.robot.direction = np.arctan2(-vy, vx) 
            for polys in polygons[i]:
                for poly in polys:
                    x, y = poly.exterior.xy
                    plt.fill(x, y, alpha = 0.3, fc='green', label='Vision Cone')
            
            self.movePatches(ax)

            quiver = ax.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], forces[i][1][::skip,::skip], forces[i][0][::skip,::skip], pivot = 'mid')
            
            return [quiver]

        matplotlib.rcParams['animation.embed_limit'] = 2**128
        ani = matplotlib.animation.FuncAnimation(fig, update, frames=generate, init_func= init, cache_frame_data=False, blit = True, repeat = False, interval = 1000 * i)
        writervideo = matplotlib.animation.FFMpegWriter(fps = 1 / i)
        ani.save(f"Car Simulation/Tests/Videos/Car_{i}.mp4", writer = writervideo)