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

def insert_line(i, line, lines):
  print "Adding line: [%d] %s" % (i, line.strip())
  lines.insert(i, line)

def modify_line(i, old_line, new_line, lines):
  print "Modifying line: [%d] from \n%s\n to \n%s" % (i, old_line.strip(), new_line.strip())
  lines[i] = new_line

def remove_line(i, lines):
  print "Removing line: [%d] %s" % (i, lines[i].strip())
  lines.pop(i)

def modify_header(lines, add):
  print "Modifying header file"
  if add: # add this variable
    for i, line in enumerate(lines):
      if "// Variable names" in line:
	index = lines[i+1].find("{") + 1
	new_line = lines[i+1][:index] + '"' + args.name + '", ' + lines[i+1][index:]
	modify_line(i+1, lines[i+1], new_line, lines)
      if "// Variable declarations" in line:
	line_to_insert = "    %s           %s;\n" % (args.type, args.name)
	insert_line(i+1, line_to_insert, lines)
      if "// Variable branches" in line:
	line_to_insert = '  BabyTree_->Branch("%s" ,&%s);\n' % (args.name, args.name)
	insert_line(i+1, line_to_insert, lines)
  else: # remove this variable
    for i, line in enumerate(lines):
      if "// Variable names" in line:
	new_line = lines[i+1].replace('"%s", ' % args.name, '')
	modify_line(i+1, lines[i+1], new_line, lines)
      if args.type in line and args.name + ";" in line:
	remove_line(i, lines)
      if 'BabyTree_->Branch("%s"' % args.name in line:
	remove_line(i, lines)
  print "\n\n\n"
  return lines

def modify_src(lines, add):
  print "Modifying src file"
  if add:
    for i, line in enumerate(lines):
      if "// Variable definitions" in line:
	line_to_insert = "      %s = %s();\n" % (args.name, args.function)
	insert_line(i+1, line_to_insert, lines)
  else:
    for i, line in enumerate(lines):
      if "%s = %s();" % (args.name, args.function) in line:
	remove_line(i, lines)
  print "\n\n\n"
  return lines

def modify_scanchain(lines, add):
  print "Modifying scanchain"
  if add:
    for i, line in enumerate(lines):
      if "// Declare BDT vars" in line:
	line_to_insert = '  %s %s;\n' % (args.type, args.name)
	insert_line(i+1, line_to_insert, lines)
      if 'mva.reset(new TMVA::Reader( "!Color:Silent" ));' in line:
	line_to_insert = '    mva->AddVariable("%s", &%s);\n' % (args.name, args.name)
	insert_line(i+1, line_to_insert, lines)
      if "// Calculate MVA value" in line:
	line_to_insert = '        %s = %s();\n' % (args.name, args.function)
	insert_line(i+1, line_to_insert, lines)
  else:
    for i, line in enumerate(lines):
      if "%s %s;" % (args.type, args.name) in line:
	remove_line(i, lines)
      if 'mva->AddVariable("%s", &%s);' % (args.name, args.name) in line:
	remove_line(i, lines)
      if '%s = %s();' % (args.name, args.function) in line:
	remove_line(i, lines)
  print "\n\n\n"
  return lines


### Modify files ###

lines = read_file(mva_babymaker_h) 
lines = modify_header(lines, not args.remove)
write_file("test.h", lines)

lines = read_file(mva_babymaker_C) 
lines = modify_src(lines, not args.remove)

write_file("test.C", lines)

lines = read_file(scanchain)
lines = modify_scanchain(lines, not args.remove)

write_file("test_scanchain.C", lines)
