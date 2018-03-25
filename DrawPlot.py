import matplotlib.pyplot as plt
from TriangularLattice import TriangularLattice
from DualLattice import DualLattice


class DrawPlot:
    # Variables

    # Methods
    def __init__(self, filename_t, filename_d, h=1):
        self.triangular_lattice = TriangularLattice(filename_t, h)
        self.dual_lattice = DualLattice(filename_d, h)

    def draw_spin_and_dual_lattice(self):
        self.triangular_lattice.draw_tri_lattice()
        self.dual_lattice.draw_dual_lattice()
        plt.show()

    def draw_spin_lattice(self):
        self.triangular_lattice.draw_tri_lattice()
        plt.show()

    def draw_dual_lattice(self):
        self.dual_lattice.draw_dual_lattice()
        plt.show()
