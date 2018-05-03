# Grab m_gg values from text files and sort them
mgg_ttHHadronic_workspace = []
mgg_ttHHadronic_looper = []

mgg_ttHLeptonic_workspace = []
mgg_ttHLeptonic_looper = []

with open("RooWorkspace_ttHHadronic.txt") as fin:
  for line in fin:
    if not line.strip(): continue
    if not (float(line) > 120 and float(line) < 130):
      mgg_ttHHadronic_workspace.append(float(line))

with open("Looper_ttHHadronic.txt") as fin:
  for line in fin:
    if not line.strip(): continue
    mgg_ttHHadronic_looper.append(float(line))

with open("RooWorkspace_ttHLeptonic.txt") as fin:
  for line in fin:
    if not line.strip(): continue
    if not (float(line) > 120 and float(line) < 130):
      mgg_ttHLeptonic_workspace.append(float(line))

with open("Looper_ttHLeptonic.txt") as fin:
  for line in fin:
    if not line.strip(): continue
    mgg_ttHLeptonic_looper.append(float(line))

mgg_ttHHadronic_workspace = sorted(mgg_ttHHadronic_workspace)
mgg_ttHHadronic_looper = sorted(mgg_ttHHadronic_looper)

mgg_ttHLeptonic_workspace = sorted(mgg_ttHLeptonic_workspace)
mgg_ttHLeptonic_looper = sorted(mgg_ttHLeptonic_looper)

with open("comparison_ttHHadronic.txt", "w") as fout:
  fout.write("All events with m_gg in [100, 120], [130, 180] GeV\n")
  fout.write("Workspace value & Looper value\n")
  idx_workspace = 0
  idx_looper = 0
  for i in range(max(len(mgg_ttHHadronic_workspace), len(mgg_ttHHadronic_looper))):
    value_workspace = str(format(mgg_ttHHadronic_workspace[idx_workspace], '.6f')) if idx_workspace < len(mgg_ttHHadronic_workspace) else "          "
    value_looper = str(format(mgg_ttHHadronic_looper[idx_looper], '.6f')) if idx_looper < len(mgg_ttHHadronic_looper) else "          "

    if value_workspace == value_looper:
      fout.write("%s  %s\n" % (value_workspace, value_looper))
    elif value_workspace == "          " or value_looper == "          ":
      fout.write("%s  %s\n" % (value_workspace, value_looper))
    else:
      if float(value_workspace) < float(value_looper):
        fout.write("%s  %s\n" % (value_workspace, "          ")) 
	idx_looper -= 1
      else:
        fout.write("%s  %s\n" % ("          ", value_looper))
	idx_workspace -= 1
    idx_workspace += 1
    idx_looper += 1

with open("comparison_ttHLeptonic.txt", "w") as fout:
  fout.write("All events with m_gg in [100, 120], [130, 180] GeV\n")
  fout.write("Workspace value & Looper value\n")
  idx_workspace = 0
  idx_looper = 0
  for i in range(max(len(mgg_ttHLeptonic_workspace), len(mgg_ttHLeptonic_looper))):
    value_workspace = str(format(mgg_ttHLeptonic_workspace[idx_workspace], '.6f')) if idx_workspace < len(mgg_ttHLeptonic_workspace) else "          "
    value_looper = str(format(mgg_ttHLeptonic_looper[idx_looper], '.6f')) if idx_looper < len(mgg_ttHLeptonic_looper) else "          "

    if value_workspace == value_looper:
      fout.write("%s  %s\n" % (value_workspace, value_looper))
    elif value_workspace == "          " or value_looper == "          ":
      fout.write("%s  %s\n" % (value_workspace, value_looper))
    else:
      if float(value_workspace) < float(value_looper):
        fout.write("%s  %s\n" % (value_workspace, "          "))
        idx_looper -= 1
      else:
        fout.write("%s  %s\n" % ("          ", value_looper))
        idx_workspace -= 1
    idx_workspace += 1
    idx_looper += 1

