EXE = Convolution
GCC = g++ -fopenmp

$(EXE): Convolution.o
	$(GCC) $^ -o $(EXE)
Convolution.o: Convolution.c
	$(GCC) -c $< -o $@
	
clean:
	rm *.o $(EXE)