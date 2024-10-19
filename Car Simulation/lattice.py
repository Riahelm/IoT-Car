import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation
from   IPython.display import HTML 
from scipy.ndimage import distance_transform_edt as bwdist
from   random import randint as rd
from tqdm.auto import tqdm

from robot import Robot 
from goal import Goal 
from obstacle import Obstacle 

class Lattice:
    def __init__(self, columns: int, rows: int, robot: Robot, goal: Goal, **kwargs):
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
        goalAttraction: float
                        attraction multiplier of the goals (default 1/700)
                        
        obstacleRepulsion: float
                           repulsion multiplier of the obstacles (default 800)
        
        safeDistance: float
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
        self.robot = robot
        [self.X, self.Y] = np.meshgrid(np.arange(columns), np.arange(rows))
        robot.addMap(self.X.shape)
        self.Fx = np.zeros_like(self.X, dtype=float)
        self.Fy = np.zeros_like(self.Y, dtype=float)
        self.goals = [goal]
        self.obstacleMap = np.zeros_like(self.X, dtype=bool)
        self.goalAttraction = float(kwargs.get('goalAttraction', 1/700.))
        self.obstacleRepulsion = float(kwargs.get('obstacleRepulsion', 800))
        self.safeDistance = float(kwargs.get('safeDistance', robot.radius * 2))

    def addGoal(self, g):
        if isinstance(g, Goal):
            self.goals.append(g)

    def addObstacle(self, o):
        (x, y) = o.coords
        (xs, ys) = self.obstacleMap.shape
        if isinstance(o, Obstacle) and 0 <= x < xs and 0 <= y < ys:
            t = ((self.X - o.coords[0])**2 + (self.Y - o.coords[1])**2) < o.radius**2
            self.obstacleMap [t] = True

    def randomize(self):
        #self.addRobot(Robot(1, (3, 3), 10))

        for _ in range(25):
            self.addObstacle(Obstacle(rd(5, 20), (rd(0, self.num_cols), rd(0, self.num_rows))))

    def genObs(self):
        self.obstacleMap [300:, 100:250] = True
        self.obstacleMap [150:200, 400:500] = True

        t = ((self.X - 200)**2 + (self.Y - 50)**2) < 50**2
        self.obstacleMap[t] = True

        t = ((self.X - 400)**2 + (self.Y - 300)**2) < 100**2
        self.obstacleMap[t] = True

    def getPotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goalAttraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)

      d = bwdist(self.robot.digitalMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacleRepulsion * ((1./rescaleD - 1/self.safeDistance)**2)
      Urep [rescaleD > self.safeDistance] = 0
      return Uatt + Urep
    
    def calcPath(self, start_coords, end_coords, max_its):
          [gy, gx] = np.gradient(-self.getPotential())
          route = np.vstack( [np.array(start_coords), np.array(start_coords)] )
          forces = [[gy, gx]]
          coords = []
          for _ in range(max_its):
            current_point = route[-1,:]
            (found, coordsT) = self.robot.seekObstacles(self.obstacleMap)
            if found:
                #print("Found obs")
                #if np.allclose((gy, gx), np.gradient(-self.getPotential())):
                #    print("Error")
                [gy, gx] = np.gradient(-self.getPotential())
                # Gradient returns y-axis and then x-axis
            if sum( abs(current_point-end_coords) ) < self.goals[0].radius:
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
        #(obsY, obsX) = np.where(self.robot.digitalMap == True)
        for x, y in zip(obsX, obsY):
            obstacle_patch = plt.Circle((x, y), self.robot.tol, color='r', alpha=1)  # Use self.robot.tol for the radius
            ax.add_patch(obstacle_patch)
        
        # Update goals
        for goal in self.goals:
            if hasattr(goal, 'patch'):
                goal.patch.remove()  # Remove old patch
            goal.patch = plt.Circle(goal.coords, goal.radius, color='g', alpha=0.5)
            ax.add_patch(goal.patch)

    def draw(self, skip):
      fig, ax = plt.subplots(figsize=(12, 8))
      [fy, fx] = np.gradient(-self.getPotential())
      plt.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], fx[::skip,::skip], fy[::skip,::skip], pivot = 'mid')
      
      self.movePatches(ax)

      return fig, ax

    def plotPath(self, **kwargs):
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
        route, _, _ = self.calcPath(start, goal, max_its)
        self.draw(skip)
        plt.plot(start[0], start[1], 'bo', markersize=10)
        plt.plot(goal[0], goal[1], 'go', markersize=10)
        plt.plot(route[:,0], route[:,1], linestyle = 'dashed', linewidth=3)
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.show()
        plt.close()

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

        # Define the robot's start and end coordinates
        start_coords = self.robot.coords
    
        # Calculate the path from the robot's start to the goal using calcPath
        for i in range(len(self.goals)):
            path, forces, coords = self.calcPath(start_coords, self.goals[0].coords, max_its)
        
        drop = kwargs.get('drop', 1)
        
        def generate():
          global stop
          stop = False
          for i in tqdm(range(len(path) - 1)):
             if i % drop == 0:
                yield i

        def update(i):
            global stop
            
            ax.clear()

            if i < len(path):
                self.robot.coords = path[i]
                for obs_coords in coords[i]:
                    if obs_coords:
                        (x_vals, y_vals) = zip(*obs_coords)
                        plt.plot(x_vals, y_vals, 'g--')

            
            self.movePatches(ax)

            quiver = ax.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], forces[i][1][::skip,::skip], forces[i][0][::skip,::skip], pivot = 'mid')
            vx = forces[i][1][int(round(self.robot.coords[1])), int(round(self.robot.coords[0]))]
            vy = forces[i][0][int(round(self.robot.coords[1])), int(round(self.robot.coords[0]))]
            
            direction_vector = np.array([vx, vy])
            
            ax.arrow(self.robot.coords[0], self.robot.coords[1], direction_vector[0], direction_vector[1],
                     head_width=5, head_length=5, fc='blue', ec='blue')
            
            return quiver

        matplotlib.rcParams['animation.embed_limit'] = 2**128
        ani = matplotlib.animation.FuncAnimation(fig, update, frames=generate,  cache_frame_data=False)
        writervideo = matplotlib.animation.FFMpegWriter(fps = 1)
        ani.save('Car.mp4', writer = writervideo)
        
        return HTML(ani.to_jshtml())
    



    def getWholePotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goalAttraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)

      d = bwdist(self.obstacleMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacleRepulsion * ((1./rescaleD - 1/self.safeDistance)**2)
      Urep [rescaleD > self.safeDistance] = 0
      return Uatt + Urep
    
    def drawFullForce(self, skip):
      fig, ax = plt.subplots(figsize=(12, 8))
      [fy, fx] = np.gradient(-self.getWholePotential())
      plt.quiver(self.X[::skip,::skip], self.Y[::skip,::skip], fx[::skip,::skip], fy[::skip,::skip], pivot = 'mid')
      self.movePatches(ax)

    