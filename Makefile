
vimtest: executable
	./executable

executable: cfacil.h simplepixels.cpp
	g++ -Wall `pkg-config --cflags --libs sdl` `pkg-config --cflags --libs gl` -o executable simplepixels.cpp -lm

clean:
	rm -r ./executable
