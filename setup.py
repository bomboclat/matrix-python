from distutils.core import setup, Extension

# setup(name='getdict', version='1.0',  \
#    ext_modules=[Extension('getdict', ['c_modules/hello.c'], extra_compile_args=[
#        '-pipe', '-Wall', '-O3', '-fomit-frame-pointer',
#        '-march=native', '-mfpmath=sse', '-msse3', '-fopenmp', '-lm'],
#        extra_link_args=['-lgomp'])])

# setup(name='matrix', version='1.0',  \
#    ext_modules=[Extension('matrix', ['c_modules/matrix.c'], extra_compile_args=[
#        '-pipe', '-Wall', '-O3', '-fomit-frame-pointer',
#        '-march=native', '-mfpmath=sse', '-msse3', '-fopenmp', '-lm'],
#        extra_link_args=['-lgomp'])])

module1 = Extension('matrix2', sources=['c_modules/matrix.c'],
        extra_compile_args=['-pipe', '-Wall', '-O3', '-fomit-frame-pointer',
                            '-march=native', '-mfpmath=sse', '-msse3', '-fopenmp', '-lm'],
        extra_link_args=['-lgomp'])

setup(name='PackageName',
        version='1.0',
        description='This is a demo package',
        ext_modules=[module1])
