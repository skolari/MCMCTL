import matplotlib.pyplot as plt
import numpy as np
# import scipy.signal as sp
from math import log
from TriangularLattice import TriangularLattice
from DualLattice import DualLattice


class DrawPlot:
    # Variables

    # Methods
    def __init__(self, filename_t, filename_d, filename_e, h=1):
        self.triangular_lattice = TriangularLattice(filename_t, h)
        self.dual_lattice = DualLattice(filename_d, h)
        data = np.genfromtxt(filename_e, dtype=float, delimiter="\t")
        self.beta = data[:, 0]
        self.T = [1/x for x in self.beta]
        self.E = data[:, 1]
        # self.E = sp.savgol_filter(self.E1, 15, 6)
        self.cv = data[:, 2]

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

    def draw_e(self):
        plt.plot(self.T, self.E)
        plt.xlabel('T')
        plt.ylabel('Energy')
        plt.title('Energy - T')
        plt.grid(True)
        plt.show()

    def draw_cv(self):
        plt.plot(self.T, self.cv)
        plt.xlabel('T')
        plt.ylabel('C_v')
        plt.title('Energy - C_v')
        plt.grid(True)
        plt.show()

    def get_entropy(self):
        entropy = log(2) - np.trapz(self.cv/self.T, x=self.T)
        print(entropy)
