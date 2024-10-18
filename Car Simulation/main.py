from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt
robot = Robot((50, 50), radius = 1, vision = 150, sensor_angle = 45, direction = 0)
lat = Lattice(600, 400, robot, Goal(2, (300, 200)), safeDistance = 2 * robot.radius)

lat.robot.tol = 1
#lat.genObs()
lat.randomize()
#offset = int(robot.vision/4)
#lat.obstacleMap[150, 250] = True
#lat.obstacleMap[100, 500] = True
#lat.obstacleMap[5,5] = True
#lat.obstacleMap[100:301:200, 50:] = True
#lat.obstacleMap[200, 200] = True
#lat.addObstacle(Obstacle(3, (200,197)))
lat.plotPath(skip=10, max_its = 700)
#lat.drawFullForce(skip = 10)
lat.animate(skip = 10, max_its = 700, drop = 10)

