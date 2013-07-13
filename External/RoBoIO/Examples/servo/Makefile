servo.exe : servo.o
	gcc -o servo.exe servo.o -L../../Lib -lRBIO -lstdc++
servo.o : servo.cpp
	gcc -c servo.cpp -I../../Include

.PHONY : clean
clean :
	-rm servo.o servo.exe
