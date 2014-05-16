#include "cfacil.h"

int main (void) {
    cout << "on demarre ..." << endl;

    int W = 320;
    int H = 200;

    initscreen (W, H);

    for (int i=0 ; i<30000 ; i++) {

	clear();
//        for (int x=0 ; x<W ; x++) {
//            for (int y=0 ; y<H ; y++) {
//
//                putpixel (x, y,  255.0*((x+i)/(double)W),  255.0*((y+i*2)/(double)H),  255.0*((x-i)/(double)W)  );
//           }
//        }

	for (int t=0 ; t<20 ; t++) {
	    setcurcolor (0,0,t*10+50);
	    setcurcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));

	    setcurcolor (0,t*10+50,0);
	    setcurcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));

	    setcurcolor (t*10+50,0,0);
	    setcurcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));
	}

        if (checkelapsedtime(40)) {
            uploadtexture ();
            renderflatty ();
        }

    }

    cerr << "on va finir" << endl;

    SDL_Quit ();

    return 0;
}
