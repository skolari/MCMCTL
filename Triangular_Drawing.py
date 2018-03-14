
# coding: utf-8

# In[1]:


import numpy as np
from matplotlib.collections import LineCollection
import matplotlib.pyplot as plt


# In[2]:


#############################################################
                ## BASES FOR DRAWING ##
#############################################################


# In[3]:


# create a function to fix the periodic boundary conditions
def fixbc(i, j, l, L):
    '''
        For a lattice side size L, this function handles the periodic boundary conditions by returning the corresponding
        value of i, j, l if they match a point which is just outside the borders of the considered cell.
    '''
    if i == 2*L : # bottom right mapped to top left
        i = 0
        j += L
    if j == 2*L: # top mapped to bottom
        i += L
        j = 0
    if i+j == L-2: # bottom left mapped to top right
        i += L
        j += L
    if i+j == 3*L-1: # top right mapped to bottom left
        i -= L
        j -= L
    if j == -1: # bottom mapped to top
        i -= L
        j = 2*L-1
    if i == -1: # top left mapped to bottom right
        i = 2*L-1
        j -= L
    return (i, j, l)


# In[4]:


def onestate_dimers2spins(sidlist, didlist, L, state):
    '''
        For a known state of the dual lattice (i.e. for each bond, is there or not a dimer), returns the corresponding
        spin state.
    '''
    #initialise the spins in a meaningless way
    spinstate = [0 for _ in range(len(sidlist))]
    #initialise the first spin randomly
    s_val =  np.random.randint(0, 2)*2-1
    s = spinstate[sidlist[0]] = s_val
    for it in range(0, len(sidlist)-1):
        dim_id = didlist[it]
        spin_id = sidlist[it+1]
        s = spinstate[spin_id] = s * state[dim_id]
    return spinstate


# In[5]:


from functools import lru_cache

@lru_cache(maxsize = None)
def graphdice(L, a):
    '''
        For the dice lattice:
        Returns two vertex <-> (i, j, l) tables, a table linking edge to the two corresponding vertices, as well as a dictionary giving the position (x,y) of each vertex
    '''
    #vertices table
    v_ijl = [(i, j, l) for i in range(-1, 2*L+1) for j in range(-1, 2*L+1) for l in range(3) if (i+j >= L-2) and (i+j <= 3*L - 1)]
    
    #vertices dictionary:
    ijl_v = {}
    for v, triplet in enumerate(v_ijl):
        ijl_v[triplet] = v
    
    #create the edges (v1, v2)
    #table of where to look at 
    nv = [(0, -1, 2), (0, 0, 1), (0, 0, 2), (-1, 1, 1), (-1, 0, 2), (-1, 0, 1)]
    #edge -> vertex: l from 0 to 5 indicates the edge
    e_2v = [((i, j, 0),(i + nv[l][0], j + nv[l][1], nv[l][2])) for i in range(-1, 2*L+1) for j in range(-1, 2*L+1) for l in range(6) if (i+j >= L-2) and (i+j <= 3*L - 1)]
    e_2v = [(ijl_v[i, j, l], ijl_v[ni, nj, nl]) for ((i, j, l), (ni, nj, nl)) in e_2v if (ni, nj, nl) in v_ijl]
    #position
    pos = {} #empty dictionary
    for v, (i,j,l) in enumerate(v_ijl):
        if l == 0:
            x = a * (i + j / 2.0)
            y = a * j * np.sqrt(3) / 2.0
        if l == 1:
            x = a * (i + j / 2.0 + 1.0 / 2.0)
            y = a * (j * np.sqrt(3) / 2.0 - 1.0 / (2.0 * np.sqrt(3.0)))
        if l == 2:
            x = a * (i + j / 2.0 + 1.0 / 2.0)
            y = a * (j * np.sqrt(3) / 2.0 + 1.0/ (2.0 * np.sqrt(3.0)))
        pos[v] = (x,y)
    return v_ijl, ijl_v, e_2v, pos


# In[6]:


def reducedgraphdice(L, a, d_ijl, v_ijl, ijl_v):
    #table of where to look at 
    nv = [(0, -1, 2), (0, 0, 1), (0, 0, 2), (-1, 1, 1), (-1, 0, 2), (-1, 0, 1)]
    #dimer -> vertex: l from 0 to 5 indicates the edge
    d_2v = [((i, j, 0),(i + nv[l][0], j + nv[l][1], nv[l][2])) for (i, j, l) in d_ijl]
    d_2v = [(ijl_v[i, j, l], ijl_v[fixbc(ni, nj, nl, L)]) for ((i, j, l), (ni, nj, nl)) in d_2v]
    
    v2_d = {} 
    for d, (v1,v2) in enumerate(d_2v):
        v2_d[v1,v2] = d
        v2_d[v2,v1] = d
    
    return d_2v, v2_d


# In[7]:


#Function creating a list of vertices corresponding to the kagome
from functools import lru_cache

@lru_cache(maxsize = None)
def graphkag(L, a):
    '''
        For the kagomé lattice:
        Returns two vertex <-> (i, j, l) tables, a table linking edge to the two corresponding vertices, as well as a dictionary giving the position (x,y) of each vertex
    '''
    #vertices table
    sv_ijl = [(i, j, l) for i in range(-1, 2*L+1) for j in range(-1, 2*L+1) for l in range(3) if (i+j >= L-2) and (i+j <= 3*L - 1)]
    
    #vertices dictionary:
    ijl_sv = {}
    for sv, triplet in enumerate(sv_ijl):
        ijl_sv[triplet] = sv
    
    #create the edges (sv1, sv2)
    #table of where to look at 
    nv = [[(0, 0, 1), (1, 0, 2)],[(-1, 1, 0), (1, 0, 2)], [(0, 0, 1), (-1, 1, 0)]]
    #edge -> vertex: l from 0 to 5 indicates the edge
    e_2sv = [((i, j, l),(i + nv[l][u][0], j + nv[l][u][1], nv[l][u][2])) for i in range(-1, 2*L+1) for j in range(-1, 2*L+1) for l in range(3) for u in range(2) if (i+j >= L-2) and (i+j <= 3*L - 1)]
    e_2sv = [(ijl_sv[i, j, l], ijl_sv[ni, nj, nl]) for ((i, j, l), (ni, nj, nl)) in e_2sv if (ni, nj, nl) in sv_ijl]
    #position
    pos = {} #empty dictionary
    for sv, (i,j,l) in enumerate(sv_ijl):
        x = a * (i + j / 2.0)
        y = a * j * np.sqrt(3) / 2.0
        if l == 0:
            x += a / 2.0
        if l == 1:
            x += a / 4.0
            y += a * np.sqrt(3) / 4.0
        if l == 2:
            x -= a / 4.0
            y += a * np.sqrt(3) / 4.0
        pos[sv] = (x,y)
    return sv_ijl, ijl_sv, e_2sv, pos


# In[8]:


#############################################################
                ## DRAW A GRAPH ##
#############################################################


# In[9]:


### FUNCTIONS DRAWING EITHER NODES OR EDGES OF A GRAPH
def draw_nodes(pos, nodelist, c, **kargs):
    '''
        This function draws the nodes of a graph
    '''
    
    ax = plt.gca()

    xy = np.asarray([pos[v] for v in nodelist])
    node_collection = plt.scatter(xy[:, 0], xy[:, 1], c = c, **kargs)
    if isinstance(c, list):
        plt.colorbar()
    node_collection.set_zorder(2)

def draw_edges(pos, edgelist, *args, **kargs):
    '''
        This function draws the edges of a graph
    '''
    ax = plt.gca()

    edge_pos = np.asarray([(pos[e[0]], pos[e[1]]) for e in edgelist])
    edge_collection = LineCollection(edge_pos, *args, **kargs)
    edge_collection.set_zorder(1)  # edges go behind nodes
    ax.add_collection(edge_collection)


# In[10]:



#############################################################
                ## KAGOME LATTICE STATE ##
#############################################################


# In[14]:


def spinvertex2spin(L,a, ijl_s, sv_ijl):
    '''
        Given a list of spin vertices, associates to each one the corresponding spin
    '''
    sv_s = list()
    for sv, (i, j, l) in enumerate(sv_ijl):
        (ni, nj, nl) = fixbc(i, j, l, L)
        s = ijl_s[ni, nj, nl]
        sv_s.append(s)
    
    return sv_s


# In[15]:


def plot_kag_nodes(L, a, color='blue', s=20, **kargs):
    (sv_ijl, ijl_sv, e_2sv, poskag) = graphkag(L,a)
    draw_nodes(poskag, list(poskag.keys()), c = color, s = s, **kargs)

def plot_kag_edges(L, a, color='lightblue', **kargs):
    sv_ijl, ijl_sv, e_2sv, pos = graphkag(L, a)
    draw_edges(pos, e_2sv, **kargs)
    
def plot_kag_spins(spins, ijl_s, L, a, color = 'red', **kargs):
    """
        :param spins: list of spins indices (for instance [3, 5, 2, ...])
    """
    (sv_ijl, ijl_sv, e_2sv, poskag) = graphkag(L,a)
    sv_s = spinvertex2spin(L,a, ijl_s, sv_ijl)
    spinsthere = []
    for sv, ijl in enumerate(sv_ijl): #for each key
        if sv_s[sv] in spins:# look for corresponding spin in spins
            spinsthere.append(sv) #list of keys
    draw_nodes(poskag, spinsthere, c = color, **kargs)
    
def plot_kag_spinstate(spinstate, ijl_s, L, a, edge_color = 'lightblue', up_color = 'blue', down_color = 'red', **kargs):
    """
        :param spins: list of spins indices (for instance [3, 5, 2, ...])
    """
    plot_kag_nodes(L, a, **kargs)
    plot_kag_edges(L, a, color = edge_color, **kargs)
    spinsup = [s for s, sstate in enumerate(spinstate) if sstate == 1]
    spinsdown = [s for s, sstate in enumerate(spinstate) if sstate == -1]
    if len(spinsup) != 0:
        plot_kag_spins(spinsup, ijl_s, L, a, color = up_color)
    if len(spinsdown) != 0:
        plot_kag_spins(spinsdown, ijl_s, L, a, color = down_color)


# In[16]:


#FUNCTION ALLOWING TO PLOT THE FULL STATE

def plotstate(temp_id, L, d_ijl, ijl_s, sidlist, didlist, s2_d, states, dim_node_color = 'black', dim_color ='black', no_dim_color = 'lightgrey', spin_edge_color = 'lightblue', spin_up_color = 'blue', spin_down_color = 'red', **kargs):    

    a = 2 #lattice parameter
    spinstate = onestate_dimers2spins(sidlist, didlist, L, states[temp_id])
    plt.figure(figsize = (2*L,2*L))
    plt.axis('equal')
    plot_dice_dimerstate(states[temp_id], d_ijl, L, a, dim_node_color, dim_color, no_dim_color, **kargs)
    plot_kag_spinstate(spinstate, ijl_s, L, a, spin_edge_color, spin_up_color, spin_down_color, **kargs)
    plt.tick_params(  
        which = 'both',      # both major and minor ticks are affected
        bottom = 'off',      # ticks along the bottom edge are off
        top = 'off',         # ticks along the top edge are off
        labelbottom = 'off',
        left = 'off',
        right = 'off',
        labelleft = 'off') # labels along the bottom edge are off


# In[17]:


### SPIN SITES TABLE
# make the mapping between the spinsite index s\in {0,..., S-1} and the numbering (ijl)
def createspinsitetable(L):
    '''
        Creates the table of spin sites corresponding to a dice lattice of side size L.
        Returns a table identifing an int with the three coordinates of the spin site and a dictionnary identifying the
        three coordinates with the spin site's int index. This allows to handle other relations between spin sites in an
        easier way.
    '''
    s_ijl = [(i, j, l) for i in range(2*L) for j in range(2*L) for l in range(3) if (i+j > L-2) and (i+j < 3*L-1)]
    # dictionary
    ijl_s = {}
    for s, triplet in enumerate(s_ijl):
        ijl_s[triplet] = s
    return s_ijl, ijl_s


# In[18]:


def plot_function_kag(f, L, a, **kargs):
    '''
        Plots the function f over the kagomé lattice. f has to be defined on the "square" brillouin zone
        for k1, k2 in range(0, L), which will be the usual output of the FT
    '''
    #get the shape of the lattice
    (sv_ijl, ijl_sv, e_2sv, pos) = graphkag(L, a) # drawing lattice
    (s_ijl, ijl_s) = createspinsitetable(L) # lattice over which the function is defined
    
    
    #chose the size of the points in a way that compensates for the parameters
    s = (a*54/L)**2
    
    new_f = np.zeros(len(sv_ijl))
    for sv, (i, j, l) in enumerate(sv_ijl):
        new_f[sv] = f[ijl_s[fixbc(i, j, l, L)]]
    
    draw_function(pos, new_f, list(pos.keys()), s = s, marker = 'h', **kargs)
    
    plt.axis('equal')
    plt.tick_params(  
        which = 'both',      # both major and minor ticks are affected
        bottom = 'off',      # ticks along the bottom edge are off
        top = 'off',         # ticks along the top edge are off
        labelbottom = 'off',
        left = 'off',
        right = 'off',
        labelleft = 'off') # labels along the bottom edge are off
