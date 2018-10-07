CC = g++
FLAGS = -Wall -std=c++11 

build: frati ursi planificare numaratoare 

frati: frati.o
	$(CC) frati.o -o frati
	#g++ -Wall -std=c++11 frati.cpp -o frati

frati.o: frati.cpp
	$(CC) $(FLAGS) -c $< -o $@
	#g++ -Wall -std=c++11 -c frati.cpp -o frati.o

ursi: ursi.o
	$(CC) $(FLAGS) ursi.o -o ursi

ursi.o: ursi.cpp
	$(CC) $(FLAGS) -c $< -o $@

planificare: planificare.o
	$(CC) $(FLAGS) $< -o $@

planificare.o: planificare.cpp planificare.hpp
	$(CC) $(FLAGS) -c $< -o $@

numaratoare: numaratoare.o
	gcc -Wall $< -lm -o $@

numaratoare.o: numaratoare.c
	gcc -Wall -c $< -o $@

.PHONY: run-p1 run-p2 run-p3 run-p4 clean

run-p1: 
	./frati

run-p2:
	./ursi

run-p3:
	./planificare

run-p4:
	./numaratoare

clean:
	rm -f frati ursi planificare numaratoare *.o