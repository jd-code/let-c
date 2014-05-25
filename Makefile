
vimtest: executable
	./executable

executable: let-c.o simplepixels.o
	g++  let-c.o simplepixels.o `pkg-config --libs sdl` `pkg-config --libs gl` -o executable -lpthread -lm -lrt -Wl,--wrap,main 

simplepixels.o: let-c.h simplepixels.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c simplepixels.cpp 

let-c.o: let-c.h let-c.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c let-c.cpp

clean:
	rm -f let-c.o  simplepixels.o
	rm -f ./executable
