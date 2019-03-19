import ROOT as r

def get_normalization():
  f = r.TFile.Open("../MVABaby_ttHHadronic_.root")
  tree = f.Get("t")

  tree.Draw("evt_weight_", "process_id_ == 3")
  hist = r.gPad.GetPrimitive("htemp")
  mean_pythia = hist.GetMean() 
  entries_pythia = hist.GetEntries() 

  tree.Draw("evt_weight_", "process_id_ == 17")
  hist = r.gPad.GetPrimitive("htemp")
  mean_madgraph = hist.GetMean()
  entries_madgraph = hist.GetEntries() 

  return (mean_madgraph * entries_madgraph) / (mean_pythia * entries_pythia)

def read_file(file):
  with open(file, 'r') as f_in:
    return f_in.readlines()

def write_file(file, lines):
  with open(file, 'w') as f_out:
    for line in lines:
      f_out.write(line)

normalization = get_normalization()
print normalization

lines = read_file("../ttHLooper.h")
for i in range(len(lines)):
  if "const double gjet_normalization" in lines[i]:
    lines[i] = "const double gjet_normalization = %.6f;\n" % (normalization)
write_file("../ttHLooper.h", lines)
