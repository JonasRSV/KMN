import kmn
import numpy as np


def test_all():
    clf = kmn.KMN(C=3, verbose=False)
    mat = np.random.rand(10, 4)
    print("Kmeans fit", clf.fit_kmeans(mat))

    print("Kmeans pred", clf.predict_knn(mat, 2))
    print("Kmeans pred", clf.predict_cent(mat))
    pred = np.random.rand(10, 4)
    print(clf.predict_knn(pred, 2))
    print()
    print(clf.predict_cent(pred))

    labels = np.random.randint(0, 4, size=10)
    print("labels", labels)

    print("\n\n\n ------------- KNN ------------------")
    clf2 = kmn.KMN(verbose=False)
    clf2.fit_knn(mat, labels)

    print("Kmeans pred", clf2.predict_knn(mat, 1))
    print("Kmeans pred", clf2.predict_cent(mat))
    pred = np.random.rand(10, 4)
    print(clf2.predict_knn(pred, 2))
    print()
    print(clf2.predict_cent(pred))

if __name__ == "__main__":
    test_all()
