default, all, measure:
	g++ --std=c++17 -Wall Example.cpp -O3 -o Example -lpthread

clean:
	$(RM) -rf Build
