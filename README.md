# matrix-python
Python plugin written in C for **matrix multiplication** and randomic generation.
It use **OpenMP** to parallelize the multiplication.

Installation
------------
To install and compile the library:
```console
$ git clone https://github.com/bomboclat/matrix-python.git
$ cd matrix-python
$ sudo python setup.py install
```

Examples
------
Generate two random matrices:
- First marix 50 x 100
- Second matrix 100 x 20

```python
import matrixC

mat1 = matrixC.generate(50, 100)
mat2 = matrixC.generate(100, 20)
```
> The returned type is a tuple

To multiply them:
```python
matrixC.matmul(mat1, mat2)
```
