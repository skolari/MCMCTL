from TriangularLattice import TriangularLattice
from DualLattice import DualLattice

filename = 'Outputfiles/Spin1.dat'
filename2 = 'Outputfiles/DimerAdj1.dat'
triangular_lattice = TriangularLattice(filename)
dual_lattice = DualLattice(filename2)
# triangular_lattice.draw_tri_lattice()
dual_lattice.draw_dual_lattice()
