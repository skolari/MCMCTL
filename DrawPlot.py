import matplotlib.pyplot as plt
import numpy as np
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
        self.E = data[:, 1]
        self.cv = -np.diff(self.E)/np.diff(self.beta)*self.beta[:-1]*self.beta[:-1]

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
        plt.plot(self.beta, self.E)
        plt.xlabel('beta')
        plt.ylabel('Energy')
        plt.title('Energy - beta')
        plt.grid(True)
        plt.show()

    def draw_cv(self):
        plt.plot(self.beta[:-1], self.cv)
        plt.xlabel('beta')
        plt.ylabel('C_v')
        plt.title('Energy - C_v')
        plt.grid(True)
        plt.show()

    def get_entropy(self):
        entropy = log(2) + np.trapz(self.cv/self.beta[:-1], x=self.beta[:-1])
        print(entropy)
