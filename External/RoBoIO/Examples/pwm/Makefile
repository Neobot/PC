pwm.exe : pwm.o
	gcc -o pwm.exe pwm.o -L../../Lib -lRBIO -lstdc++
pwm.o : pwm.cpp
	gcc -c pwm.cpp -I../../Include

.PHONY : clean
clean :
	-rm pwm.o pwm.exe
