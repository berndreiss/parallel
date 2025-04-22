CPP = g++ -std=c++11 -fopenmp
LIBS = -lm

default: main

main: main.cpp
	$(CPP) main.cpp -o main $(LIBS)

clean:
	rm -f *.o *.bin main

