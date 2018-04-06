import numpy as np
import networkx as nx
import math
import matplotlib.pyplot as plt


class DualLattice:
    # Variables

    # Methods
    def __init__(self, filename, h=1):
        dimer_adj = np.genfromtxt(filename, dtype=float, delimiter="\t")
        d_adj = np.asmatrix(dimer_adj)
        d_adj = d_adj[0:, 0:-1]
        d_start = nx.from_numpy_matrix(d_adj)

        self.N = int(math.sqrt(len(d_adj[:, 0])/2))
        self.rows = range(self.N)
        self.cols = range(self.N * 2)

        node_map = {}
        for j in self.rows:
            for i in self.cols:
                ind = j * 2 * self.N + i
                node_map[ind] = (i, j)

        self.D = nx.relabel_nodes(d_start, node_map)

        hh = math.sqrt(3) / 2 * h
        for i in self.cols:
            for j in self.rows:
                if (i, j) in self.D:
                    if i % 2 == 0:  # if i even
                        x = (j + 1) * 0.5 + i / 2
                        y = j * hh + 1/3 * hh
                        self.D.node[(i, j)]['pos'] = (x, y)
                    else:  # if i odd
                        x = j * 0.5 + (i + 1) * 0.5
                        y = j * hh + 2 / 3 * hh
                        self.D.node[(i, j)]['pos'] = (x, y)

    # Note that we don't show the periodic edges.
    def draw_dual_lattice(self):
        positions = nx.get_node_attributes(self.D, 'pos')

        elarge = [(u, v) for (u, v, d) in self.D.edges(data=True) if d['weight'] > 0.0
        and abs(u[0] - v[0]) + abs(u[1] - v[1]) < self.N]
        esmall = [(u, v) for (u, v, d) in self.D.edges(data=True) if d['weight'] <= 0.0
        and abs(u[0] - v[0]) + abs(u[1] - v[1]) < self.N]

        nx.draw_networkx_edges(self.D,
                               pos=positions,
                               edgelist=elarge,
                               width=3)
        nx.draw_networkx_edges(self.D,
                               pos=positions,
                               edgelist=esmall,
                               width=1)

        plt.axis('equal')
        plt.axis('off')
