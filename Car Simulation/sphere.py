from util import getDistanceFromCoordinate
import numpy as np
class Sphere:
    def __init__(self, radius, coords):
        self.radius = radius
        self.coords = np.array([coords[0], coords[1]])

    def get_distance(self, x, y) -> float:
        return getDistanceFromCoordinate(self.coords[0], self.coords[1], x, y)

    def get_distance_coords(self, coords) -> float:
        return getDistanceFromCoordinate(self.coords[0], self.coords[1], coords[0], coords[1])
    
    def isTouching(self, Sphere) -> bool:
        return self.get_distance(Sphere.coords[0], Sphere.coords[1]) <= self.radius + Sphere.radius

    def isInside(self, Sphere) -> bool:
        return self.get_distance(Sphere.coords[0], Sphere.coords[1]) <= Sphere.radius

    def isOutside(self, Sphere) -> bool:
        return self.get_distance(Sphere.coords[0], Sphere.coords[1]) >= Sphere.radius
    