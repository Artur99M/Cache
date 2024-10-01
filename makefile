
all: LFU src/LFU.cpp include/LFU.hpp

test: all
	bash test/test.sh

LFU: src/LFU.cpp include/LFU.hpp
	g++ src/LFU.cpp -std=c++17 -o LFU

clear:
	rm LFU
