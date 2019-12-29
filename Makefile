
CXX = clang++
FLAGS = -Wall -g
PYFLAG = -shared -fPIC -Wall -undefined dynamic_lookup

fast_knn_py: lib/python.cpp lib/knn.cpp lib/distance.cpp lib/mat.cpp lib/vec.cpp lib/preds.cpp
	$(CXX) $(PYFLAG) -o py/fast_knn.so $?

test: build/test.o build/knn.o build/distance.o build/mat.o build/vec.o build/preds.o 
	$(CXX) $(FLAGS) -o bin/test $?
	./bin/test

build/python.o: lib/python.cpp 
	$(CXX) $(FLAGS) -c lib/python.cpp -o build/python.o

build/test.o: lib/test.cpp
	$(CXX) $(FLAGS) -c lib/test.cpp -o build/test.o

build/knn.o: lib/knn.cpp
	$(CXX) $(FLAGS) -c lib/knn.cpp -o build/knn.o

build/distance.o: lib/distance.cpp
	$(CXX) $(FLAGS) -c lib/distance.cpp -o build/distance.o

build/mat.o: lib/mat.cpp
	$(CXX) $(FLAGS) -c lib/mat.cpp -o build/mat.o

build/vec.o: lib/vec.cpp
	$(CXX) $(FLAGS) -c lib/vec.cpp -o build/vec.o

build/preds.o: lib/preds.cpp
	$(CXX) $(FLAGS) -c lib/preds.cpp -o build/preds.o


