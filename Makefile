
CXX = clang++
FLAGS = -Wall -g

test: build/test.o build/knn.o build/distance.o build/mat.o build/vec.o build/preds.o 
	$(CXX) $(FLAGS) -o bin/test $?
	./bin/test

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


