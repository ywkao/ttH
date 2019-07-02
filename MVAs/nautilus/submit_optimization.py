import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input .hdf5 file", type=str)
parser.add_argument("--tag", help = "tag to identify this optimization", type=str)
args = parser.parse_args()

template = "optimize_dnn_hyperparams"

os.system("cp %s %s" % (args.input, "~/public_html/ttH/nautilus/"))

lines = []
with open(template + ".yaml", "r") as f_in:
    lines = f_in.readlines()

for i in range(len(lines)):
    if "INPUT" in lines[i]:
        lines[i] = lines[i].replace("INPUT", args.input.split("/")[-1])
    if "TAG" in lines[i]:
        lines[i] = lines[i].replace("TAG", args.tag)

submit_script = "submit_scripts/" + template + "_" + args.tag + ".yaml"
with open(submit_script, "w") as f_out:
    for line in lines:
        f_out.write(line)

#os.system("kubectl create -f %s" % submit_script) 
