import ctypes
import time
import numpy as np
import typing
import os
import sys

Matrix = typing.List[typing.List[float]]


def _int_pointer_to_np(pointer: int, size: int):
    arr = ctypes.cast(pointer, ctypes.POINTER(ctypes.c_int))
    res = np.array([arr[i] for i in range(size)])
    return res


class KMN:

    def __init__(self, C=3, verbose=True):
        self.lib = ctypes.cdll.LoadLibrary(f"{os.path.dirname(os.path.abspath(__file__))}/fast_kmn.so")

        self.verbose = verbose
        self.kmn = self.lib.init_kmn(C)

        self.lib.init_mat.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(
            ctypes.POINTER(
                ctypes.c_double
            )
        )]

    def _numpy_to_mat(self, matrix: np.ndarray) -> int:
        """Convert a numpy matrix to mat and return its pointer"""
        if matrix.ndim != 2:
            raise Exception(f"Matrix should have 2 dimensions, {matrix.ndim} != 2")

        rows, columns = matrix.shape

        timestamp = time.time()
        double_mat = (ctypes.POINTER(ctypes.c_double) * rows)()
        for i, row in enumerate(matrix):
            double_mat[i] = (ctypes.c_double * columns)(*row)

        if self.verbose:
            print("Time to construct ctype matrix", time.time() - timestamp)

        timestamp = time.time()
        mat = self.lib.init_mat(rows, columns, double_mat)

        if self.verbose:
            print("Time to construct mat", time.time() - timestamp)

        return mat

    def _get_mat(self, matrix: Matrix):
        mat = None
        if type(matrix) is np.ndarray:
            mat = self._numpy_to_mat(matrix)

        if mat is None:
            raise Exception(f"Unsupported type {type(matrix)}")

        return mat

    def fit_kmeans(self, matrix: Matrix, max_iter: int = 10) -> np.ndarray:
        """Fit the KMN model"""
        mat = self._get_mat(matrix)

        timestamp = time.time()
        labels = self.lib.kmeans(self.kmn, mat, max_iter)

        if self.verbose:
            print("Time to fit", time.time() - timestamp)

        return _int_pointer_to_np(labels, len(matrix))

    def fit_knn(self, matrix: Matrix, labels: np.ndarray):
        mat = self._get_mat(matrix)

        int_pointer = ctypes.cast((ctypes.c_int * len(labels))(*labels), ctypes.POINTER(ctypes.c_int))

        timestamp = time.time()
        self.lib.kneigh(self.kmn, mat, int_pointer)

        if self.verbose:
            print("Time to fit", time.time() - timestamp)

        return self

    def predict_knn(self, matrix: Matrix, k: int) -> np.ndarray:
        """Make kmn prediction and return labels with cluster belongance"""
        mat = self._get_mat(matrix)
        pred = self.lib.predict_knn(self.kmn, mat, k)
        return _int_pointer_to_np(pred, len(matrix))

    def predict_cent(self, matrix: Matrix) -> np.ndarray:
        mat = self._get_mat(matrix)
        pred = self.lib.predict_cent(self.kmn, mat)
        return _int_pointer_to_np(pred, len(matrix))
