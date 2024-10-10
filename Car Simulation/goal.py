
import numpy as np

from gravitational_sphere import Gravitational_Sphere

class Goal(Gravitational_Sphere):
    def __init__(self, radius, coords, attractionConstant):
        super().__init__(radius, coords, attractionConstant)


    def getForces(self, X, Y):
        Fx = np.zeros_like(X)
        Fy = np.zeros_like(Y)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                ra = self.getDistance(X[i, j], Y[i, j]) + self.radius
                theta = np.arctan2(self.coords[1] - Y[i, j], self.coords[0] - X[i, j])
                Fx[i, j] = self.kG * (ra) * np.cos(theta)
                Fy[i, j] = self.kG * (ra) * np.sin(theta)
        return Fx, Fy

    def getPotential(self, X, Y):
        Uatt = np.zeros_like(X, dtype = float)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                distance = self.getDistance(X[i, j], Y[i, j])
                Uatt[i, j] = 0.5 * self.kG * (distance)**2
        return Uatt
