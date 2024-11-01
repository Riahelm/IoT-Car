from lattice import Polygon_Lattice as PL
from robot import Polygon_Robot as PR
from control import Control_Lattice
from control import Control_Robot 
from goal import Goal
from util import createConfig, loadConfig

def gen_animations(alphas, lat_size, obstacle_count):

    vision = lat_size/5
    createConfig((lat_size, lat_size), obstacle_count, 2)
    
    for alpha in alphas:
        radius = 1
        robot = PR((lat_size - 2 * radius, 1 + 2 * radius), radius = radius, vision = vision, sensor_tolerance = 15, scale = alpha, tolerance=1.2)
        radius = lat_size/10
        offset = lat_size/10
        lat = PL(lat_size, lat_size, robot, Goal(radius, (0 + offset,lat_size - offset)), safe_distance = 1 + 0.075)
        lat.obstacleMap = loadConfig()
        lat.animate(i= alpha, skip = 1, max_its = 100, drop = 1)

def gen_stats(alphas, lat_size, robot_radius, obstacle_count, obstacle_max_radius):

    vision = lat_size/5
    createConfig((lat_size, lat_size), obstacle_count, obstacle_max_radius)
    
    for alpha in alphas:
        radius = robot_radius
        robot = Control_Robot((lat_size - 2 * radius, 1 + 2 * radius), radius = radius, vision = vision, sensor_tolerance = 15, scale = alpha, tolerance=1)
        radius = lat_size/10
        offset = lat_size/10
        lat = Control_Lattice(lat_size, lat_size, robot, Goal(radius, (0 + offset,lat_size - offset)), safe_distance = 1 + 0.075)
        lat.obstacleMap = loadConfig()
        lat.plotPath(alpha, skip = 1, max_its = 200)
        lat.resetRobot()
        lat.plotFullPath(alpha, skip = 1, max_its = 200) 

alphas = [1, 0.5, 0.25, 0.1]
lat_size = 100
robot_radius = 2
obstacle_max_radius = 2
obstacle_count = 20
gen_stats(alphas, lat_size, robot_radius, obstacle_count, obstacle_max_radius)