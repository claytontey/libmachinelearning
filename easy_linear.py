#!/usr/bin/env python

import sys
import os
print sys.path[0]+'oi'
if len(sys.argv) <= 1:
	print 'Usage: %s training_file [testing_file]' % sys.argv[0]
	raise SystemExit

svmtrain_exe = sys.path[0]+'/svm_linear-train'
svmpredict_exe = sys.path[0]+'/svm_linear-predict'
grid_py = sys.path[0]+'/grid_linear.py'

assert os.path.exists(svmtrain_exe),"svm-train executable not found"
assert os.path.exists(svmpredict_exe),"svm-predict executable not found"
assert os.path.exists(grid_py),"grid.py not found"

train_pathname = sys.argv[1]
assert os.path.exists(train_pathname),"training file not found"
file_name = os.path.split(train_pathname)[1]
model_file = file_name + ".model"

if len(sys.argv) > 2:
	test_pathname = sys.argv[2]
	file_name = os.path.split(test_pathname)[1]
	assert os.path.exists(test_pathname),"testing file not found"
	scaled_test_file = file_name + ".scale"
	predict_test_file = file_name + ".predict"

cmd = "%s -log2c -3,0,1 -log2c 1,1,1 -svmtrain %s  %s" % (grid_py, svmtrain_exe, train_pathname)
print 'Cross validation...'
dummy, f = os.popen2(cmd)

line = ''
while 1:
	last_line = line
	line = f.readline()
	if not line: break
c,g,rate = map(float,last_line.split())

print 'Best c=%s, CV rate=%s' % (c,rate)

cmd = "%s -c %s %s %s" % (svmtrain_exe,c,train_pathname,model_file)
print 'SVM Training...'
os.popen(cmd)
