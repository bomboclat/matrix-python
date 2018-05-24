from distutils.core import setup, Extension

module1 = Extension('matrixC', sources=['c_modules/matrix.c'],
        extra_compile_args=['-pipe', '-Wall', '-O3', '-fomit-frame-pointer',
                            '-march=native', '-mfpmath=sse', '-msse3', '-fopenmp', '-lm'],
        extra_link_args=['-lgomp'])

setup(name='matrixC',
        version='1.0',
        description='package for matrix calc',
        ext_modules=[module1])
