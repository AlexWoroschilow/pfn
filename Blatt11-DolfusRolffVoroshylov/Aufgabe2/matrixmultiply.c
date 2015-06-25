#include <stdlib.h>
#include <stdio.h>
#include <Python.h>

#define STEP_SIZE 32
#define min(x,y) x < y ? x : y
#define max(x,y) x > y ? x : y

/* Fast cache friendly nxn matrix multiplication, for details see:
   http://suif.stanford.edu/papers/lam-asplos91.pdf
*/
static double* blockMultiplication(const double* A,
    const double* B,
    const unsigned long n) {
    unsigned long kk, jj, i, j, k;
    double* C = calloc(n * n, sizeof(*C));
    if (!C)
        return NULL;

    for (kk = 0; kk < n; kk += STEP_SIZE) {
        for (jj = 0; jj < n; jj += STEP_SIZE) {
            for (i = 0; i < n; ++i) {
                const unsigned long mink = min(kk + STEP_SIZE, n);
                for (k = kk; k < mink; ++k) {
                    const unsigned long minj = min(jj + STEP_SIZE, n);
                    for (j = jj; j < minj; ++j) {
                        C[i * n + k] += A[i * n + j] * B[j * n + k];
                    }
                }
            }
        }
    }
    return C;
}

/* Simple matrix multiplication for different size matrices
*/
static double* simpleMultiplication(const double* A,
                             const double* B,
                             const unsigned long l,
                             const unsigned long m,
                             const unsigned long n) {
  unsigned long i, j, k;
  double* C =  calloc(l * n, sizeof(*C));
  if (!C) {
    /* if an error occured inform python */
    PyErr_SetString(PyExc_Exception,
                    "Could not allocate memory for a new matrix\n");
    return NULL;
  }

  /** Matrix multiplication */
  for (i = 0; i < l; ++i) {
    for (k = 0; k < n; ++k) {
      for (j = 0; j < m; ++j) {
        C[i * n + k] += A[i * m + j] * B[j * n + k];
      }
    }
  }
  return C;
}


double* matrixMultiplication(const double* A,
                             const double* B,
                             const unsigned long l,
                             const unsigned long m,
                             const unsigned long n) {
  if (l == m && m == n) {
    return blockMultiplication(A, B, n);
  }
  return simpleMultiplication(A, B, l, m, n);
}