all: profit

profit: main.cpp
	g++ -g -o profit main.cpp
valgrind:
	valgrind --tool=callgrind ./profit
	kcachegrind callgrind.out.*
.PHONY: clean
clean:
	rm profit
	rm callgrind.out.*
