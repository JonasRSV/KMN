
CXX = clang++
FLAGS = -Wall

test: build/point.o build/test.o build/cluster.o build/knn.o
	echo $?
	$(CXX) $(FLAGS) -o bin/test $?
	./bin/test

build/test.o: lib/test.cpp
	$(CXX) $(FLAGS) -c lib/test.cpp -o build/test.o

build/cluster.o: lib/cluster.cpp
	$(CXX) $(FLAGS) -c lib/cluster.cpp -o build/cluster.o

build/knn.o: lib/knn.cpp
	$(CXX) $(FLAGS) -c lib/knn.cpp -o build/knn.o

build/point.o: lib/point.cpp
	$(CXX) $(FLAGS) -c lib/point.cpp -o build/point.o


