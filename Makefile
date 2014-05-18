
vimtest: executable
	./executable

executable: cfacil.o simplepixels.o
	g++ `pkg-config --libs sdl` `pkg-config --libs gl` -o executable -lm -Wl,--wrap,main cfacil.o simplepixels.o 

simplepixels.o: cfacil.h simplepixels.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c simplepixels.cpp 

cfacil.o: cfacil.h cfacil.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c cfacil.cpp

clean:
	rm -f cfacil.o  simplepixels.o
	rm -r ./executable
