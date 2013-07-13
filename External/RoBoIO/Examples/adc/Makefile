adc.exe : adc.o
	gcc -o adc.exe adc.o -L../../Lib -lRBIO -lstdc++
adc.o : adc.cpp
	gcc -c adc.cpp -I../../Include

.PHONY : clean
clean :
	-rm adc.o adc.exe
