import imp
import sys
try:
  imp.find_module('os')
  sys.exit(0)
except:
  sys.exit(1)
