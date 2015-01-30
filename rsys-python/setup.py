#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
import os

params = dict(libraries = ["boost_python"],
	 		  extra_compile_args=['-std=c++14'],
     		  include_dirs = ['/usr/local/include', '../src/'])

setup(name="rsys",
    ext_modules=[
        Extension("rsys", ["./rsys.py.cc",
                           "../src/rsys/data_sources/mexception.cc",
                           "../src/rsys/util/util.cc"], **params)
    ])
