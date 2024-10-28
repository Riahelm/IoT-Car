from lattice import Third_Paper_Lattice as TPL
from robot import ComplexRobot as CR 
from robot import Third_Paper_Robot as TPR
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

robot = CR((10, 10), radius = 1, vision = 0.5, sensor_angle = 90, direction = 0, sensor_tolerance = 15, tolerance = 1)
lat = TPL(50, 50, robot, Goal(2, (40, 40)), safe_distance = robot.radius + 0.05)
lat.addObstacle(Obstacle(1, (16,19)))
#lat.genObs()
#lat.randomize()
lat.plotPath(skip = 10, max_its = 700)
#lat.animate(skip = 1, max_its = 120, drop = 10)

