all: resolution_app.o resolution_prover.o
	g++ -o resolution resolution_app.o resolution_prover.o

resolution_app.o: resolution_app.cpp resolution_prover.h
	g++ -c resolution_app.cpp

resolution_prover.o: resolution_prover.cpp resolution_prover.h
	g++ -c resolution_prover.cpp

clean:
	rm -f *.o resolution
