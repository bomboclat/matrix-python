import matrixC
import sys

mat1 = (
    (2, 1, 4),
    (0, 1, 1)
)

mat2 = (
    (6, 3, -1, 0),
    (1, 1, 0, 4),
    (-2, 5, 0, 2)
)
# matrix2.parse(mat1)
# matrix2.parse(mat2)
mat1 = matrixC.generate(1000, 1000)
mat2 = matrixC.generate(1000, 1000)
# print matrix2.generate(10, 6)
# matrix2.generate(10, 6)
# matrix2.generate(6, 6)
matrixC.calc(mat1, mat2)
# print matrix2.returnTuple()
# matrix2.generate(4, 3)
# ciao = matrix2.returnTuple()
# res = matrix2.create(3, 4)
# test = matrix2.returnTuple()

# for i in test:
#     print i
