import numpy as np
import networkx as nx
import math
import matplotlib.pyplot as plt


class DualLattice:
    # Variables

    # Methods
    def __init__(self, filename, h=1):
        # self.D = nx.Graph()
        dimer_adj = np.genfromtxt(filename, dtype=float, delimiter="\t")
        d_adj = np.asmatrix(dimer_adj)
        d_adj = d_adj[0:, 0:-1]
        self.D = nx.from_numpy_matrix(d_adj)

    def draw_dual_lattice(self):
        # node_size = 400 / math.sqrt(self.D.number_of_nodes())
        nx.draw(self.D)

        plt.axis('equal')
        plt.axis('off')
        plt.show()
