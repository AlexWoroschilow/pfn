from timeit import Timer
import random as rand
import numpy as np
import timeit
import ctypes
import sys
import re
import os

# Parse function name from an function pointer
def parseFunctionName(func):
  # Function str is something like <function functionname at 0x123ad131>
  m = re.match('\<\s*function\s+(\w*)\s+at\s+0x[0-9a-f]*\s*\>', str(func))
  if m:
    # return \w* (funtionname)
    return m.group(1)
  return ""

# Multiply matrices with ython
def pythonMultiply(A, B):
  if not A or not B:
    return None

  sizeA = len(A), len(A[0])
  sizeB = len(B), len(B[0])
  if sizeA[0] == 0 or sizeA[1] == 0 or sizeB[1] == 0:
    raise Exception("Es darf keine Spalten- / Zeilen- Breite " + 
                    "von 0 angegeben werden")
  
  if sizeA[1] != sizeB[0]:
    raise Exception("Die Zeilenbreite A muss der " +
                    "Spaltenbreite von B entsprechen")

  C = [[0 for y in range(0, sizeB[1])] for x in range(0, sizeA[0])]
  for i in range(0, sizeA[0]):
    for k in range(0, sizeB[1]):
      for j in range(0, sizeA[1]):
        C[i][k] += A[i][j] * B[j][k]
  return C

# Multiply matrices with numpy
def numpyMultiply(A, B):
  return np.matrix(A) * np.matrix(B)

# Multiply matrices with the custom c function
def cMultiply(A, B):
  sizeA = len(A), len(A[0])
  sizeB = len(B), len(B[0])
  
  if sizeA[0] == 0 or sizeA[1] == 0 or sizeB[1] == 0:
    raise Exception("Es darf keine Spalten- / Zeilen- Breite " + 
                    "von 0 angegeben werden")

  if sizeA[1] != sizeB[0]:
    raise Exception("Die Zeilenbreite A muss der " +
                    "Spaltenbreite von B entsprechen")

  # Search for library
  libPath = os.path.abspath(os.path.dirname(sys.argv[0])) + "/matrixmultiply.so"
  function = ctypes.CDLL(libPath).matrixMultiplication

  # Make A and B one dimensional
  Ad = [x for y in A for x in y]
  Bd = [x for y in B for x in y]

  # Call c function
  function.restype = ctypes.POINTER(ctypes.c_double * sizeA[0] * sizeB[1])
  res = function((ctypes.c_double * len(Ad)) (*Ad),
           (ctypes.c_double * len(Bd)) (*Bd),
            ctypes.c_long(sizeA[0]),
            ctypes.c_long(sizeA[1]),
            ctypes.c_long(sizeB[1]))

  # Build resulting two dimensional matrix
  return [x for i in range(0, sizeA[0]) for x in res[0][i]]

# Build new random matrix because we do a test we use A_ij = 1
def randomMatrix(size):
  return [[1 for i in range(0, size)] for j in range(0, size)]

# Run a given multiplication function for two matrices
def run(size, func):
  func(randomMatrix(size), randomMatrix(size))

if __name__ == '__main__':
  try:
    sizes = [25, 50, 100, 500, 1000, 2500, 5000]
    functions = [pythonMultiply, numpyMultiply, cMultiply]

    # Print Header
    header = "%12s |" % ("Matrix size")
    for func in functions:
      header += ("%15s in sec |" % (parseFunctionName(func)))
    print (header)
    print ("-" * len(header))
    
    # Print needed time in seconds
    for size in sizes:
      string = "{:>12.0f} |".format(size)
      for func in functions:
        # skip runs for pythonMultiply if size != 2500 and size != 5000
        if func == pythonMultiply and size > 1000:
          string += "{:>22.14f} |".format(0)
        else:
          # Create timer and measure needed time
          t = Timer(lambda: run(size, func))
          string += " {:>21.14f} |".format(t.timeit(number = 1))
      print(string)
  except Exception as e:
    print (e)