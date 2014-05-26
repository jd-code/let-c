
vimtest: executable
	./executable


letcdir = $(datadir)/let-c
letc_DATA = let-c.cpp let-c.h Makefile.template 

dist_bin_SCRIPTS = let-c-build let-c
CLEANFILES = $(dist_bin_SCRIPTS)

do_subst = sed	-e 's,[@]LETCDATADIR[@],$(letcdir),g'			\
		-e 's,[@]LETCBINDIR[@],$(bindir),g'			\
		-e 's,[@]NANO_EDITOR_BIN[@],$(AC_NANO_EDITOR_BIN),g'

let-c-build: let-c-build.in
	$(do_subst) < let-c-build.in > let-c-build
	chmod +x let-c-build

let-c: let-c.in
	$(do_subst) < let-c.in > let-c
	chmod +x let-c

executable: let-c.o simplepixels.o
	g++  let-c.o simplepixels.o `pkg-config --libs sdl` `pkg-config --libs gl` -o executable -lpthread -lm -lrt -Wl,--wrap,main 

simplepixels.o: let-c.h simplepixels.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c simplepixels.cpp 

let-c.o: let-c.h let-c.cpp
	g++ -Wall `pkg-config --cflags sdl` `pkg-config --cflags gl` -c let-c.cpp

clean-local:
	rm -f let-c.o  simplepixels.o
	rm -f ./executable
	rm -f m4/*.m4
	rm -f stamp-h2

