#!/usr/bin/env python

from distutils.core import setup
from distutils.extension import Extension
import os

#os.environ["CC"] = "/usr/bin/c++"
#os.environ["CXX"] = "/usr/bin/c++"

params = dict(libraries = ["boost_python", "mysqlcppconn"],
	 		  extra_compile_args=['-std=c++14'],
     		  include_dirs = ['/usr/local/include', '../src/'])

setup(name="rsys",
    ext_modules=[
        Extension("rsys", ["./rsys.py.cc",
                           "../src/rsys/data_sources/mexception.cc",
                           "../src/rsys/util/util.cc",
                           "../src/rsys/db_conf/mysql_config.cc",
                           "../src/rsys/exporters/mysql_exporter.cc",
                           "../src/rsys/exporters/svd_mysql_exporter.cc",
                           ], **params)
    ])
