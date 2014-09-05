#include "let-c.h"  // this inclusion isn't needed to state when using let-c build scheme

int main (int nb, char ** cmde) {
    cout << "on demarre ..." << endl;

    int W = 320;
    int H = 200;

    if (true)
	    initscreen (W, H);
    else
	    initmaxscreen (W, H);

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

setdrawmode (TRANSPARENT);
settransparency (128);
	for (int t=0 ; t<20 ; t++) {
	    setcolor (0,0,t*10+50);
	    setcolor (Random(255), Random(255), Random(255));
	    rectangle (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);

	    setcolor (0,t*10+50,0);
	    setcolor (Random(255), Random(255), Random(255));
	    rectangle (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);
	    //rectangle (Random(W), Random(H), Random(W), Random(H));

	    setcolor (t*10+50,0,0);
	    setcolor (Random(255), Random(255), Random(255));
	    rectangle (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);
	}
setdrawmode (OPAQUE);

	for (int t=0 ; t<20 ; t++) {
	    setcolor (0,0,t*10+50);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);

	    setcolor (0,t*10+50,0);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);
	    line (Random(W), Random(H), Random(W), Random(H));

	    setcolor (t*10+50,0,0);
	    setcolor (Random(255), Random(255), Random(255));
	    line (Random(3*W)-W, Random(3*H)-H, Random(3*W)-W, Random(3*H)-H);
	}

	setcolor (255,255,255);
	int Xm, Ym;
	getmouse (Xm, Ym);
setdrawmode (TRANSPARENT);
settransparency (128);
	line (Random(0), Random(0), Xm, Ym);
	for (int t=0 ; t<H ; t+=H/10) {
	    circle (Xm, Ym, t+(i % (H/10)));
	}
setdrawmode (OPAQUE);

	vsync();
    }

    cerr << "on va finir" << endl;

    return 0;
}
