from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt
robot = Robot((400, 500), radius = 0.5, vision = 25)
lat = Lattice(600, 400, robot, Goal(2, (400, 50)), safeDistance = 0.5 * robot.radius)

lat.robot.tol = int(lat.safeDistance)
lat.genObs()

#lat.obstacleMap[100:301:200, 50:] = True

#lat.plotPath(skip=10, max_its = 150)

lat.animate(skip = 10, max_its = 350, drop = 75)

