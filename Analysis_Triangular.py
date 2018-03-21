import numpy as np
import networkx as nx
import math
import matplotlib.pyplot as plt

# init graphs
S = nx.Graph()
SpinData = np.genfromtxt('Outputfiles/Spin1.dat', dtype=float, delimiter="\t")

i_max = max(SpinData[:, 0])
j_max = max(SpinData[:, 1])
rows = range(int(i_max) + 1)
cols = range(int(j_max) + 1)

Spin_pos = zip(SpinData[:, 0], SpinData[:, 1])
Spin_val = SpinData[:, 2]
for ind, s_pos in enumerate(Spin_pos):
    S.add_node(s_pos, spin=Spin_val[ind])

nx.set_node_attributes(S, 'none', 'color')

# no periodic bc at the moment some error potential
for i in rows:
    for j in cols:
        if (i, j) in S:
            if i < i_max and j < j_max:
                    S.add_edge((i, j), (i + 1, j))
                    S.add_edge((i, j), (i, j + 1))
                    S.add_edge((i + 1, j), (i, j + 1))
            elif i == i_max and j < j_max:
                S.add_edge((i, j), (i, j + 1))
            elif j == j_max and i < i_max:
                S.add_edge((i, j), (i + 1, j))
# positions
for i in rows:
    for j in cols:
        if (i, j) in S:
            x = (j * 0.5) + i
            y = j * math.sqrt(3) / 2
            S.node[(i, j)]['pos'] = (x, y)

remove = [node for node, data in S.nodes(data=True) if data['spin'] == 0]
S.remove_nodes_from(remove)

color_map = []
for node, data in S.nodes(data=True):
    if data['spin'] == 1:
        color_map.append('b')  # blue color
    elif data['spin'] == -1:
        color_map.append('r')  # red color

# Draw

positions = nx.get_node_attributes(S, 'pos')

nx.draw(S, positions, node_color=color_map)
plt.axis('equal')
plt.show()
