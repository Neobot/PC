i2c_srf.exe : i2c_srf.o
	gcc -o i2c_srf.exe i2c_srf.o -L../../Lib -lRBIO -lstdc++
i2c_srf.o : i2c_srf.cpp
	gcc -c i2c_srf.cpp -I../../Include

.PHONY : clean
clean :
	-rm i2c_srf.o i2c_srf.exe
