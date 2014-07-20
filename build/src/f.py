#!/usr/bin/python
import pipes, os, sys

rfPath = "/tmp/satObPIPE"
#os.mkfifo(rfPath)
rp = open(rfPath, 'r')
var = 1
while var == 1:
	response = rp.read(10)
	print "P2 hear %s" % response
