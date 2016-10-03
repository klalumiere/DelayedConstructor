#!/usr/bin/env python

import os
import subprocess

fullProjectDir = os.path.dirname(os.path.realpath(__file__))

archiveName = os.environ['ARCHIVE_NAME_CMAKE']
outputCmake = "/tmp/cmake.tar.gz";
subprocess.call(["wget","--no-check-certificate",
	"https://cmake.org/files/v3.6/"+archiveName+".tar.gz","-O",outputCmake])
subprocess.call(["tar","-xvf",outputCmake])
f = open("cmakeEnv.sh", 'w')
f.write("export PATH="+fullProjectDir+"/"+archiveName+"/bin/:$PATH")
f.close()
