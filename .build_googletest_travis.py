#!/usr/bin/env python

import os
import subprocess

fullProjectDir = os.path.dirname(os.path.realpath(__file__))

buildName = "myBuild"
installName = "myInstall"
googletestDir = fullProjectDir + "/googletest"
buildDir = googletestDir + "/" + buildName
installDir = buildDir + "/" + installName
subprocess.call(["git","clone","https://github.com/google/googletest"])
subprocess.call(["git","checkout","release-1.8.0"], cwd = googletestDir)
subprocess.call(["mkdir",buildName], cwd = googletestDir)
subprocess.call(["cmake","-DCMAKE_INSTALL_PREFIX="+installDir,".."], cwd = buildDir)
subprocess.call(["make"], cwd = buildDir)
subprocess.call(["make","install"], cwd = buildDir)

f = open("googletestEnv.sh", 'w')
f.write("export PATH="+installDir+":$PATH\n")
f.write("export CMAKE_PREFIX_PATH="+installDir+":$CMAKE_PREFIX_PATH\n")
f.close()
