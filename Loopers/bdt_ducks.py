import sys, os

from multiprocessing import Process

def run(command):
  print "Command is ", command
  os.system(command)
  return

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("selection", help = "e.g. ttHLeptonicLoose", type=str)
parser.add_argument("year", help = "e.g. 2016 or 2017", type=str)
parser.add_argument("tag", help = "tag to identify this training of the BDT", type=str)
parser.add_argument("n_trainings", help = "number of trainings to run", type=int)
args = parser.parse_args()

# Use maximum niceness like the good citizens we are 
nice = 19
os.nice(nice)

# Maximum number of threads to launch in parallel (in principle, can be different from n_trainings)
n_par = 10

running_procs = []

for i in range(args.n_trainings):
  command = "python run_bdt_workflow.py %s %s %s %s %s --randomize" % (args.channel, args.selection, args.year, str(i+1), args.tag)
  running_procs.append(Process(target=run, args=(command,)))
  running_procs[-1].start()
  while True:
    for i in range(len(running_procs)):
      if not running_procs[i].is_alive():
        running_procs.pop(i)
        break
    if len(running_procs) < n_par:
      break
    else:
      os.system("sleep 5s") 
