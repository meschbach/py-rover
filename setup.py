from distutils.core import setup, Extension
import numpy.distutils.misc_util

setup(
    ext_modules=[Extension("_rover", ["py_module.cc"], extra_compile_args=["-std=c++1y"])],
    include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs()
)
