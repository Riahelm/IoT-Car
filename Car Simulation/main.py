from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

lat = Lattice(600, 400, Robot((50, 350), vision= 2, direction= 0), Goal(1, (400,50)))

lat.genObs()
#lat.addObstacle(Obstacle(1, (15, 12)))

lat.plotPath(skip=10, max_its = 1500)
#lat.animate(skip = 1)

