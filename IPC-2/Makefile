all: prime-calculator

prime-calculator: prime-calculator.o
	g++ prime-calculator.o -o prime-calculator 

prime-calculator.o: prime-calculator.cpp
	g++ prime-calculator.cpp -c

clean:
	rm *.o prime-calculator
