from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt
robot = Robot((50, 50), radius = 1, vision = 150, sensor_angle = 45, direction = 0, tolerance = 1)
lat = Lattice(400, 600, robot, Goal(4, (300, 300)), safeDistance = 0.5 + robot.radius)

lat.randomize()
lat.plotPath(skip = 10, max_its = 700)
#lat.animate(skip = 10, max_its = 700, drop = 10)

