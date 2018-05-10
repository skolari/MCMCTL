from DrawPlot import DrawPlot


filename_s = 'Outputfiles/funk0.055225_Spin.dat'
filename_d = 'Outputfiles/funk0.055225_DimerAdj.dat'
filename_e = 'Outputfiles/Energy_and_Cv.dat'
draw_plot = DrawPlot(filename_s, filename_d, filename_e)
draw_plot.draw_spin_and_dual_lattice()
#draw_plot.draw_cv()
#draw_plot.get_entropy()
#draw_plot.draw_e()

# filename_s = 'Outputfiles/nr_0_Spin.dat'
# filename_d = 'Outputfiles/nr_0_DimerAdj.dat'
# draw_plot = DrawPlot(filename_s, filename_d, filename_e)
# draw_plot.draw_spin_and_dual_lattice()
