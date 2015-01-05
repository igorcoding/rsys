#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension

params = dict(libraries = ["boost_python"],
	 		  extra_compile_args=['-std=c++14'],
     		  include_dirs = ['.', '/usr/local/include'])

setup(name="rsys",
    ext_modules=[
        Extension("rsys", ["rsys.py.cc", "../src/math/mexception.cc"], **params)
        # Extension("rsys.math", ["./math/mexception.cc"], **params)
    ])
