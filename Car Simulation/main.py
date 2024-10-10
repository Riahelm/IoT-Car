from lattice import Lattice
from robot import Robot
from goal import Goal
from obstacle import Obstacle

lat = Lattice(30, 30, 0.25, 0.35, 200)

lat.randomize()

#lat.addRobot(Robot(1,(2,2), 10))
#
#lat.addGoal(Goal(2,(25,25), 0.35))
#lat.addObstacle(Obstacle(1, (11, 9), 100, lat.robot.radius * 2))
#lat.addObstacle(Obstacle(1, (19, 11), 100, lat.robot.radius * 2))
lat.animate()
