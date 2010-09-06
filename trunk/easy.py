#!/usr/bin/env python

import sys
import os

if len(sys.argv) <= 1:
	print 'Usage: %s training_file [testing_file]' % sys.argv[0]
	raise SystemExit

svmtrain_exe = sys.path[0]+'"/home/papa/src/libmachinelearning/svm-train'
svmpredict_exe = sys.path[0]+'/home/papa/src/libmachinelearning/svm-predict'
grid_py = sys.path[0]+'/home/papa/src/libmachinelearning/grid.py'

assert os.path.exists(svmtrain_exe),"svm-train executable not found"
assert os.path.exists(svmpredict_exe),"svm-predict executable not found"
assert os.path.exists(grid_py),"grid.py not found"

train_pathname = sys.argv[1]
kernel_type = sys.argv[2]

assert os.path.exists(train_pathname),"training file not found"
file_name = os.path.split(train_pathname)[1]
model_file = file_name + ".model"


cmd = "%s -svmtrain %s -t %s %s" % (grid_py, svmtrain_exe, kernel_type, train_pathname)
print 'Cross validation...'
dummy, f = os.popen2(cmd)

line = ''
while 1:
	last_line = line
	line = f.readline()
	if not line: break
c,g,rate = map(float,last_line.split())

print 'Best c=%s, g=%s CV rate=%s' % (c,g,rate)

cmd = "%s -c %s -t %s -g %s %s %s" % (svmtrain_exe,c,kernel_type,g,train_pathname,model_file)
print 'SVM Training...'
os.popen(cmd)
