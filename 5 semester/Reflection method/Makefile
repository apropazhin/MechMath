all: main.o getopt.o MyException.o matrix.o
	g++ -o run main.o getopt.o MyException.o matrix.o -O3

main.o: main.cpp matrix.h
	g++ -c main.cpp -o main.o -O3
	
MyException.o: MyException.cpp MyException.h
	g++ -c MyException.cpp -o MyException.o -O3

matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp -o matrix.o -O3
	
getopt.o: getopt.cpp getopt.h
	g++ -c getopt.cpp -o getopt.o -O3

clean:
	rm -f *.o run
