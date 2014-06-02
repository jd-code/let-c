
# Let-C #

**Let-C** is a set of headers, templates and utilities for programming tutorials.

By combining libSDL, simple graphical primitives, and some tuning to open source
utilities, **Let-C** permits writing a few lines of codes and have them compiled and
launched **in one key-press**.

> **Let-C** brings back the ease of simple programming that
> old pre-windows, pre-linux, pre-mac machines used to provide ...

# Example #

Typing "**let-c**" invokes a crafted intsance of the "nano" text-editor.

The following source is typed-in :

```C++
int main (void) {
    initscreen (640, 400);

    int i;
    for (i=0 ; i<400 ; i+=20) {
        setcolor (255-i,255-i/2,0);
        line (i,0, 0,400-i);
        line (399,i, 400-i,399);
    }

    while (mousek() == 0) {}    // wait for a mouse-click
    return 0;
}
```

at **F12** key-press, all is compiled and linked, and the result window pops up :

![result](https://raw.githubusercontent.com/jd-code/let-c/master/misc/let-c-filtendus.png)


## Dependencies ##

Let-C depends on several stae-of-the-art open-source softwares :

* [gcc](http://gcc.gnu.org/) - the GNU Compiler Collection, mostly the C/C++ compiler and the linker (for it's ability to wrap functions).
* [libSDL](http://www.libsdl.org/) - the Simple DirectMedia Layer, the version 1.2 is used, and the choice of 2.0 is underway.
* [OpenGL](http://www.opengl.org/) - The 3D graphic library is used internally, it is not planned to switch to another rendering method.
* [nano](http://www.nano-editor.org/) - The GNU nano text editor is used, a [patched version](https://github.com/jd-code/nano-let-c) can be used for a better handling of compiler's error messages.
* [matcalc](https://github.com/jd-code/matcalc) - A small set of 3d math templates.


## Packaged Versions ##

### Debian ###
packages for *wheezy* and *squeeze* are available via those repositories, gpg-signed with [that key](http://archive.disjunkt.com/package-maint-key.gpg)

- wheezy

    ```deb http://archive.disjunkt.com/debian/ wheezy main```

- squeeze

    ```deb http://archive.disjunkt.com/debian/ squeeze main```

## ubuntu ##

packages for ubutu releases are available via [launchpad/letc](https://launchpad.net/~nekodune/+archive/let-c).

adding the **Let-C** ubuntu repository to you system :
```sh
sudo add-apt-repository ppa:nekodune/let-c
```


## Installation ##

Packages are though to be the best way of installing (see above), however
the source can be compiled after some usual autotool invocations :

```sh
cd dist
(g)libtoolize --copy && aclocal && autoheader && automake   --add-missing && autoconf
./configure -prefix=/usr/local
make
make install
```

some releases ready to configure are also avalaible in the ```debian-src``` directory


## License ##

GPL-V2

