import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input .hdf5 file", type=str)
parser.add_argument("--tag", help = "tag to identify this optimization", type=str)
parser.add_argument("--n_points", help = "number of points to probe", type=str)
parser.add_argument("--random", help = "do random exploration instead of BayesOpt", action = "store_true")
parser.add_argument("--fixed", help = "sample a fixed pbound set", action = "store_true")
args = parser.parse_args()

template = "optimize_dnn_hyperparams"
channel = "Hadronic" if "Hadronic" in args.input else "Leptonic"

os.system("cp %s %s" % (args.input, "~/public_html/ttH/nautilus/"))
os.system("chmod 755 %s" % ("~/public_html/ttH/nautilus/" + args.input.split("/")[-1]))

lines = []
with open(template + ".yaml", "r") as f_in:
    lines = f_in.readlines()

for i in range(len(lines)):
    if "INPUT" in lines[i]:
        lines[i] = lines[i].replace("INPUT", args.input.split("/")[-1])
    if "TAG" in lines[i]:
        lines[i] = lines[i].replace("TAG", args.tag)
    if "CHANNEL" in lines[i]:
        lines[i] = lines[i].replace("CHANNEL", channel)
    if "NPOINTS" in lines[i]:
        lines[i] = lines[i].replace("CHANNEL", int(args.n_points)/3)
    if "RANDOM" in lines[i]:
        if not args.random:
            lines[i] = lines[i].replace(" RANDOM", "")
    if "FIXED" in lines[i]:
        if not args.fixed:
            lines[i] = lines[i].replace(" FIXED", "")

submit_script = "submit_scripts/" + template + "_" + args.tag + ".yaml"
with open(submit_script, "w") as f_out:
    for line in lines:
        f_out.write(line)

os.system("kubectl create -f %s" % submit_script) 
