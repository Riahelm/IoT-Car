from lattice import Polygon_Lattice as PL
from robot import Polygon_Robot as PR 
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

alpha = 1
lat_size = 100
vision = lat_size/5
robot = PR((20, 20), radius = 1, vision = vision, sensor_angle = 30, direction = 0, sensor_tolerance = 15, scale = alpha)
radius = (robot.vision * robot.scale) / 2
goal = Goal(radius, (90, 90))
lat = PL(lat_size, lat_size, robot, goal, safe_distance = robot.radius + 0.05)

lat.animate(skip = 1, max_its = 100, drop = 1)



