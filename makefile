FLAGS = -O2 -W -Wall

all: LFU.out LFU/src/LFU.cpp LFU/include/LFU.hpp

test: all
	@ bash test/test.sh test/test1.dat
	@ bash test/test.sh test/test2.dat
	@ bash test/test.sh test/test3.dat

LFU.out: LFU/src/LFU.cpp LFU/include/LFU.hpp
	g++ LFU/src/LFU.cpp -std=c++17 $(FLAGS) -o LFU.out

perfect: perfect_cache
	@ echo "1:"
	@ bash test/perfect_cache.sh test/test1.dat
	@ echo "2:"
	@ bash test/perfect_cache.sh test/test2.dat
	@ echo "3:"
	@ bash test/perfect_cache.sh test/test3.dat

perfect_cache: PC/src/perfect_cache.cpp PC/include/perfect_cache.hpp
	g++ PC/src/perfect_cache.cpp -std=c++17 $(FLAGS) -o perfect_cache

clear:
	rm LFU perfect_cache
