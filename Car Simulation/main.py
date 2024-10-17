from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

lat = Lattice(600, 400, Robot((100, 200), radius = 1, vision = 500, direction = 0, sensor_angle = 0), Goal(5, (500, 200)))

lat.robot.tol = int(lat.safeDistance)
#lat.genObs()

lat.obstacleMap[100:301:200, 50:] = True
#lat.addObstacle(Obstacle(1, (300, 199)))
#lat.addObstacle(Obstacle(1, (300, 199)))
#lat.plotPath(skip=10, max_its = 500)
lat.robot.seekObstacles(lat.obstacleMap)
#lat.animate(skip = 10, max_its = 500, drop = 10)

