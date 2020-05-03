all: sat_app.o SAT.o
	g++ -o sat sat_app.o SAT.o

sat_app.o: sat_app.cpp SAT.h
	g++ -c sat_app.cpp

SAT.o: SAT.cpp SAT.h
	g++ -c SAT.cpp

clean:
	rm -f *.o sat
