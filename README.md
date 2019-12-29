
### Fast-KNN
---

A fast K-nearest-neighbour implementation

Installing

```bash
make fast_knn_py
python3 setup.py install
``` 


```python3
knn = fast_knn.KNN(C=3, verbose=True)
mat = np.random.rand(1000, 4)
knn.fit(mat)

pred = np.random.rand(10, 4)
print(knn.predict_knn(pred, 2))
print()
print(knn.predict_cent(pred))
```


