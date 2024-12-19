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
    avg_calculations = []
    avg_scans = []
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
            calculations = []
            scans = []
            times = []

            for line in lines[1:]:
                parts = line.strip().split('|')
                if len(parts) >= 5:  # Ensure there are enough parts
                    has_reached = int(parts[0].strip())
                    results.append(has_reached)
                    if has_reached:
                        steps.append(float(parts[1].strip()))
                        calculations.append(int(parts[2].strip()))
                        scans.append(int(parts[3].strip()))
                        times.append(float(parts[4].strip()))

            avg_results.append(sum(results) * 100./len(results))
            avg_steps.append(sum(steps)/len(steps))
            avg_calculations.append(sum(calculations)/len(calculations))
            avg_scans.append(sum(scans)/len(scans))
            avg_times.append(sum(times)/len(times))
    
    if full:
        save_plot(alphas, "Control Average Distance",             avg_steps)
        save_plot(alphas, "Control Average Calculations",         avg_calculations)
        save_plot(alphas, "Control Average Time to compute, (s)", avg_times)
    else:
        save_plot(alphas, "Average Success Percentage",   avg_results)
        save_plot(alphas, "Average Distance",             avg_steps)
        save_plot(alphas, "Average Calculations",         avg_calculations)
        save_plot(alphas, "Average Scans",                avg_scans)
        save_plot(alphas, "Average Time to compute, (s)", avg_times)

def plot_distance(alphas, optimal_alpha):
    filenames = [f"Car Simulation/Tests/Results/Data/Exp_{alpha}" for alpha in alphas]
    control_file = f"Car Simulation/Tests/Results/Data/Exp_{optimal_alpha}_Full"
    
    # Extract the optimal path distance from the control file
    optimal_distance = 0
    with open(control_file, 'r') as file:
        lines = file.readlines()
        steps = []
        for line in lines[1:]:
            parts = line.strip().split('|')
            if len(parts) >= 5:  # Ensure there are enough parts
                has_reached = int(parts[0].strip())
                if has_reached:
                    steps.append(float(parts[1].strip()))
        if steps:
            optimal_distance = sum(steps) / len(steps)  # Calculate the average optimal distance
        else:
            print("No optimal distance found.")
            return
    
    # Calculate the actual/optimal ratio for each alpha
    distance_ratios = []
    for file_path in filenames:
        with open(file_path, 'r') as file:
            lines = file.readlines()
            actual_distances = []
            for line in lines[1:]:
                parts = line.strip().split('|')
                if len(parts) >= 5:  # Ensure there are enough parts
                    has_reached = int(parts[0].strip())
                    if has_reached:
                        actual_distances.append(float(parts[1].strip()))
            # Compute the average actual distance for this alpha
            avg_actual_distance = sum(actual_distances) / len(actual_distances) if actual_distances else 0
            # Compute the ratio compared to the optimal distance
            if optimal_distance > 0:
                distance_ratios.append(avg_actual_distance / optimal_distance)
            else:
                distance_ratios.append(float('inf'))  # Handle improbable case where optimal distance is zero
    
    # Save the plot of the ratios
    save_plot(alphas, "Average Distance Ratio", distance_ratios)

def setup_files(alphas):

    files = [f"Car Simulation/Tests/Results/Data/Exp_{alpha}" for alpha in alphas]
    control_file = f"Car Simulation/Tests/Results/Data/Exp_0.05_Full"

    try:
        os.remove(control_file)
    except FileNotFoundError:
        pass
    with open(control_file, 'a') as f:
        f.write(f"{'Result':<15}|{'Distance':<20}|{'Calculations':<15}|{'Scans':<15}|Time\n")

    for file in files:
        try:
            os.remove(file) # Delete file
        except FileNotFoundError:
            pass    
        with open(file, 'a') as f:
            f.write(f"{'Result':<15}|{'Distance':<20}|{'Calculations':<15}|{'Scans':<15}|Time\n")


def save_data(filename, data):
    # Open the file in append mode
    with open(filename, 'a') as file:
        # Append a line of text
        file.write(f"{data[0]:<15}|{data[1]:<20}|{data[2]:<15}|{data[3]:<15}|{data[4]}\n")
        
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

def gen_stats(i, alphas, lat_size, robot_coords, robot_radius, goal_coords, obstacle_count, obstacle_max_radius, max_its):
    vision = lat_size / 5
    createConfig((lat_size, lat_size), robot_coords, goal_coords, obstacle_count, obstacle_max_radius)

    # Initialize the "full experiment" first
    filename_full = f"Car Simulation/Tests/Results/Data/Exp_0.05_Full"
    radius = robot_radius
    robot = Control_Robot(robot_coords, radius=radius, vision=vision, sensor_tolerance=15, scale=0.05, tolerance=1)
    radius = lat_size / 10
    lat = Control_Lattice(lat_size, lat_size, robot, Goal(radius, goal_coords), safe_distance=1 + 0.075)
    lat.obstacleMap = loadConfig()

    # Run the full experiment
    reached, route_len, calculations, scans, time = lat.plotPath(i, 0.05, skip=1, max_its=max_its, full=True)
    save_data(filename_full, (reached, route_len, calculations, scans, time))

    # If the goal is not reached in the full experiment, skip further tests
    if not reached:
       return

    # Perform experiments for other alphas if the full experiment succeeded
    for alpha in alphas:
        filename = f"Car Simulation/Tests/Results/Data/Exp_{alpha}"
        
        radius = robot_radius
        robot = Control_Robot(robot_coords, radius=radius, vision=vision, sensor_tolerance=15, scale=alpha, tolerance=1)
        radius = lat_size / 10
        lat = Control_Lattice(lat_size, lat_size, robot, Goal(radius, goal_coords), safe_distance=1 + 0.075)
        lat.obstacleMap = loadConfig()

        # Run the experiment and save data
        data = lat.plotPath(i, alpha, skip=1, max_its=max_its)
        save_data(filename, data)
        lat.resetRobot()


alphas = [1, 0.5, 0.4, 0.3, 0.2, 0.1]
lat_size = 100
robot_radius = 2
obstacle_max_radius = 2
obstacle_count = 10
exp_count = 400
robot_coords = (lat_size - 2 * robot_radius, 1 + 2 * robot_radius)
goal_coords = (0 + lat_size/10,lat_size - lat_size/10)
max_its = 350


#subprocess.run(["cleanup.bat"], shell= False)

#setup_files(alphas)

#for i in tqdm(range(exp_count)):
#    gen_stats(i, alphas, lat_size, robot_coords, robot_radius, goal_coords, obstacle_count, obstacle_max_radius, max_its)

plot_data(alphas)
plot_data([0.05], full = True)
plot_distance(alphas, 0.05)