import ctypes
import time
import numpy as np
import typing
import os

Matrix = typing.List[typing.List[float]]


def _int_pointer_to_np(pointer: int, size: int):
    arr = ctypes.cast(pointer, ctypes.POINTER(ctypes.c_int))
    res = np.array([arr[i] for i in range(size)])
    return res


class KNN:

    def __init__(self, C=3, verbose=True):
        self.lib = ctypes.cdll.LoadLibrary(f"{os.path.dirname(os.path.abspath(__file__))}/fast_knn.so")

        self.verbose = verbose
        self.knn = self.lib.init_knn(C)

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

    def fit(self, matrix: Matrix, max_iter: int = 10):
        """Fit the KNN model"""
        mat = self._get_mat(matrix)

        timestamp = time.time()
        self.lib.fit_knn(self.knn, mat, max_iter)

        if self.verbose:
            print("Time to fit", time.time() - timestamp)

    def predict_knn(self, matrix: Matrix, k: int) -> np.ndarray:
        """Make knn prediction and return labels with cluster belongance"""
        mat = self._get_mat(matrix)
        pred = self.lib.predict_knn(self.knn, mat, k)
        return _int_pointer_to_np(pred, len(matrix))

    def predict_cent(self, matrix: Matrix) -> np.ndarray:
        mat = self._get_mat(matrix)
        pred = self.lib.predict_cent(self.knn, mat)
        return _int_pointer_to_np(pred, len(matrix))
