EXE = 2dFFT
GCC = g++ -fopenmp

$(EXE): 2dFFT.o tools.o
	$(GCC) $^ -o $(EXE)
2dFFT.o: 2dFFT.c tools.h
	$(GCC) -c $< -o $@
tools.o: tools.c tools.h
	$(GCC) -c $< -o $@
clean:
	rm *.o $(EXE)