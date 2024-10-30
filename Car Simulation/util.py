import numpy as np

def get_bounded_indexes(coords, shapes):
    i = np.clip(int(coords[1]), 0, shapes[0] - 1)
    j = np.clip(int(coords[0]), 0, shapes[1] - 1)
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
