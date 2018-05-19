import numpy as np
import networkx as nx
import math
import matplotlib.pyplot as plt

class TriangularLattice:
    # Variables

    # Methods
    def __init__(self, filename, h=1):
        self.S = nx.Graph()
        spin_data = np.genfromtxt(filename, dtype=float, delimiter="\t")

        i_max = max(spin_data[:, 0])
        j_max = max(spin_data[:, 1])
        self.cols = range(int(i_max) + 1)
        self.rows = range(int(j_max) + 1)

        spin_pos = zip(spin_data[:, 0], spin_data[:, 1])
        spin_val = spin_data[:, 2]
        for ind, s_pos in enumerate(spin_pos):
            self.S.add_node(s_pos, spin=spin_val[ind])

        nx.set_node_attributes(self.S, 'none', 'color')

        for i in self.cols:
            for j in self.rows:
                if (i, j) in self.S:
                    if i < i_max and j < j_max:
                        self.S.add_edge((i, j), (i + 1, j))
                        self.S.add_edge((i, j), (i, j + 1))
                        self.S.add_edge((i + 1, j), (i, j + 1))
                    elif i == i_max and j < j_max:
                        self.S.add_edge((i, j), (i, j + 1))
                    elif j == j_max and i < i_max:
                        self.S.add_edge((i, j), (i + 1, j))
        # positions
        for i in self.cols:
            for j in self.rows:
                if (i, j) in self.S:
                    x = (j * 0.5) + i
                    y = j * math.sqrt(3) / 2 * h
                    self.S.node[(i, j)]['pos'] = (x, y)

        remove = [node for node, data in self.S.nodes(data=True) if data['spin'] == 0]
        self.S.remove_nodes_from(remove)
        # detect triangles cycls_3 = [c for c in nx.cycle_basis(G) if len(c)==3]

    def draw_tri_lattice(self):
        positions = nx.get_node_attributes(self.S, 'pos')
        color_map = []
        for node, data in self.S.nodes(data=True):
            if data['spin'] == 1:
                color_map.append('b')  # blue color
            elif data['spin'] == -1:
                color_map.append('r')  # red color

        node_size = 400 / math.sqrt(self.S.number_of_nodes())
        nx.draw_networkx_nodes(self.S,
                               pos=positions,
                               with_labels=False,
                               node_color=color_map,
                               node_size=node_size)
        nx.draw_networkx_edges(self.S,
                               pos=positions,
                               alpha=0.4)

        plt.axis('equal')
        plt.axis('off')
