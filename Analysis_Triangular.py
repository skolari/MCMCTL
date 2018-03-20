import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

# init graphs
S = nx.Graph()
SpinData = np.genfromtxt('Outputfiles/Spin1.dat',dtype=float, delimiter="\t")
i_len = int(max(SpinData[:, 0]) + 1)
j_len = int(max(SpinData[:, 1]) + 1)
Spin_pos = [tuple([SpinData[i][0], SpinData[i][1]]) for i in range(len(SpinData[:, 1])-1)]
S.add_nodes_from(Spin_pos)
# no periodic bc at the moment
for i in range(0, i_len - 1):
    for j in range(0, j_len - 1):
        S.add_edge((i, j), (i + 1, j))
        S.add_edge((i, j), (i, j + 1))
        S.add_edge((i, j), (i + 1, j + 1))
# print


# Draw
pos = dict((n, n) for n in S.nodes())
# labels = dict(((i, j), i * 10 + j) for i, j in S.nodes())
nx.draw_networkx(S, pos=pos)

print(S.number_of_edges())

plt.axis('off')
plt.show()


