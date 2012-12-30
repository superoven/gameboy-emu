from subprocess import call
from os import listdir,devnull
from os.path import isfile, join

SUBDUEGCC = True

path = "./tests/dis/"
FNULL = open(devnull)

testfiles = [ f for f in listdir(path) if isfile(join(path,f)) ]
testfiles.sort()

if not testfiles:
  print "No test files present!"
  exit()

failed = 0
numfiles = len(testfiles)

print
print "Disassembler Test Suite"
print "-----------------------"

for f in testfiles:
  head = f.split(".")[0]
  print "%s:\t\t" % head,
  command = ["gcc", "-o", path + head, path + f]

  if SUBDUEGCC:
    notcompile = call(command, stdout = FNULL, stderr = FNULL)
  else:
    notcompile = call(command)

  if notcompile:
    print "FAIL (did not compile)"
    failed += 1
    continue
  
  returncode = call([path + head])
  if returncode:
    print "FAIL (return code %d)" % returncode
    failed += 1
    call(["rm", path + head], stdout = FNULL, stderr = FNULL)
    continue

  call(["rm", path + head], stdout = FNULL, stderr = FNULL)
  print "PASS"



print
if failed:
  print "Total Failed:\t", failed
  print "Percent Failed:\t%.2f%%" % ((float(failed)/float(numfiles))*100)
else:
  print "All Tests Pass!"
