import parallel_utils
import itertools

def make_tables_and_plots(input_file, plot_label):
  # Tables
  parallel_utils.run('./makeTables "%s" > "%s"' % (input_file, input_file.replace(".root", ".txt")))

  # Plots
  plot_styles = ["std", "std_linear"]
  plot_years = ["", "_2016", "_2017", "_2018"]
  for plot_type in [x+y for x in plot_styles for y in plot_years]:
    parallel_utils.run('./makePlots "%s" "%s" "%s"' % (plot_type, input_file, plot_label))
