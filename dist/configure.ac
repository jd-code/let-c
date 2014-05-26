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
AC_INIT([let-c], [0.9.1], [let-c@nekodune.com])
AC_CONFIG_SRCDIR([./let-c.cpp])


dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET


dnl quieten some complains from missing files
AM_INIT_AUTOMAKE([foreign])

dnl the magic of additionnal local m4 ?
AC_CONFIG_MACRO_DIR([m4])


dnldnl dnl autoheader
dnldnl AM_CONFIG_HEADER(config.h)

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


dnl dnl check for mac-osX file scheme
dnl case "$target" in
dnl     *-apple-darwin*)
dnl 	AC_DEFINE(OSXFILESCHEME, 1, [a macosX file scheme flavour is used])
dnl 	AC_MSG_RESULT([a macosX file scheme flavour is used])
dnl 	;;
dnl     *)
dnl 	AC_DEFINE(OSXFILESCHEME, 0, [a regular unix file scheme flavour is used])
dnl 	AC_MSG_RESULT([a regular unix file scheme flavour is used])
dnl 	;;
dnl esac


dnl concatenation of include dirs
CFLAGS="$CFLAGS $INCPATH"
CXXFLAGS="$CXXFLAGS $INCPATH"

dnl AC_EXPAND_DIR(datadir,LETCDATADIR)
dnl DEFINE_UNQUOTED([DATA_PATH], ["$pkgdatadir"])

dnl Finally create all the generated files
AC_OUTPUT([
	Makefile
])


