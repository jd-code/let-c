
#include <map>

#define CFACIL_STATICINSTANCE
#include "let-c.h"

#include <pthread.h>
//#include "SDL_thread.h"

#ifdef __MACH__
#include <sys/time.h>
#endif	// __MACH__

#if SDL_VERSION_ATLEAST(2, 0, 0)
#define SDL2 SDL2_IN_USE
#endif

using namespace std;

namespace let_c {

#ifdef SDL2
typedef SDL_Keycode KeyCode;
#else
typedef SDLKey KeyCode;
#endif


    bool checkelapsedtime (uint32_t ceil) {
#ifdef __MACH__
static struct timeval lastcall;	    // maybe gettimefoday should be used on all systems ????
	struct timeval curtime;
	if (gettimeofday(&curtime, NULL)) {
	    uint32_t r = (curtime.tv_sec - lastcall.tv_sec) * 1000 + (curtime.tv_usec - lastcall.tv_usec) / 1000;
	    if (r > ceil) {
		lastcall.tv_sec = curtime.tv_sec;
		lastcall.tv_usec = curtime.tv_usec;
		return true;
	    }
	}
	return false;
#else  // __MACH__
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
#endif  // __MACH__
    }


	int	initial_w = -1,
		initial_h = -1;
	int	 window_w = -1,
		 window_h = -1;
   bool exitoninitfailure = true;
#ifdef SDL2
  SDL_Window     *Wscreen = NULL;
  SDL_GLContext glcontext = NULL;
#else
  SDL_Surface	  *screen = NULL;
#endif
       GLuint	  texture;
       uint8_t *rawscreen = NULL;

	 int	 texturew = -1,
		 textureh = -1;

    Uint32  currentwpixel = 0xffffffff;

    bool      autorefresh = true;
  pthread_mutex_t GLmutex = PTHREAD_MUTEX_INITIALIZER;

    int syncscreen = 0;
    int needrefresh = 1;

    int mousekey = 0;
    int mousex = 0;
    int mousey = 0;


#ifndef __MACH__
#   define USE_SIGTIMER
#else
#   define USE_METRONOME
#endif

#ifdef USE_SIGTIMER
#   define CLOCKID CLOCK_REALTIME
#   define SIG SIGRTMIN
#endif
#ifdef USE_METRONOME
#   define SIG SIGALRM
#endif

#ifdef USE_SIGTIMER
    timer_t timerid;
#endif

    int stoptimer (void) {
	int r = 0;
	sigset_t mask;
	/* Block timer signal temporarily */
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at sigprocmask : " << strerror(e) << endl;
	    r = -1;
	}

#ifdef USE_SIGTIMER
	timer_delete (timerid);
#else
	// JDJDJDJDJD missing stopping metronome thread
#endif
	return r;
    }

    void uncatch_failures (void) {
	exitoninitfailure = false;
    }

    int initmaxscreen (int &w, int &h) {
// ------- start
	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
//	if (SDL_Init (SDL_INIT_EVERYTHING) < 0) {
	    cerr << "SDL_Init (SDL_INIT_VIDEO) failed : " << SDL_GetError() << endl ;
	    if (exitoninitfailure) exit (2);
	    return -1;
	}
	{
#ifdef SDL2
	    int nbdisplay = SDL_GetNumVideoDisplays ();
	    if (nbdisplay <= 0) {
		cerr << "SDL_GetNumVideoDisplays reported " << nbdisplay << " !" << endl;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    SDL_Rect rect, maxrect;
	    int i, maxi=-1, max = -1;
	    for (i=0 ; i<nbdisplay ; i++) {
		if (SDL_GetDisplayBounds (i, &rect) != 0) continue;
		if (rect.h*rect.w > max) {
		    maxi = i;
		    max = rect.h*rect.w;
		    maxrect = rect;
		}
	    }
	    if (maxi == -1) {
		cerr << "could not get screen bounds with any SDL_GetDisplayBounds" << endl;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    initial_w = rect.w;
	    initial_h = rect.h;
	    
#else
    	const SDL_VideoInfo *p = SDL_GetVideoInfo ();	// let's gather the screen size
	    if (p == NULL) {
		cerr << "SDL_VideoInfo () failed : " << SDL_GetError() << endl ;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    initial_w = p->current_w;
	    initial_h = p->current_h;
#endif
	}
	// screen = SDL_SetVideoMode(scr_width, scr_height, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);

	{
	    double screen_ratio = ((double)initial_w) / initial_h;
	    double render_ratio = ((double)w) / h;

	    if (screen_ratio > render_ratio) {
		window_h = initial_h-128;
		window_w = window_h * render_ratio;
	    } else {
		window_w = initial_w-128;
		window_h = window_w / render_ratio;
	    }
	}
//	window_w = initial_w-128;
//	window_h = initial_h-128;
#ifdef SDL2
//SDL_Window *
	Wscreen = SDL_CreateWindow("let-c app",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          window_w, window_h,
                          // SDL_WINDOW_FULLSCREEN | 
			  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (Wscreen == NULL) {
	    cerr << "Couldn't set " << window_w << "x" << window_h << " video mode : " << SDL_GetError() << endl;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
	if (
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION,      3) ||
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION,      3) ||
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,       SDL_GL_CONTEXT_PROFILE_CORE) ||
//            SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL,         1) ||
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,               1)
	) {
	    cerr << "SDL_GL_SetAttribute Error: " << SDL_GetError() << endl;
	}

	glcontext = SDL_GL_CreateContext(Wscreen);

	// JDJDJDJD are those necessary ?
	SDL_GL_SetSwapInterval(1);
	if (SDL_GL_MakeCurrent(Wscreen, glcontext) != 0)
	    cerr << "could not set current gl context ?" << endl;
//    if( !InitGLLoad()){
//        cerr << "InitGLLoad Error: " << endl;
//    }
//    cerr << glGetString( GL_VERSION) << endl;
#else
	screen = SDL_SetVideoMode(window_w, window_h, 16, SDL_OPENGL|SDL_SWSURFACE);
	// screen = SDL_SetVideoMode(initial_w-128, initial_h-128, 16, SDL_OPENGL|SDL_SWSURFACE);


	if (screen == NULL) {
	    cerr << "Couldn't set " << window_w << "x" << window_h << " video mode : " << SDL_GetError() << endl;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
#endif

	h=window_h;
	w=window_w;
cerr << "[" << w << "x" << h << "]" << endl;
	rawscreen = (uint8_t *) malloc (4*w*h);
	if  (rawscreen == NULL) {
	    cerr << "initscreen : could not allocate " << w << "x" << h << "x4 bytes ..." << endl;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
	texturew = w;
	textureh = h;


	glEnable(GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, texturew, textureh, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawscreen);

//	GLmutex = SDL_CreateMutex();
	pthread_mutex_init (&GLmutex, NULL);

	settle_timer ();

	clear();
	return 0;
    }

    int initscreen (int w, int h) {
	rawscreen = (uint8_t *) malloc (4*w*h);
	if  (rawscreen == NULL) {
	    cerr << "initscreen : could not allocate " << w << "x" << h << "x4 bytes ..." << endl;
	    if (exitoninitfailure) exit (2);
	    return -1;
	}
	texturew = w;
	textureh = h;
	if (SDL_Init ( SDL_INIT_VIDEO) < 0) {
//	if (SDL_Init (SDL_INIT_EVERYTHING) < 0) {
	    cerr << "SDL_Init (SDL_INIT_VIDEO) failed : " << SDL_GetError() << endl ;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
#ifdef SDL2
	    int nbdisplay = SDL_GetNumVideoDisplays ();
	    if (nbdisplay <= 0) {
		cerr << "SDL_GetNumVideoDisplays reported " << nbdisplay << " !" << endl;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    SDL_Rect rect, maxrect;
	    int i, maxi=-1, max = -1;
	    for (i=0 ; i<nbdisplay ; i++) {
		if (SDL_GetDisplayBounds (i, &rect) != 0) continue;
		if (rect.h*rect.w > max) {
		    maxi = i;
		    max = rect.h*rect.w;
		    maxrect = rect;
		}
	    }
	    if (maxi == -1) {
		cerr << "could not get screen bounds with any SDL_GetDisplayBounds" << endl;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    initial_w = rect.w;
	    initial_h = rect.h;
	    
#else
    	const SDL_VideoInfo *p = SDL_GetVideoInfo ();	// let's gather the screen size
	    if (p == NULL) {
		cerr << "SDL_VideoInfo () failed : " << SDL_GetError() << endl ;
		if (exitoninitfailure) { SDL_Quit (); exit (2); }
		return -1;
	    }
	    initial_w = p->current_w;
	    initial_h = p->current_h;
#endif
	// screen = SDL_SetVideoMode(scr_width, scr_height, 16, SDL_OPENGL|SDL_SWSURFACE|SDL_RESIZABLE);

	{
	    double screen_ratio = ((double)initial_w) / initial_h;
	    double render_ratio = ((double)w) / h;

	    if (screen_ratio > render_ratio) {
		window_h = initial_h-128;
		window_w = window_h * render_ratio;
	    } else {
		window_w = initial_w-128;
		window_h = window_w / render_ratio;
	    }
	}


#ifdef SDL2
//SDL_Window *
	Wscreen = SDL_CreateWindow("let-c app",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          window_w, window_h,
                          // SDL_WINDOW_FULLSCREEN | 
			  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (Wscreen == NULL) {
	    cerr << "Couldn't set " << window_w << "x" << window_h << " video mode : " << SDL_GetError() << endl;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
	if (
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION,      3) ||
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION,      3) ||
//            SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK,       SDL_GL_CONTEXT_PROFILE_CORE) ||
//            SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL,         1) ||
            SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,               1)
	) {
	    cerr << "SDL_GL_SetAttribute Error: " << SDL_GetError() << endl;
	}
	glcontext = SDL_GL_CreateContext(Wscreen);

	// JDJDJDJD are those necessary ?
	SDL_GL_SetSwapInterval(1);
	if (SDL_GL_MakeCurrent(Wscreen, glcontext) != 0)
	    cerr << "could not set current gl context ?" << endl;
//    if( !InitGLLoad()){
//        cerr << "InitGLLoad Error: " << endl;
//    }
//    cerr << glGetString( GL_VERSION) << endl;
    

#else
	screen = SDL_SetVideoMode(window_w, window_h, 16, SDL_OPENGL|SDL_SWSURFACE);
	// screen = SDL_SetVideoMode(initial_w-128, initial_h-128, 16, SDL_OPENGL|SDL_SWSURFACE);


	if (screen == NULL) {
	    cerr << "Couldn't set " << window_w << "x" << window_h << " video mode : " << SDL_GetError() << endl;
	    if (exitoninitfailure) { SDL_Quit (); exit (2); }
	    return -1;
	}
#endif

	glEnable(GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, texturew, textureh, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawscreen);

//	GLmutex = SDL_CreateMutex();
	pthread_mutex_init (&GLmutex, NULL);

	settle_timer ();

	clear();
	return 0;
    }

    void leavescreen (void) {
#ifdef SDL2
	if (Wscreen == NULL) return;
#else
	if (screen == NULL) return;
#endif
	stoptimer ();
	time_t t = time (NULL);
	while (time(NULL) - t < 2) {
	    if (pthread_mutex_trylock (&GLmutex) != 0) {
		break;
	    }
	}
	pthread_mutex_destroy (&GLmutex);
	SDL_GL_DeleteContext(glcontext);
	SDL_Quit ();
#ifdef SDL2
	Wscreen = NULL;
#else
	screen = NULL;
#endif
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

#ifdef SDL2
	{   int w,h;
	    SDL_GetWindowSize(Wscreen, &w, &h);
	    glViewport (0,0, w, h);
	    // glViewport (0,0, 482, 772);
	}
#else
	glViewport (0,0, screen->w, screen->h);
#endif

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
#ifdef SDL2
	SDL_GL_SwapWindow (Wscreen);
#else
	SDL_GL_SwapBuffers();
#endif
    }


    map<KeyCode, int> keypressed;
    int nbkeypressed = 0;
    bool escapeout = true;	// does "escape"-key toggles quit


    void timersofty (void) {
	SDL_Event event;
	while (SDL_PollEvent (&event)) {
	    switch (event.type) {
		case SDL_QUIT:
		    leavescreen ();
cerr << "Ended via closing window" << endl;
		    exit(0);
		case SDL_MOUSEMOTION:
		    mousex = event.motion.x;
		    mousey = event.motion.y;
		    break;
		case SDL_MOUSEBUTTONDOWN:
		    mousekey |= 1 << (event.button.button-1);
		    break;
		case SDL_MOUSEBUTTONUP:
		    mousekey &= ~(1 << (event.button.button-1));
		    break;

		case SDL_KEYDOWN:
// cerr << "keysym = " << event.key.keysym.sym << endl;
		    KeyCode keysym = event.key.keysym.sym;
		    if (escapeout && (keysym == SDLK_ESCAPE)) {
			leavescreen ();
cerr << "Ended via Escape keystroke" << endl;
			exit(0);
		    }
		    map<KeyCode, int>::iterator mi = keypressed.find(keysym);
		    if (mi == keypressed.end())
			keypressed[keysym] = 1;
		    else
			keypressed[keysym] ++;
		    nbkeypressed ++;
	    }
	}

	if (!autorefresh) return;
	if (pthread_mutex_trylock (&GLmutex) != 0) return;
	if (needrefresh == 0) {
	    syncscreen = 1;
	    pthread_mutex_unlock (&GLmutex);
	    return;
	}
//	    if (checkelapsedtime (10)) {
		uploadtexture ();
		renderflatty ();
		syncscreen = 1;
		needrefresh = 0;
//	    }
	pthread_mutex_unlock (&GLmutex);
    }

    static void timerhandler (int sig, siginfo_t *si, void *uc) {
	timersofty ();
    }

    void inline autoupdatetexture () {
	if (!autorefresh) return;
	needrefresh = 1;
//	if (pthread_mutex_trylock (&GLmutex) != 0) return;
//	    if (checkelapsedtime (33)) {
//		uploadtexture ();
//		renderflatty ();
//	    }
//	pthread_mutex_unlock (&GLmutex);
    }

    void startrefresh (void) {
	autorefresh = true;
	needrefresh = 1;
	autoupdatetexture ();
    }

    void stoprefresh (void) {
	autorefresh = false;
    }

    void * start_thr_timer (void * V) {
	pid_t pid = *(pid_t *)V;

	while (true) {
	    usleep (1000000/25);
	    kill (pid, SIG);
	}
    }

    int settle_timer (void) {
#ifdef USE_SIGTIMER
	struct itimerspec its;
	struct sigevent sev;
#endif
	sigset_t mask;
	struct sigaction sa;

	/* Establish handler for timer signal */
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timerhandler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at sigaction : " << strerror(e) << endl;
	    return -1;
	}

	/* Block timer signal temporarily */
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at sigprocmask : " << strerror(e) << endl;
	    return -1;
	}

#ifdef USE_SIGTIMER
	/* Create the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCKID, &sev, &timerid) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at timer_create : " << strerror(e) << endl;
	    return -1;
	}

	/* Start the timer */
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = 50000000;    // 20Hz ?
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;

	if (timer_settime(timerid, 0, &its, NULL) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at timer_settime : " << strerror(e) << endl;
	    return -1;
	}
#else
	static pthread_t thr_timer;
	static pid_t pid = getpid();
	if (pthread_create (&thr_timer, NULL, &start_thr_timer, &pid) != 0) {
	    int e = errno;
	    cerr << "pthread_create (thr_timer) failed : " << strerror (e) << endl;
	}
#endif

	/* unblocking signales */
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
	    int e = errno;
	    cerr << "settle_timer, error at sigprocmask : " << strerror(e) << endl;
	    return -1;
	}

	return 0;
    }

    void vsync (void) {
	if (!autorefresh) return;
	sigset_t mask, oldmask;
	sigemptyset (&mask);
	sigaddset (&mask, SIG);

	sigprocmask (SIG_BLOCK, &mask, &oldmask);
	syncscreen = 0;
	while (syncscreen == 0)
	    sigsuspend (&oldmask);
	sigprocmask (SIG_UNBLOCK, &mask, NULL);
    }

    void setcolor (int r, int g, int b) {
	// currentwpixel = ((r & 0xff)<<24) | ((g & 0xff)<<16) | ((b&0xff)<<8) | 0xff;
	currentwpixel = (r & 0xff) | ((b & 0xff)<<16) | ((g&0xff)<<8) | (0xff<<24);
    }

    void fputpixel (int x, int y, int r, int g, int b) {
	uint8_t *p = rawscreen + ((x+texturew*y) << 2);
	*p++ = r;
	*p++ = g;
	*p++ = b;
	*p++ = 255;
	autoupdatetexture ();
    }

    void fputpixel (int x, int y) {
	uint32_t *p = (uint32_t *) (rawscreen + ((x+texturew*y) << 2));
	*p = currentwpixel;
	autoupdatetexture ();
    }

    void putpixel (int x, int y, int r, int g, int b) {
	if ((x<0) || (y<0) || (x>=texturew) || (y>=textureh)) return;
	uint8_t *p = rawscreen + ((x+texturew*y) << 2);
	*p++ = r;
	*p++ = g;
	*p++ = b;
	*p++ = 255;
	autoupdatetexture ();
    }

    void putpixel (int x, int y) {
	if ((x<0) || (y<0) || (x>=texturew) || (y>=textureh)) return;
	uint32_t *p = (uint32_t *) (rawscreen + ((x+texturew*y) << 2));
	*p = currentwpixel;
	autoupdatetexture ();
    }

    void clear (void) {
	uint32_t *p = (uint32_t*) rawscreen;
	int nbpix=texturew*textureh;
	for (int i=0 ; i<nbpix ; i++) *p++ = 0;
	autoupdatetexture ();
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
	autoupdatetexture ();
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
	autoupdatetexture ();
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
		fputpixel (x0,y0);
	    if ((x0 == x1) && (y0 == y1)) break;

	    int e2 = err;
	    if (e2 > -dx) { 
		err -= dy; x0 += sx;
	    }
	    if (e2 < dy) {
		err += dx; y0 += sy;
	    }
	}
	if ((x0>=0) && (y0>=0) && (x0<texturew) && (y0<textureh))
	    fputpixel (x0,y0);
	autoupdatetexture ();
    }

    void circle (int xc, int yc, double r) {
	double a;
	int x1 = xc+r, y1=yc;

	stoprefresh ();
	for (a=0.01 ; a < 2*M_PI ; a+= 0.01) {
	    int x2 = xc + r*cos(a);
	    int y2 = yc + r*sin(a);
	    line (x1,y1, x2,y2);
	    x1=x2, y1=y2;
	}
	line (x1,y1, xc+r,yc);
	startrefresh ();
    }

    int mousek (void) {
	return mousekey;
    }

    void getmouse (int &x, int &y) {
	x = (texturew*mousex)/window_w;
	y = (textureh*(window_h-mousey))/window_h;
    }

    void getmouse (int &x, int &y, int &k) {
	x = (texturew*mousex)/window_w;
	y = (textureh*(window_h-mousey))/window_h;
	k = mousekey;
    }


    int Random (int max) {
	return (int)(((long int)random () * max)/RAND_MAX);
    }

    GlobalInstance::GlobalInstance () {
	cerr << "Start !" << endl;
    }

    GlobalInstance::~GlobalInstance () {
	cerr << "End !" << endl;
	leavescreen ();
    }

} // namespace let_c

//	using namespace let_c;
//	
//	extern "C" int __real_main (int nb, char ** cmde);
//	
//	extern "C" int __wrap_main (int nb, char ** cmde) {
//	
//	
//	cerr << "Start !" << endl;
//	    int r = __real_main (nb, cmde);
//	cerr << "End !" << endl;
//	
//	    leavescreen ();
//	
//	    return r;
//	}

