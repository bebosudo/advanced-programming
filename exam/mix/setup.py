from distutils.core import setup
from Cython.Build import cythonize

setup(
    name="bst_cpp",
    ext_modules=cythonize('*.pyx'),
)
