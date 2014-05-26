#ifndef CFACIL_H_INCLUDED
#define CFACIL_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include <errno.h>
#include <string.h>

#include <math.h>

#include <matcalc/matcalc.h>

#include "SDL.h"

using namespace std;

namespace let_c {

    bool checkelapsedtime (uint32_t ceil);

#ifdef CFACIL_STATICINSTANCE
#   define CFACIL_H_SCOPE
#else
#   define CFACIL_H_SCOPE extern
#endif

#ifdef CFACIL_STATICINSTANCE
CFACIL_H_SCOPE    Vector4 GLRGBA_BLACK       (0.0, 0.0, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_TRANSPBLACK (0.0, 0.0, 0.0, 0.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_WHITE       (1.0, 1.0, 1.0, 1.0);

CFACIL_H_SCOPE    Vector4 GLRGBA_RED         (1.0, 0.0, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_GREEN       (0.0, 1.0, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_BLUE        (0.0, 0.0, 1.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_YELLOW      (1.0, 1.0, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_CYAN        (0.0, 1.0, 1.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_MAUVE       (1.0, 0.0, 1.0, 1.0);

CFACIL_H_SCOPE    Vector4 GLRGBA_DARKRED     (0.5, 0.0, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKGREEN   (0.0, 0.5, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKBLUE    (0.0, 0.0, 0.5, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKYELLOW  (0.5, 0.5, 0.0, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKCYAN    (0.0, 0.5, 0.5, 1.0);
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKMAUVE   (0.5, 0.0, 0.5, 1.0);

CFACIL_H_SCOPE    Vector4 GLRGBA_ORANGE      (1.0, 5.0, 0.0, 1.0);
#else
CFACIL_H_SCOPE    Vector4 GLRGBA_BLACK       ;
CFACIL_H_SCOPE    Vector4 GLRGBA_TRANSPBLACK ;
CFACIL_H_SCOPE    Vector4 GLRGBA_WHITE       ;

CFACIL_H_SCOPE    Vector4 GLRGBA_RED         ;
CFACIL_H_SCOPE    Vector4 GLRGBA_GREEN       ;
CFACIL_H_SCOPE    Vector4 GLRGBA_BLUE        ;
CFACIL_H_SCOPE    Vector4 GLRGBA_YELLOW      ;
CFACIL_H_SCOPE    Vector4 GLRGBA_CYAN        ;
CFACIL_H_SCOPE    Vector4 GLRGBA_MAUVE       ;

CFACIL_H_SCOPE    Vector4 GLRGBA_DARKRED     ;
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKGREEN   ;
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKBLUE    ;
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKYELLOW  ;
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKCYAN    ;
CFACIL_H_SCOPE    Vector4 GLRGBA_DARKMAUVE   ;

CFACIL_H_SCOPE    Vector4 GLRGBA_ORANGE      ;
#endif

    int initscreen (int w, int h);

    void uploadtexture (void);
    void renderflatty (void);
    int settle_timer (void);

    void startrefresh (void);
    void stoprefresh (void);

    void vsync (void);

    void setcolor (int r, int g, int b);
    void putpixel (int x, int y, int r, int g, int b);
    void putpixel (int x, int y);
    void clear (void);

    void rhline (int x1, int y1, int x2, int y2, Uint32 pixel);
    void rvline (int x1, int y1, int x2, int y2, Uint32 pixel);
    void line(int x0, int y0, int x1, int y1);
    int Random (int max);

} // namespace let_c

using namespace let_c;

#endif /* CFACIL_H_INCLUDED */
# 1 "orig.cpp"
