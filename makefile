FLAGS = -O2 -W -Wall

all: LFU src/LFU.cpp include/LFU.hpp

test: all
	bash test/test.sh

LFU: src/LFU.cpp include/LFU.hpp
	g++ src/LFU.cpp -std=c++17 $(FLAGS) -o LFU

perfect:
	g++ src/perfect_cache.cpp -std=c++17 $(FLAGS) -o perfect_cache
	bash test/perfect_cache.sh

clear:
	rm LFU perfect_cache
