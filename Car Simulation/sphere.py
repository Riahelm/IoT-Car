from util import getDistanceFromCoordinate

class Sphere:
    def __init__(self, radius, coords):
        self.radius = radius
        self.coords = coords

    def getDistance(self, x, y) -> float:
        return getDistanceFromCoordinate(self.coords[0], self.coords[1], x, y)

    def isTouching(self, Sphere) -> bool:
        return self.getDistance(Sphere.coords[0], Sphere.coords[1]) <= self.radius + Sphere.radius

    def isInside(self, Sphere) -> bool:
        return self.getDistance(Sphere.coords[0], Sphere.coords[1]) <= Sphere.radius

    def isOutside(self, Sphere) -> bool:
        return self.getDistance(Sphere.coords[0], Sphere.coords[1]) >= Sphere.radius
    