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
        rows = range(int(i_max) + 1)
        cols = range(int(j_max) + 1)

        spin_pos = zip(spin_data[:, 0], spin_data[:, 1])
        spin_val = spin_data[:, 2]
        for ind, s_pos in enumerate(spin_pos):
            self.S.add_node(s_pos, spin=spin_val[ind])

        nx.set_node_attributes(self.S, 'none', 'color')

        # no periodic bc at the moment some error potential
        for i in rows:
            for j in cols:
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
        for i in rows:
            for j in cols:
                if (i, j) in self.S:
                    x = (j * 0.5) + i
                    y = j * math.sqrt(3) / 2 * h
                    self.S.node[(i, j)]['pos'] = (x, y)

        remove = [node for node, data in self.S.nodes(data=True) if data['spin'] == 0]
        self.S.remove_nodes_from(remove)

    def draw_tri_lattice(self):
        positions = nx.get_node_attributes(self.S, 'pos')
        color_map = []
        for node, data in self.S.nodes(data=True):
            if data['spin'] == 1:
                color_map.append('b')  # blue color
            elif data['spin'] == -1:
                color_map.append('r')  # red color

        nx.draw(self.S, positions, node_color=color_map)
        plt.axis('equal')
        plt.show()