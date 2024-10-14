from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle
import numpy as np
from matplotlib import pyplot as plt

lat = Lattice(600, 400, Robot((50, 350), vision= 2000), Goal(1, (400,50)))

#lat.addObstacle(Obstacle(1, (12,6)))

lat.genObs()
#for i in range (150, 350):
#    lat.addObstacle(Obstacle(1, (5, i), lat.obstacleRepulsion, lat.safeDistance))

lat.plotPath()

#lat.animate(skip = 1)

