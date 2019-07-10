import sys, os
import time

n_trainings = 1000
for i in range(n_trainings):
  start = time.time()
  os.system("python run_bdt_workflow.py Leptonic ttHLeptonicLoose All rand_baseline_%d --randomize" % i)
  end = time.time()
  elapsed_time = end - start
  print("Training no. %d done, took %.2f seconds" % (i+1, elapsed_time))
