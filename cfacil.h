#ifndef CFACIL_H_INCLUDED
#define CFACIL_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>

#include <math.h>

#include <matcalc/matcalc.h>

#include "SDL.h"

using namespace std;

namespace easycpp {

    bool checkelapsedtime (uint32_t ceil) {
static timespec lastcall;
	timespec curtime;
	clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &curtime);

	uint32_t r = (curtime.tv_sec - lastcall.tv_sec) * 1000 + (curtime.tv_nsec - lastcall.tv_nsec) / 1000000;
	if (r > ceil) {
	    lastcall.tv_sec = curtime.tv_sec;
	    lastcall.tv_nsec = curtime.tv_nsec;
	    return true;
	}
	return false;
    }




    Vector4 GLRGBA_BLACK       (0.0, 0.0, 0.0, 1.0);
    Vector4 GLRGBA_TRANSPBLACK (0.0, 0.0, 0.0, 0.0);
    Vector4 GLRGBA_WHITE       (1.0, 1.0, 1.0, 1.0);
    
    Vector4 GLRGBA_RED         (1.0, 0.0, 0.0, 1.0);
    Vector4 GLRGBA_GREEN       (0.0, 1.0, 0.0, 1.0);
    Vector4 GLRGBA_BLUE        (0.0, 0.0, 1.0, 1.0);
    Vector4 GLRGBA_YELLOW      (1.0, 1.0, 0.0, 1.0);
    Vector4 GLRGBA_CYAN        (0.0, 1.0, 1.0, 1.0);
    Vector4 GLRGBA_MAUVE       (1.0, 0.0, 1.0, 1.0);
    
    Vector4 GLRGBA_DARKRED     (0.5, 0.0, 0.0, 1.0);
    Vector4 GLRGBA_DARKGREEN   (0.0, 0.5, 0.0, 1.0);
    Vector4 GLRGBA_DARKBLUE    (0.0, 0.0, 0.5, 1.0);
    Vector4 GLRGBA_DARKYELLOW  (0.5, 0.5, 0.0, 1.0);
    Vector4 GLRGBA_DARKCYAN    (0.0, 0.5, 0.5, 1.0);
    Vector4 GLRGBA_DARKMAUVE   (0.5, 0.0, 0.5, 1.0);
    
    Vector4 GLRGBA_ORANGE      (1.0, 5.0, 0.0, 1.0);



	int	initial_w = -1,
		initial_h = -1;
	int	 window_w = -1,
		 window_h = -1;
  SDL_Surface	  *screen = NULL;
       GLuint	  texture;
       uint8_t *rawscreen = NULL;

	 int	 texturew = -1,
		 textureh = -1;

	Uint32	currentwpixel = 0xffffffff;

    int initscreen (int w, int h) {
	rawscreen = (uint8_t *) malloc (4*w*h);
	if  (rawscreen == NULL) {
	    cerr << "initscreen : could not allocate " << w << "x" << h << "x4 bytes ..." << endl;
	    return -1;
	}
	texturew = w;
	textureh = h;
	if (SDL_Init ( SDL_INIT_VIDEO) < 0) {
	    cerr << "SDL_Init (SDL_INIT_VIDEO) failed : " << SDL_GetError() << endl ;
	    return -1;
	}
	{	const SDL_VideoInfo *p = SDL_GetVideoInfo ();	// let's gather the screen size
	    if (p == NULL) {
		cerr << "SDL_VideoInfo () failed : " << SDL_GetError() << endl ;
		return -1;
	    }
	    initial_w = p->current_w;
	    initial_h = p->current_h;
	}
	// screen = SDL_SetVideoMode(scr_width, scr_height, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);

	{
	    double screen_ratio = ((double)initial_w) / initial_h;
	    double render_ratio = ((double)w) / h;

	    if (screen_ratio > render_ratio) {
		window_h = initial_h-32;
		window_w = window_h * render_ratio;
cerr << window_w << "x" << window_h << endl;
	    } else {
		window_w = initial_w-32;
		window_h = window_w / render_ratio;
	    }
	}


	screen = SDL_SetVideoMode(window_w, window_h, 16, SDL_OPENGL|SDL_SWSURFACE);
	// screen = SDL_SetVideoMode(initial_w-32, initial_h-32, 16, SDL_OPENGL|SDL_SWSURFACE);


	if (screen == NULL) {
	    cerr << "Couldn't set " << window_w << "x" << window_h << " video mode : " << SDL_GetError() << endl;
	    return -1;
	}

	glEnable(GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, texturew, textureh, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawscreen);

	return 0;
    }

    void uploadtexture (void) {
	//glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, texturew, textureh, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawscreen);
    }

    // -------------- flatty ----------------------------------------------------------------------------

//	GLint vx, vy;                                        //!< the lower left corner of the viewport rectangle
//	GLsizei vw, vh;                                      //!< the width and height of the viewport

    void renderflatty (void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_BLEND);
	glEnable (GL_TEXTURE_2D);
//	glShadeModel(GL_SMOOTH);

	glViewport (0,0, screen->w, screen->h);

	glMatrixMode ( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat *) GLRGBA_WHITE);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat *) GLRGBA_WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, (GLfloat *) GLRGBA_BLACK);

	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *) GLRGBA_WHITE);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,            (GLfloat *) GLRGBA_TRANSPBLACK );
	glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION,            (GLfloat *) GLRGBA_BLACK);
	glMateriali  (GL_FRONT_AND_BACK, GL_SHININESS,           128);

static GLfloat GLXYZT_FROMABOVE[] = {0.0, 0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, GLXYZT_FROMABOVE);

//	glShadeModel (GL_SMOOTH);
//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture (GL_TEXTURE_2D, texture);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_NORMALIZE);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin (GL_QUADS);
	    glTexCoord2f (0.0, 0.0);
	    glVertex3f (-1.0, -1.0, 0.0);
	    glNormal3f (0.0, 0.0, 1.0);

	    glTexCoord2f (1.0, 0.0);
	    glVertex3f ( 1.0, -1.0, 0.0);
	    glNormal3f (0.0, 0.0, 1.0);

	    glTexCoord2f (1.0, 1.0);
	    glVertex3f ( 1.0,  1.0, 0.0);
	    glNormal3f (0.0, 0.0, 1.0);

	    glTexCoord2f (0.0, 1.0);
	    glVertex3f (-1.0,  1.0, 0.0);
	    glNormal3f (0.0, 0.0, 1.0);
	glEnd ();

	glPopMatrix ();


	glDisable(GL_BLEND);
	glFinish();
	SDL_GL_SwapBuffers();
    }

    void setcurcolor (int r, int g, int b) {
	// currentwpixel = ((r & 0xff)<<24) | ((g & 0xff)<<16) | ((b&0xff)<<8) | 0xff;
	currentwpixel = (r & 0xff) | ((b & 0xff)<<16) | ((g&0xff)<<8) | (0xff<<24);
    }

    void putpixel (int x, int y, int r, int g, int b) {
	uint8_t *p = rawscreen + ((x+texturew*y) << 2);
	*p++ = r;
	*p++ = g;
	*p++ = b;
	*p++ = 255;
    }

    void putpixel (int x, int y) {
	uint32_t *p = (uint32_t *) (rawscreen + ((x+texturew*y) << 2));
	*p = currentwpixel;
    }

    void clear (void) {
	uint32_t *p = (uint32_t*) rawscreen;
	int nbpix=texturew*textureh;
	for (int i=0 ; i<nbpix ; i++) *p++ = 0;
    }

    void rhline (int x1, int y1, int x2, int y2, Uint32 pixel) {
	if ((y1<0) || (y1>=textureh)) return;
	if (x1 > x2) {
	    rhline (x2, y2, x1, y1, pixel);
	    return;
	}
	if (x2 < 0) return;
	if (x1 >= texturew) return;
	if (x1 < 0) x1 = 0;
	if (x2 >= texturew) x2 = texturew;
	Uint32 *p = (Uint32*)(rawscreen + ((x1+texturew*y1) << 2));
	for (; x1<x2 ; x1++)
	    *p++ = pixel;
    }

    void rvline (int x1, int y1, int x2, int y2, Uint32 pixel) {
	if ((x1 < 0) || (x1 >= texturew)) return;
	if (y1 > y2) {
	    rvline (x2, y2, x1, y1, pixel);
	    return;
	}
	if (y2 < 0) return;
	if (y1 >= textureh) return;
	if (y1 < 0) y1 = 0;
	if (y2 >= textureh) y2 = textureh-1;
	Uint32 *p = (Uint32*)(rawscreen + ((x1+texturew*y1) << 2));
	for (; y1<y2 ; y1++) {
//	    if ((y1>=0) && (y1<textureh))
		*p = pixel;
	    p += 640;
	}
    }

    void line(int x0, int y0, int x1, int y1) {
	if ((x0 < 0) && (x1 < 0)) return;
	if ((y0 < 0) && (y1 < 0)) return;
	if ((x0 >= texturew) && (x1 >= texturew)) return;
	if ((y0 >= textureh) && (y1 >= textureh)) return;

	int dx = abs(x1-x0);
	int dy = abs(y1-y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2;

	while (true) {
	    if ((x0>=0) && (y0>=0) && (x0<texturew) && (y0<textureh))
		putpixel (x0,y0);
	    if ((x0 == x1) && (y0 == y1)) break;

	    int e2 = err;
	    if (e2 > -dx) { 
		err -= dy; x0 += sx;
//if ((x0 == x1) && (y0 == y1)) break;
	    }
//if ((x0 == x1) && (y0 == y1)) break;
	    if (e2 < dy) {
		err += dx; y0 += sy;
//if ((x0 == x1) && (y0 == y1)) break;
	    }
	}
//	putpixel (x0,y0);
    }

    int Random (int max) {
	return (int)(((long int)random () * max)/RAND_MAX);
    }

} // namespace easycpp

using namespace easycpp;

#endif /* CFACIL_H_INCLUDED */
# 1 "orig.cpp"
