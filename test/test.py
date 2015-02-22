#!/bin/python

import subprocess
import re

defaults = { 'intflag': -100, 
             'uintflag': 101,
             'int64flag': -4294969295,
             'uint64flag': 4294969295,
             'doubleflag': 1.029300,
             'strflag': 'CFLAGS',
             'boolflag': 0 }

custom = { 'intflag': -201,
           'uintflag': 101,
           'int64flag': -4294979295,
           'uint64flag': 4294979295,
           'doubleflag': 2.875,
           'strflag': 'CUSTOMVALUE',
           'boolflag': 1 }

def test_values(checkvalues, cmdstring):
    output = subprocess.check_output(cmdstring)

    for line in output.splitlines():
        (var, val) = line.split()
        if (type(checkvalues[var]) is int):
            val = int(val)
        elif (type(checkvalues[var]) is float):
            val = float(val)
        elif (type(checkvalues[var]) is str):
            val = str(val)
            
        if (val != checkvalues[var]):
            print "Incorrect value for var",var,". Expected", checkvalues[var], "got", val,"."
            break

print "Testing default values..."
test_values(defaults, ["./cflags"]);

print "Testing custom values..."
test_values(custom, ["./cflags", "--intflag=-201", "--uintflag=101",
                     "--int64flag=-4294979295", "--uint64flag=4294979295",
                     "--doubleflag=2.875", "--strflag=CUSTOMVALUE",
                     "--boolflag"])

print "Testing help..."

output = subprocess.check_output(["./cflags", "--help"])

lines = output.splitlines()

for flag in defaults:
    for line in lines:
        regex = ".*--%s.*" % (flag)
        found = False
        m = re.search(regex, line)
        if (m != None):
            found = True
            break
    if (found == False):
        print "Flag %s is not in help\n" % (flag)


print "Testing for missing value..."    
output = subprocess.check_output(["./cflags", "--intflag"])

lines = output.splitlines()

if (lines[0] != 'Missing value for parameter intflag'):
    print "Expected missing value error"

print "Testing parameters..."
output = subprocess.check_output(["./cflags","one","two"])

lines = output.splitlines()

idx=0
for line in lines:
    m = re.search("^Parameters (.*)", line)
    if (m != None):
        numparams = int(m.group(1))
        break;
    idx = idx + 1

if (numparams != 2):
    print "Incorrect number of parameters"

(n, param) = lines[idx+1].split()

if ((int(n) != 1) or (param != "one")):
    print "Bad parameter %d.  Expected 'one' got %s" % (int(n), param)

(n, param) = lines[idx+2].split()

if ((int(n) != 2) or (param != "two")):
    print "Bad parameter %d.  Expected 'two' got %s" % (int(n), param)

# TODO: check parameter rearranging
# TODO: flag validation

print "Done"
