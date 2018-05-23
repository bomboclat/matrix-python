import time
import matrixC
import sys

# mat1 = (
#     (2, 1, 4),
#     (0, 1, 1)
# )

# mat2 = (
#    (6, 3, -1, 0),
#    (1, 1, 0, 4),
#    (-2, 5, 0, 2)
# )

start_time = time.time()
mat1 = matrixC.generate(10000, 1000)
elapsed_time = time.time() - start_time
print "matrix 1 generated in: " + str(elapsed_time) + " sec"

start_time = time.time()
mat2 = matrixC.generate(1000, 10000)
elapsed_time = time.time() - start_time
print "matrix 2 generated in: " + str(elapsed_time) + " sec"

print "\nstart calculating"
start_time = time.time()
matrixC.calc(mat1, mat2)
elapsed_time = time.time() - start_time
print "calculated in: " + str(elapsed_time) + " sec"

