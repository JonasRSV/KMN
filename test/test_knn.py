import fast_knn
import numpy as np


def test_all():
    knn = fast_knn.KNN(C=3, verbose=True)
    mat = np.random.rand(1000, 4)
    knn.fit(mat)

    pred = np.random.rand(10, 4)
    print(knn.predict_knn(pred, 2))
    print()
    print(knn.predict_cent(pred))


if __name__ == "__main__":
    test_all()
