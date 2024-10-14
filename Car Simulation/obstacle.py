
import numpy as np
from sphere import Sphere

class Obstacle(Sphere):
    def __init__(self, radius, coords):
        super().__init__(radius, coords)