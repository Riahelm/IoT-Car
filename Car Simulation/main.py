from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

lat = Lattice(600, 400, Robot((50, 350), radius = 3, vision = 250, direction = np.pi/4), Goal(1, (400, 50)))

lat.robot.tol = 1 #int(lat.safeDistance)
lat.genObs()
offset = int(lat.robot.vision / 2)

lat.plotPath(skip=10, max_its = 1500)
#lat.animate(skip = 10, max_its = 100)

