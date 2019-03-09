default, all, measure:
	g++ --std=c++17 -Wall Example.cpp -O3 -o Example

clean:
	$(RM) -rf Build