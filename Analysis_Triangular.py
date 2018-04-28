from DrawPlot import DrawPlot


filename_s = 'Outputfiles/test29_Spin.dat'
filename_d = 'Outputfiles/test29_DimerAdj.dat'
filename_e = 'Outputfiles/test_Energy.dat'
draw_plot = DrawPlot(filename_s, filename_d, filename_e)
draw_plot.draw_spin_and_dual_lattice()
draw_plot.draw_cv()
draw_plot.get_entropy()
draw_plot.draw_e()

filename_s = 'Outputfiles/test5_Spin.dat'
filename_d = 'Outputfiles/test5_DimerAdj.dat'
draw_plot = DrawPlot(filename_s, filename_d, filename_e)
# draw_plot.draw_spin_and_dual_lattice()
