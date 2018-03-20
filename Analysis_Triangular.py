import numpy as np
import networkx as nx
import math
import matplotlib.pyplot as plt

# init graphs
S = nx.Graph()
SpinData = np.genfromtxt('Outputfiles/Spin1.dat',dtype=float, delimiter="\t")
Spin_val = SpinData[:, 2]

i_max = max(SpinData[:, 0])
j_max = max(SpinData[:, 1])
rows = range(int(i_max)+1)
cols = range(int(j_max)+1)

Spin_pos = zip(SpinData[:, 0], SpinData[:, 1])
Spin_val = SpinData[:, 2]
Spin_couple = {pos: Spin_val[ind] for ind, pos in enumerate(Spin_pos)}
S.add_nodes_from(Spin_pos)
nx.set_node_attributes(S, 'spin', Spin_couple) # apparently they aren't set...

# no periodic bc at the moment
for i in rows:
    for j in cols:
        if i < i_max:
            if j < j_max:
                S.add_edge((i, j), (i + 1, j))
                S.add_edge((i, j), (i, j + 1))
                S.add_edge((i + 1, j), (i, j + 1))

# positions

for i in rows:
    for j in cols:
        x = (j * 0.5) + i
        y = j * math.sqrt(3)/2
        S.node[(i, j)]['pos'] = (x, y)

positions = nx.get_node_attributes(S, 'pos')
nx.draw(S, positions)

'''
ii = (i for i in cols for j in rows)
jj = (j for i in cols for j in rows)
xx = (0.5 * (j % 2) + i for i in cols for j in rows)
h = math.sqrt(3) / 2
yy = (h * j + .01 * i * i for i in cols for j in rows)
pos = {(i, j): (x, y) for i, j, x, y in zip(ii, jj, xx, yy) if (i, j) in S}

nx.set_node_attributes(S, pos, 'pos')
'''


# Draw
# pos = dict((n, n) for n in S.nodes())
# labels = dict(((i, j), i * 10 + j) for i, j in S.nodes())

plt.axis('equal')
plt.show()


