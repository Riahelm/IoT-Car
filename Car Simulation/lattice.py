import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation
from   IPython.display import HTML 
from   random import randint as rd

from robot import Robot 
from goal import Goal 
from obstacle import Obstacle 

class Lattice:
    def __init__(self, sizeX, sizeY, stepSize, goalAttraction, obstacleRepulsion):
        self.xs = np.arange(0, sizeX, 1)
        self.ys = np.arange(0, sizeY, 1)
        self.X, self.Y = np.meshgrid(self.xs, self.ys)
        self.Fx = np.zeros_like(self.X, dtype= float)
        self.Fy = np.zeros_like(self.Y, dtype= float)
        self.goals = []
        self.obstacles = []
        self.stepSize = stepSize
        self.goalAttraction = goalAttraction
        self.obstacleRepulsion = obstacleRepulsion
        #for i in range(self.X.shape[0]):
        #    self.addObstacle(Obstacle(1, (i,0), 200, 1))
        #    self.addObstacle(Obstacle(1, (i,self.X.shape[1]), 20, 1))
        #for j in range(self.X.shape[1]):
        #    self.addObstacle(Obstacle(1, (0,j), 200, 1))
        #    self.addObstacle(Obstacle(1, (self.X.shape[0], j), 20, 1))

    def addRobot(self, r):
        if isinstance(r, Robot):
            self.robot = r

    def addGoal(self, g):
        if isinstance(g, Goal):
            self.goals.append(g)

    def addObstacle(self, o):
        if isinstance(o, Obstacle):
            self.obstacles.append(o)

    def calcForces(self):
        # Reset forces
        self.Fx = np.zeros_like(self.X)
        self.Fy = np.zeros_like(self.Y)

        # Compute goal forces
        if len(self.goals) > 0:
          Fx, Fy = self.goals[0].getForces(self.X, self.Y)
          self.Fx += Fx
          self.Fy += Fy

        # Compute obstacle forces
        for obstacle in self.robot.knownObstacles:
            Fx, Fy = obstacle.getForces(self.X, self.Y)
            self.Fx += Fx
            self.Fy += Fy

        # Set forces to zero inside any goal or obstacle radius
        for i in range(self.X.shape[0]):
            for j in range(self.X.shape[1]):
                for obstacle in self.robot.knownObstacles:
                    if obstacle.getDistance(self.X[i, j], self.Y[i, j]) <= obstacle.radius:
                        self.Fx[i, j] = 0
                        self.Fy[i, j] = 0


    def getPotential(self):
      Ures = np.zeros_like(self.X, dtype= float)
      if len(self.goals) > 0:
          Uatt = self.goals[0].getPotential(self.X, self.Y)
          Ures += Uatt
      for obstacle in self.obstacles:
          Urep = obstacle.getPotential(self.X, self.Y)
          Ures += Urep
      return Ures

    def showPotential(self):
        ax = plt.axes(projection='3d')
        ax.plot_surface(self.X, self.Y, self.getPotential(), cmap='viridis')
        plt.show()

    def randomize(self):
        self.addRobot(Robot(1, (3, 3), 10))

        for i in range(rd(1, 5)):
          self.addGoal(Goal(rd(3,4), (rd(2, len(self.xs)-2), rd(2, len(self.ys)-2)), self.goalAttraction))

        for i in range(rd(4, 10)):
            self.addObstacle(Obstacle(1, (rd(0, len(self.xs)), rd(0, len(self.ys))), self.obstacleRepulsion, 3 * self.robot.radius))

    def draw(self):
      fig, ax = plt.subplots(figsize=(10, 10))
      ax.quiver(self.X, self.Y, self.Fx, self.Fy)
      self.movePatches(ax)
      plt.xlim(0, len(self.xs))
      plt.ylim(0, len(self.ys))
      plt.gca().set_aspect('equal', adjustable='box')
      return fig, ax

    def movePatches(self, ax):
        ax.clear()
        ax.quiver(self.X, self.Y, self.Fx, self.Fy)
        if hasattr(self, "robot") and isinstance(self.robot, Robot):
            ax.add_patch(plt.Circle(self.robot.coords, self.robot.radius, color='b', alpha=0.5))
            i = min(max(int(self.robot.coords[0]), 0), self.Fx.shape[0] - 1)
            j = min(max(int(self.robot.coords[1]), 0), self.Fy.shape[1] - 1)
            plt.streamplot(self.X, self.Y, self.Fx, self.Fy, start_points=np.array([[i, j]]), integration_direction='forward')
        for goal in self.goals:
            ax.add_patch(plt.Circle(goal.coords, goal.radius, color='g', alpha=0.5))
        for obstacle in self.obstacles:
            ax.add_patch(plt.Circle(obstacle.coords, obstacle.radius, color='r', alpha=0.5))

    def animate(self):

        fig, ax = self.draw()

        def generate():
          global stop
          stop = False
          i = 0
          while not stop and i < 200:
            yield i
            i += 1

        def update(i):
          global stop
          self.robot.findAndAddNewObstacles(self.obstacles)
          self.calcForces()
          self.robot.move(self.stepSize, self.getPotential(), self.Fx, self.Fy)

          self.movePatches(ax)
          quiver = ax.quiver(self.X, self.Y, self.Fx, self.Fy)

          if len(self.goals) > 0 and self.robot.isInside(self.goals[0]):
            print("Reached goal!")
            self.goals.pop(0)
            if len(self.goals) == 0:
              print("Reached all goals!")
              stop = True
          if self.robot.isStuck():
            print("Got stuck!")
            self.addObstacle(Obstacle(1, (self.robot.coords), self.obstacleRepulsion, 3 * self.robot.radius))
            #stop = True
          return quiver

        matplotlib.rcParams['animation.embed_limit'] = 2**128
        ani = matplotlib.animation.FuncAnimation(fig, update, frames=generate,  cache_frame_data=False)
        writervideo = matplotlib.animation.FFMpegWriter(fps = 60)
        ani.save('Car.mp4', writer = writervideo)
        
        return HTML(ani.to_jshtml())