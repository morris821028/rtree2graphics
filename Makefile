CXX=g++
CFLAGS=-std=c++11 -O2

INPUT=./test/2.in
OUTPUT=2.tex

all: pin rtree rtree2graphic

rtree: ./test/rtree.cpp
	$(CXX) $(CFLAGS) $^ -o $@
	
pin: ./test/pin.cpp
	$(CXX) $(CFLAGS) $^ -o $@

rtree2graphic: ./src/rtree2graphic.cpp
	$(CXX) $(CFLAGS) $^ -o $@

test:
	./rtree <$(INPUT) | ./rtree2graphic >$(OUTPUT)
	pdflatex $(OUTPUT)

clean:
	-rm pin rtree rtree2graphic
	-rm *.log *.aux *.pdf *.tex *.out

