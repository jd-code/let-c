#include "let-c.h"  // this inclusion isn't needed to state when using let-c build scheme

int main (int nb, char ** cmde) {
    cout << "on demarre ..." << endl;

    int W = 1024;
    int H = 768;

    if (false)
	initscreen (W, H);
    else
	initmaxscren (W, H);


    for (int i=0 ; mousek()==0 ; i++) {

	if (false) {
	    clear();
	} else {
	    stoprefresh();
	    for (int x=0 ; x<W ; x++) {
		for (int y=0 ; y<H ; y++) {

		    putpixel (x, y,  255.0*((x+i)/(double)W),  255.0*((y+i*2)/(double)H),  255.0*((x-i)/(double)W)  );
	       }
	    }
	    startrefresh();
	}

	for (int t=0 ; t<20 ; t++) {
	    setcolor (0,0,t*10+50);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));

	    setcolor (0,t*10+50,0);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));

	    setcolor (t*10+50,0,0);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(W), Random(H), Random(W), Random(H));
	}

	setcolor (255,255,255);
	int Xm, Ym;
	getmouse (Xm, Ym);
	line (Random(0), Random(0), Xm, Ym);
	for (int t=0 ; t<H ; t+=H/10) {
	    circle (Xm, Ym, t+(i % (H/10)));
	}

	vsync();
    }

    cerr << "on va finir" << endl;

    return 0;
}
