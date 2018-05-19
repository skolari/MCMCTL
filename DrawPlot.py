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
        self.cv = data[:, 2]
        self.bc = data[:, 3]
        self.n_string = data[:, 4]

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
        plt.title('$T - C_v$')
        plt.grid(True)
        plt.show()

    def draw_bc(self):
        plt.plot(self.T, self.bc)
        plt.xlabel('T')
        plt.ylabel('B_c')
        plt.title('T - Binder cumulant')
        plt.grid(True)
        plt.show()

    def draw_n_string(self):
        plt.plot(self.T, self.n_string)
        plt.xlabel('T')
        plt.ylabel('n_string')
        plt.title('T - n_string')
        plt.grid(True)
        plt.show()

    def get_entropy(self):
        entropy = log(2) - np.trapz(self.cv * self.beta, x=self.T)
        print(entropy)

    def draw_observables(self):
        self.draw_cv()
        self.get_entropy()
        self.draw_e()
        self.draw_bc()
        self.draw_n_string()
