from DrawPlot import DrawPlot


filename_s = 'Outputfiles/Spin11.dat'
filename_d = 'Outputfiles/DimerAdj11.dat'
draw_plot = DrawPlot(filename_s, filename_d)
draw_plot.draw_spin_lattice()
# fehler entdeckt mit boundary...