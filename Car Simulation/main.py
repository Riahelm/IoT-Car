from lattice import Lattice
from third_paper_lattice import Third_Paper_Lattice as TPL
from third_paper_robot import Third_Paper_Robot as TPR
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

robot = TPR((50, 50), radius = 1, vision = 150, sensor_angle = 45, direction = 0, tolerance = 1)
lat = TPL(400, 600, robot, Goal(50, (300, 300)), safe_distance = 0.5 + robot.radius)

lat.randomize()
#lat.plotPath(skip = 10, max_its = 700)
lat.animate(skip = 10, max_its = 700, drop = 10)

