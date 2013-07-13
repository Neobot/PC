gpio.exe : gpio.o
	gcc -o gpio.exe gpio.o -L../../Lib -lRBIO -lstdc++
gpio.o : gpio.cpp
	gcc -c gpio.cpp -I../../Include

.PHONY : clean
clean :
	-rm gpio.o gpio.exe
