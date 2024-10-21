from goal import Goal
from robot import Robot
from lattice import Lattice
from   scipy.ndimage import distance_transform_edt as bwdist

class Modified_Lattice(Lattice):
    def __init__(self, rows: int, columns: int, robot: Robot, goal: Goal, **kwargs):
        self.kV = kwargs.get('virtual_repulsion', 800)
        super().__init__(rows, columns, robot, goal, **kwargs)

    def getPotential(self):
      if len(self.goals) > 0:
          g = self.goals[0].coords
          Uatt = self.goal_attraction * ((self.X - g[0])**2 + (self.Y - g[1])**2)
    
      d = bwdist(self.robot.digitalMap==0)
      rescaleD = (d/100.) + 1
      Urep = self.obstacle_repulsion * ((1./rescaleD - 1/self.safe_distance)**2)
      Uvir = self.kV / rescaleD
      Urep [rescaleD > self.safe_distance] = 0
      Uvir [rescaleD > self.safe_distance] = 0
      return Uatt + Urep + Uvir
    