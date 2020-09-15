all: bst-test

bst-test: bst.h avlbst.h bst-test.cpp print_bst.h
	g++ -g -Wall --std=c++11 bst-test.cpp -o bst-test

clean:
	rm -f *.o bst-test

*PHONY: clean 