
CXX = clang++
FLAGS = -Wall

build/knn.o: lib/knn.cpp
	$(CXX) $(FLAGS) -c lib/knn.cpp -o build/knn.o

knn: build/knn.o
	$(CXX) $(FLAGS) -o bin/knn build/knn.o


