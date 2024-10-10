import numpy as np

from sphere import Sphere

class Robot(Sphere):
    def __init__(self, radius, coords, vision):
        super().__init__(radius, coords)
        self.vision = vision
        self.path = []
        self.knownObstacles = []

    def isStuck(self) -> bool:
        return len(self.path) >= 2 and self.path[-1] == self.path[-2]

    def findAndAddNewObstacles(self, obstacles):
        for obstacle in obstacles:
            if self.getDistance(obstacle.coords[0], obstacle.coords[1]) <= self.vision and obstacle not in self.knownObstacles:
                self.knownObstacles.append(obstacle)

#    def move(self, stepSize, Utot, Fx, Fy):
#        # Save the current position to the path
#        self.path.append(self.coords)
#
#        # Get the current position indices in the grid (interpreting correctly)
#        i = min(max(int(self.coords[1]), 1), Fx.shape[1] - 2)  # Interpret y as x-axis
#        j = min(max(int(self.coords[0]), 1), Fy.shape[0] - 2)  # Interpret x as y-axis
#
#        # Extract the 3x3 neighborhood around the current position
#        neighborhood = Utot[i-1:i+2, j-1:j+2]
#
#        # Get the coordinates of the minimum potential within the 3x3 neighborhood
#        min_coords_local = np.unravel_index(np.argmin(neighborhood), neighborhood.shape)
#
#        # Calculate the global coordinates by adjusting the local coordinates
#        new_i = i - 1 + min_coords_local[0]  # New global i (y)
#        new_j = j - 1 + min_coords_local[1]  # New global j (x)
#
#        # Update the robot's coordinates based on the calculated global coordinates
#        self.coords = (new_j, new_i)  # Update global position (x, y)

    def move(self, stepSize, Utot, Fx, Fy):
      self.path.append(self.coords)
      i = min(max(int(self.coords[1]), 1), Fx.shape[1] - 2)  # Interpret y as x-axis
      j = min(max(int(self.coords[0]), 1), Fy.shape[0] - 2)  # Interpret x as y-axis 

      angle = np.arctan2(Fy[i, j], Fx[i, j])
      direction = (int(round(4 * angle / np.pi + 8)) % 8)
      if direction == 0:      # East (Right)
          dx, dy = 1, 0
      elif direction == 1:    # Northeast
          dx, dy = 1, 1
      elif direction == 2:    # North (Up)
          dx, dy = 0, 1
      elif direction == 3:    # Northwest
          dx, dy = -1, 1
      elif direction == 4:    # West (Left)
          dx, dy = -1, 0
      elif direction == 5:    # Southwest
          dx, dy = -1, -1
      elif direction == 6:    # South (Down)
          dx, dy = 0, -1
      elif direction == 7:    # Southeast
          dx, dy = 1, -1

      self.coords = (self.coords[0] + dx * stepSize, self.coords[1] + dy * stepSize)