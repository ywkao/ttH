import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("name", help = "name of variable to add/remove from BDT", type=str)
parser.add_argument("type", help = "variable type (e.g. 'double', 'int')", type=str)
parser.add_argument("function", help = "function that defines the variable", type=str)
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("--remove", help = "Remove this variable instead of adding it", action="store_true")
args = parser.parse_args()

mva_babymaker_h = "MakeMVABabies_ttH" + args.channel + ".h"
mva_babymaker_C = "MakeMVABabies_ttH" + args.channel + ".C"
scanchain = "ScanChain_ttH" + args.channel + ".C"

def read_file(file):
  with open(file, 'r') as f_in:
    return f_in.readlines()

def write_file(file, lines):
  with open(file, 'w') as f_out:
    for line in lines:
      f_out.write(line)

def modify_header(lines, add):
  print "Modifying header file"
  if add: # add this variable
    for i, line in enumerate(lines):
      if "// Variable names" in line:
	index = lines[i+1].find("{") + 1
	print i, "Modifying line from \n%s\n to \n%s" % (lines[i+1], lines[i+1][:index] + '"' + args.name + '", ' + lines[i+1][index:])
	lines[i+1] = lines[i+1][:index] + '"' + args.name + '", ' + lines[i+1][index:]
      if "// Variable declarations" in line:
	#print i+1, "Adding line %s" % 
	lines.insert(i+1, "    %s           %s;\n" % (args.type, args.name))
      if "// Variable branches" in line:
	#print i, "Adding line %s" % line
	lines.insert(i+1, '  BabyTree_->Branch("%s" ,&%s);\n' % (args.name, args.name))
  else: # remove this variable
    for i, line in enumerate(lines):
      if "// Variable names" in line:
	print i,  "Modifying line from \n%s\n to \n%s" % (lines[i+1], lines[i+1].replace('"%s", ' % args.name, ''))
        lines[i+1] = lines[i+1].replace('"%s", ' % args.name, '')
      if args.type in line and args.name + ";" in line:
	print i,  "Removing line %s " % line
        lines.pop(i)
      if 'BabyTree_->Branch("%s"' % args.name in line:
	print i,  "Removing line %s " % line
        lines.pop(i)
  print "\n\n\n"
  return lines

def modify_src(lines, add):
  print "Modifying src file"
  if add:
    for i, line in enumerate(lines):
      if "// Variable definitions" in line:
	#print i,  "Adding line %s" % line
        lines.insert(i+1, "      %s = %s();\n" % (args.name, args.function))
  else:
    for i, line in enumerate(lines):
      if "%s = %s();" % (args.name, args.function) in line:
	print i,  "Removing line %s " % line
        lines.pop(i)
  print "\n\n\n"
  return lines

def modify_scanchain(lines, add):
  print "Modifying scanchain"
  if add:
    for i, line in enumerate(lines):
      if "// Declare BDT vars" in line:
        #print i,  "Adding line %s" % line
	lines.insert(i+1, '  %s %s;\n' % (args.type, args.name))
      if 'mva.reset(new TMVA::Reader( "!Color:Silent" ));' in line:
	#print i,  "Adding line %s" % line
        lines.insert(i+1, '    mva->AddVariable("%s", &%s);\n' % (args.name, args.name))
      if "// Calculate MVA value" in line:
	#print i,  "Adding line %s" % line
        lines.insert(i+1, '        %s = %s();\n' % (args.name, args.function))
  else:
    for i, line in enumerate(lines):
      if "%s %s;" % (args.type, args.name) in line:
        print i,  "Removing line %s " % line
        lines.pop(i)
      if 'mva->AddVariable("%s", &%s);' % (args.name, args.name) in line:
        print i,  "Removing line %s " % line
        lines.pop(i)
      if '%s = %s();' % (args.name, args.function) in line:
        print i,  "Removing line %s " % line
        lines.pop(i)
  print "\n\n\n"
  return lines

# Modify header
lines = read_file(mva_babymaker_h) 
lines = modify_header(lines, not args.remove)
write_file("test.h", lines)

lines = read_file(mva_babymaker_C) 
lines = modify_src(lines, not args.remove)
#for i, line in enumerate(lines):
#  if "// Variable definitions" in line:
#    lines.insert(i+1, "      %s = %s();\n" % (args.name, args.function))

write_file("test.C", lines)

lines = read_file(scanchain)
lines = modify_scanchain(lines, not args.remove)
#for i, line in enumerate(lines):
#  if "// Declare BDT vars" in line:
#    lines.insert(i+1, '  %s %s;\n' % (args.type, args.name))

write_file("test_scanchain.C", lines)
