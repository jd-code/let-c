
vimtest: executable
	./executable


letcdir = $(datadir)/let-c
letc_DATA = let-c.cpp let-c.h Makefile.template 

dist_man_MANS = let-c.1 build-4-nano-let-c.1

dist_bin_SCRIPTS = build-4-nano-let-c let-c
CLEANFILES = $(dist_bin_SCRIPTS) $(dist_man_MANS)

do_subst = sed	-e 's,[@]LETCDATADIR[@],$(letcdir),g'			\
		-e 's,[@]LETCBINDIR[@],$(bindir),g'			\
		-e 's,[@]VERSION[@],${VERSION},g'			\
		-e 's,[@]NANO_EDITOR_BIN[@],$(AC_NANO_EDITOR_BIN),g'

let-c.1: let-c.man
	$(do_subst) < let-c.man > let-c.1

build-4-nano-let-c.1: build-4-nano-let-c.man
	$(do_subst) < build-4-nano-let-c.man > build-4-nano-let-c.1

build-4-nano-let-c: build-4-nano-let-c.in
	$(do_subst) < build-4-nano-let-c.in > build-4-nano-let-c
	chmod +x build-4-nano-let-c

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
	rm -f "${PACKAGE}_${VERSION}.orig.tar.gz" tarfiles.txt

.PHONY: tar
tar: 
	repozclean && autoall
	find . | grep -v '\.git' | grep -v 'debian-src' | grep -v 'autom4te' > tarfiles.txt
	bozoconf
	tar -zcphf "${PACKAGE}_${VERSION}.orig.tar.gz" --no-recursion --transform='s=^='"${PACKAGE}"_"${VERSION}"'/=' -T tarfiles.txt
	tar -ztvf "${PACKAGE}_${VERSION}.orig.tar.gz"
