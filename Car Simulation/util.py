import numpy as np
from   random import randint as rd

def get_bounded_indexes(coords, shapes):
    i = np.clip(int(np.round(coords[1])), 0, shapes[0] - 1)
    j = np.clip(int(np.round(coords[0])), 0, shapes[1] - 1)
    return (i, j)

def normalize_radians(angle):
    """ Normalize an angle to be within the range [0, 2π) radians. """
    normalized_angle = angle % (2 * np.pi)
    if normalized_angle < 0:
        normalized_angle += 2 * np.pi
    return normalized_angle

def getDistanceFromCoordinate(x, y, x2, y2) -> float:
    return np.sqrt((x - x2)**2 + (y - y2)**2)

def bresenham(start: tuple[int, int], end: tuple[int, int]) -> list[tuple[int, int]]:
    """Return a list of coordinates from `start` to `end` including both endpoints.

    This implementation is symmetrical. It uses the same set of coordinates in either direction.

    >>> bresenham_line((0, 0), (3, 4))
    [(0, 0), (1, 1), (1, 2), (2, 3), (3, 4)]
    >>> bresenham_line((3, 4), (0, 0))
    [(3, 4), (2, 3), (1, 2), (1, 1), (0, 0)]
    """
    # Setup initial conditions
    x1, y1 = start
    x2, y2 = end
    dx = x2 - x1
    dy = y2 - y1

    # Determine how steep the line is
    is_steep = abs(dy) > abs(dx)

    # Rotate line
    if is_steep:
        x1, y1 = y1, x1
        x2, y2 = y2, x2

    # Swap start and end points if necessary and store swap state
    swapped = False
    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1
        swapped = True

    # Recalculate differentials
    dx = x2 - x1
    dy = y2 - y1

    # Calculate error
    error = dx // 2
    ystep = 1 if y1 < y2 else -1

    # Iterate over bounding box generating points between start and end
    y = y1
    points = []
    for x in range(x1, x2 + 1):
        coord = (y, x) if is_steep else (x, y)
        points.append(coord)
        error -= abs(dy)
        if error < 0:
            y += ystep
            error += dx

    # Reverse the list if the coordinates were swapped
    if swapped:
        points.reverse()
    return points

def createConfig(sizes, robot_coords, goal_coords, num, max_radius):
    """
    Create a matrix with circular clusters (spheres) of 1's.
    
    Parameters:
        sizes (tuple): Dimensions of the matrix (rows, cols).
        num (int): Number of spheres to add.
        max_radius (int): Maximum radius of each sphere.
    """
    matrix = np.zeros(sizes, dtype=int)
    for _ in range(num):
        # Random center point for the sphere
        center_x = rd(0, sizes[1] - 1)
        center_y = rd(0, sizes[0] - 1)
        radius = rd(1, max_radius)

        # Create the sphere by setting points within the radius to 1
        for x in range(center_x - radius, center_x + radius + 1):
            for y in range(center_y - radius, center_y + radius + 1):
                # Check if (x, y) is within bounds and within the circle
                if (0 <= x < sizes[1] and 0 <= y < sizes[0] and
                        (x - center_x)**2 + (y - center_y)**2 <= radius**2 and
                        (x, y) != robot_coords and (x, y) != goal_coords):
                    matrix[y, x] = 1

    np.savetxt("Car Simulation/Tests/matrix.txt", matrix, fmt='%d', delimiter=",")

def loadConfig():
    """
    Load the configuration matrix from the text file.
    
    Returns:
        np.ndarray: Loaded matrix of 1's and 0's.
    """
    return np.loadtxt("Car Simulation/Tests/matrix.txt", dtype=int, delimiter=",")
