from lattice import Polygon_Lattice as PL
from robot import Polygon_Robot as PR 
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

robot = PR((10, 10), radius = 1, vision = 10, sensor_angle = 45, direction = 0, sensor_tolerance = 30, tolerance = 1)
lat = PL(50, 50, robot, Goal(2, (40, 40)), safe_distance = robot.radius + 0.05)
lat.addObstacle(Obstacle(1, (16,17)))
lat.addObstacle(Obstacle(1, (30,20)))
lat.addObstacle(Obstacle(1, (8,18)))
#lat.addObstacle(Obstacle(1, (25,18)))
#lat.genObs()
#lat.randomize()
#lat.plotPath(skip = 10, max_its = 700)
lat.animate(skip = 1, max_its = 120, drop = 1)

