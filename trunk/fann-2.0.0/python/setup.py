#!/usr/bin/python

from distutils.core import setup, Extension
import glob
#from compiler.pycodegen import compileFile
#import distutils
#import distutils.sysconfig
#import distutils.core
import os

NAME='pyfann'
VERSION='1.2.0'

LONG_DESCRIPTION="""\
Fast Artificial Neural Network Library implements multilayer
artificial neural networks with support for both fully connected
and sparsely connected networks. It includes a framework for easy 
handling of training data sets. It is easy to use, versatile, well 
documented, and fast. 
"""

#These lines are needed to circumvent a bug in distutils
swig_cmd = 'swig -python pyfann/pyfann.i'
#print 'running SWIG:', swig_cmd
os.system(swig_cmd)

#This utility function searches for files
def hunt_files(root, which):
    return glob.glob(os.path.join(root, which))

setup(
    name=NAME,
    description='Fast Artificial Neural Network Library (fann)',
    long_description=LONG_DESCRIPTION,
    version=VERSION,
    author='Steffen Nissen',
    author_email='lukesky@diku.dk',
    maintainer='Gil Megidish & Vincenzo Di Massa',
    maintainer_email='gil@megidish.net & hawk.it@tiscali,it',
    url='http://sourceforge.net/projects/fann/',
    license='GNU LESSER GENERAL PUBLIC LICENSE (LGPL)',
    py_modules=['pyfann.libfann','pyfann.fann'],
    ext_modules=[Extension('pyfann._libfann',['pyfann/pyfann.i','pyfann/fann_helper.c'], include_dirs=['../src/include'], extra_objects=['../src/doublefann.o']) ]
)

