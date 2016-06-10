from distutils.core import setup, Extension
import numpy.distutils.misc_util
import os

if "PYROVER_CFLAGS" in os.environ:
    native_cflags = os.environ["PYROVER_CFLAGS"].split(" ")
else:
    native_cflags = ["-std=c++1y"]

print "[Rover] Building native extensions using " + str(native_cflags)

setup(
    ext_modules=[Extension("_rover", ["py_module.cc"], extra_compile_args=native_cflags)],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs()
)
