import os

# check for files smaller than x bytes
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("tag", help = "job tag e.g. 'v7'", type=str)
args = parser.parse_args()


os.system("du -a /hadoop/cms/store/user/smay/ttH/*" + args.tag + "* | awk '$1*512 < 2000 {print $2} ' > bad_files.txt")

with open ("bad_files.txt") as fin:
  for line in fin:
    print(("Removing file %s" % line))
    os.system("rm %s" % line)

