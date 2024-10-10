from sphere import Sphere

class Gravitational_Sphere(Sphere):
    def __init__(self, radius, coords, attractionConstant):
        super().__init__(radius, coords)
        self.kG = attractionConstant

    def getForces(self, X, Y):
        pass
    def getPotential(self, X, Y):
        pass