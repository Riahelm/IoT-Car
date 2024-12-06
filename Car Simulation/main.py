import os
import subprocess
import numpy as np
from matplotlib import pyplot as plt
from tqdm.auto import tqdm
from lattice import Polygon_Lattice as PL
from robot import Polygon_Robot as PR
from control import Control_Lattice
from control import Control_Robot 
from goal import Goal
from util import createConfig, loadConfig

def save_plot(alphas, string, data):
    plt.plot(alphas, data, marker='o', linestyle='-', color='skyblue', markersize=8)
    plt.xlabel('Alpha')
    plt.ylabel(string)
    plt.xticks(rotation=45)
    plt.grid(True)  # Optional: add grid for better readability
    plt.tight_layout()
    plt.savefig(f"Car Simulation/Tests/Results/Images/{string}.png")
    plt.close()

def plot_data(alphas, **kwargs):
    full = bool(kwargs.get('full', False))
    filenames = [f"Car Simulation/Tests/Results/Data/Exp_{alpha}" for alpha in alphas]


    avg_results = []
    avg_steps = []
    avg_obstacles = []
    avg_times = []
    for file_path in filenames:
        if full:
            file_path = file_path + f"_Full"
        with open(file_path, 'r') as file:
            # Read and parse the data
            lines = file.readlines()

            # Prepare lists to store data
            results = []
            steps = []
            obstacles = []
            times = []

            for line in lines[1:]:
                parts = line.strip().split('|')
                if len(parts) >= 4:  # Ensure there are enough parts
                    results.append(int(parts[0].strip()))
                    steps.append(int(parts[1].strip()))
                    obstacles.append(int(parts[2].strip()))
                    times.append(float(parts[3].strip()))

            avg_results.append(sum(results) * 100./len(results))
            avg_steps.append(sum(steps)/len(steps))
            avg_obstacles.append(sum(obstacles)/len(obstacles))
            avg_times.append(sum(times)/len(times))
    
    if full:
        save_plot(alphas, "Full Average Success Percentage",   avg_results)
        save_plot(alphas, "Full Average Steps",                avg_steps)
        save_plot(alphas, "Full Average Time to compute, (s)", avg_times)
    else:
        save_plot(alphas, "Partial Average Success Percentage",   avg_results)
        save_plot(alphas, "Partial Average Steps",                avg_steps)
        save_plot(alphas, "Partial Average Obstacles Seen",       avg_obstacles)
        save_plot(alphas, "Partial Average Time to compute, (s)", avg_times)
        
def setup_files(alphas):

    files = [f"Car Simulation/Tests/Results/Data/Exp_{alpha}" for alpha in alphas]
    for file in files:
        try:
            os.remove(file) # Delete file
            os.remove(file + "_Full")
        except FileNotFoundError:
            pass    

        with open(file, 'a') as f:
            f.write(f"{'Result':<15}|{'Steps':<15}|{'Obstacles':<15}|Time\n")

        with open(file + "_Full", 'a') as f:
            f.write(f"{'Result':<15}|{'Steps':<15}|{'Obstacles':<15}|Time\n")

def save_data(filename, data):
    # Open the file in append mode
    with open(filename, 'a') as file:
        # Append a line of text
        file.write(f"{data[0]:<15}|{data[1]:<15}|{data[2]:<15}|{data[3]}\n")
        
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

def gen_stats(i, alphas, lat_size, robot_coords, robot_radius, goal_coords, obstacle_count, obstacle_max_radius):

    vision = lat_size/5
    createConfig((lat_size, lat_size), robot_coords, goal_coords, obstacle_count, obstacle_max_radius)

    for alpha in alphas:
        filename = f"Car Simulation/Tests/Results/Data/Exp_{alpha}"
        
        radius = robot_radius
        robot = Control_Robot(robot_coords, radius = radius, vision = vision, sensor_tolerance = 15, scale = alpha, tolerance=1)
        radius = lat_size/10
        lat = Control_Lattice(lat_size, lat_size, robot, Goal(radius, goal_coords), safe_distance = 1 + 0.075)
        lat.obstacleMap = loadConfig()
        data = lat.plotPath(i, alpha, skip = 1, max_its = 200)
        lat.resetRobot()
        save_data(filename, data)
        data = lat.plotPath(i, alpha, skip = 1, max_its = 200, full = True) 
        save_data(filename + f"_Full", data)


alphas = [1, 0.5, 0.25, 0.1]
lat_size = 100
robot_radius = 2
obstacle_max_radius = 2
obstacle_count = 10
exp_count = 100
robot_coords = (lat_size - 2 * robot_radius, 1 + 2 * robot_radius)
goal_coords = (0 + lat_size/10,lat_size - lat_size/10)

subprocess.run(["cleanup.bat"], shell= False)

setup_files(alphas)

#for i in tqdm(range(exp_count)):
#    gen_stats(i, alphas, lat_size, robot_coords, robot_radius, goal_coords, obstacle_count, obstacle_max_radius)

#plot_data(alphas)
#plot_data(alphas, full = True)