from DrawPlot import DrawPlot


filename_s = 'Outputfiles/Spin33.dat'
filename_d = 'Outputfiles/DimerAdj33.dat'
draw_plot = DrawPlot(filename_s, filename_d)
draw_plot.draw_spin_and_dual_lattice()
# fehler entdeckt mit boundary...