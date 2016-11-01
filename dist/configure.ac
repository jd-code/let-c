dnl
dnl Let-C Copyright (C) 2013 Jean-Daniel Pauget
dnl A whole set of http tcp sessions
dnl
dnl let-c@nekodune.com  -  http://let-c.nekodune.com/


dnl This file is part of Let-C
dnl 
dnl Let-C is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl 
dnl Let-C is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with Let-C; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
dnl 
dnl you can also try the web at http://www.gnu.org/
dnl 
dnl
dnl let-c@nekodune.com  -  http://let-c.nekodune.com/

dnl Process this file with autoconf to produce a configure script.
AC_INIT([let-c], [0.9.5], [let-c@nekodune.com])
AC_CONFIG_SRCDIR([./let-c.cpp])


dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET


dnl quieten some complains from missing files
AM_INIT_AUTOMAKE([foreign])

dnl the magic of additionnal local m4 ?
AC_CONFIG_MACRO_DIR([m4])


dnl dnl dnl autoheader
dnl dnl AM_CONFIG_HEADER(config.h)

dnl Check for tools

AC_PROG_CC
AC_PROG_CXX
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

dnl Check for compiler environment

AC_C_CONST

dnl JD's standard minimum CFLAG
dnl CHANGED for JD's ENterGroup default nasty warnings
CFLAGS="$CFLAGS -O2 -Wall -Wcast-qual -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes"
CXXFLAGS="$CXXFLAGS -O2 -Wall -Woverloaded-virtual -Wcast-qual -Wwrite-strings -Wredundant-decls"


dnl explain we want rather c++
AC_LANG_CPLUSPLUS

dnl check for matcalc template -------------------------------------------------------
AC_MSG_CHECKING(for matcalc template)
have_matcalc=no
SAVECXXFLAGS="$CXXFLAGS"
CXXFLAGS="$SAVECXXFLAGS $INCPATH"
AC_TRY_COMPILE([
#include <matcalc/matcalc.h>
],[
],[
have_matcalc=yes
])
if test x$have_matcalc = xno; then
    for P_INC in /usr /usr/local /opt/local "${prefix}"
    do
	if test x$have_matcalc = xno; then
	    /bin/echo -n "  ($P_INC :"
	    CXXFLAGS="$SAVECXXFLAGS $INCPATH -I$P_INC/include"
	    AC_TRY_COMPILE([
#include <matcalc/matcalc.h>
	    ],[
	    ],[
	    have_matcalc=yes
	    ])
	    if test x$have_matcalc = xyes; then
		INCPATH="$INCPATH -I$P_INC/include"
		/bin/echo -n " yes.)"
	    else
		/bin/echo -n " no.)"
	    fi
	fi
    done
    /bin/echo -n " : "
fi
CXXFLAGS="$SAVECXXFLAGS"
AC_MSG_RESULT($have_matcalc)
if test x$have_matcalc = xyes; then
    :
else
    AC_MSG_ERROR(Unable to find matcalc headers)
fi

dnl end of check for matcalc template ------------------------------------------------
dnl let's find where the nano editor stands ...
AC_MSG_CHECKING(for nano editor)
NANO_EDITOR_BIN=""
for NNDIR in /bin /usr/bin /usr/local/bin
do
    if test  -x "${NNDIR}"/nano
    then
	if "${NNDIR}/nano" -V | grep 'GNU nano' > /dev/null
	then
	    NANO_EDITOR_BIN="${NNDIR}/nano"
	fi
    fi
done
if test "${NANO_EDITOR_BIN}" ; then
    AC_MSG_RESULT([$NANO_EDITOR_BIN])
else
    AC_MSG_ERROR(Unable to find nano editor)
fi
AC_SUBST(AC_NANO_EDITOR_BIN,[$NANO_EDITOR_BIN])

dnl check for sdl to be used
use_sdl="sdl1"
AC_MSG_CHECKING([for which libsdl to use])
AC_ARG_ENABLE(sdl2,
[  --enable-sdl2           build with libSDL2],
    use_sdl="sdl2")
AC_MSG_RESULT($use_sdl)

if test "$use_sdl" = "sdl2" ; then
SDL_CFLAGS="`pkg-config --cflags sdl2`"
SDL_LDFLAGS="`pkg-config --libs sdl2`"
else
SDL_CFLAGS="`pkg-config --cflags sdl`"
SDL_LDFLAGS="`pkg-config --libs sdl`"
fi
AC_SUBST(SDL_CFLAGS, [$SDL_CFLAGS])
AC_SUBST(SDL_LDFLAGS, [$SDL_LDFLAGS])


dnl where to find and compile with GL
GL_LDFLAGS="`pkg-config --libs gl`"
GL_CFLAGS="`pkg-config --cflags gl`"
case "$target" in
    *-apple-darwin*)
	GL_LDFLAGS="-Wl,-framework,OpenGL"
	GL_CFLAGS=""
	;;
    *)
	:
	;;
esac
AC_SUBST(GL_CFLAGS, [$GL_CFLAGS])
AC_SUBST(GL_LDFLAGS, [$GL_LDFLAGS])



dnl check for mac-osX file scheme
LIBRT_LDFLAGS=""
case "$target" in
    *-apple-darwin*)
	AC_DEFINE(OSXFILESCHEME, 1, [a macosX file scheme flavour is used])
	AC_MSG_RESULT([a macosX file scheme flavour is used])
	LIBRT_LDFLAGS=""
	;;
    *)
	AC_DEFINE(OSXFILESCHEME, 0, [a regular unix file scheme flavour is used])
	AC_MSG_RESULT([a regular unix file scheme flavour is used])
	LIBRT_LDFLAGS="-lrt"
	;;
esac
AC_SUBST(LIBRT_LDFLAGS, [$LIBRT_LDFLAGS])

dnl concatenation of include dirs
CFLAGS="$CFLAGS $INCPATH"
CXXFLAGS="$CXXFLAGS $INCPATH"

dnl AC_EXPAND_DIR(datadir,LETCDATADIR)
dnl DEFINE_UNQUOTED([DATA_PATH], ["$pkgdatadir"])

dnl Finally create all the generated files
AC_OUTPUT([
	Makefile
	Makefile.template
])


