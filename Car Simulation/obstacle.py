
import numpy as np

from gravitational_sphere import Gravitational_Sphere

class Obstacle(Gravitational_Sphere):
    def __init__(self, radius, coords, repulsionConstant, safeDistance):
        super().__init__(radius, coords, repulsionConstant)
        self.g0 = safeDistance + radius

    def getForces(self, X, Y):
        Fx = np.zeros_like(X)
        Fy = np.zeros_like(Y)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                rr = self.getDistance(X[i, j], Y[i, j]) - self.radius
                theta = np.arctan2(self.coords[1] - Y[i, j], self.coords[0] - X[i, j])
                
                if 0 < rr < self.g0:
                    frep = self.kG * (1/rr - 1/self.g0) / rr**2
                    Fx[i, j] = -frep*np.cos(theta)
                    Fy[i, j] = -frep*np.sin(theta)
                if np.sqrt(Fx[i, j]**2 + Fy[i,j]**2) > self.g0 * 2:
                    Fx[i, j] = Fx[i, j] / np.sqrt(Fx[i, j]**2 + Fy[i, j]**2) * self.g0 * 2 
                    Fy[i, j] = Fy[i, j] / np.sqrt(Fx[i, j]**2 + Fy[i, j]**2) * self.g0 * 2     

        return Fx, Fy

    def getPotential(self, X, Y):
        Urep = np.zeros_like(X, dtype = float)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                rr = self.getDistance(X[i, j], Y[i, j])
                if rr == 0:
                    Urep[i, j] = np.inf
                elif rr > self.g0:
                    Urep[i, j] = 0
                else:
                    Urep[i, j] = 0.5 * self.kG * (1/rr - 1/self.g0)**2
        return Urep