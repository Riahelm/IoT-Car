from typing import override
from robot import Robot
import numpy as np

class Third_Paper_Robot (Robot):
    def __init__(self, coords, **kwargs):
        super().__init__(coords, **kwargs)

    def isStuck(self, Utot) -> bool:
        res = False
        i = min(max(int(self.coords[0]), 0), Utot.shape[1] - 1)
        j = min(max(int(self.coords[1]), 0), Utot.shape[0] - 1)
        neighbours = Utot[i-1: i+2, j-1:j+2]
        local_minimum = np.unravel_index(np.argmin(neighbours), neighbours.shape)
        new_i = i - 1 + local_minimum[0]  # New global i (y)
        new_j = j - 1 + local_minimum[1]  # New global j (x)
        if (i, j) == (new_j, new_i):
            res = True
        return res