
#include "let-c.h"  // this inclusion isn't needed to state when using let-c build scheme


int main (void) {
    int W = 640;
    int H = 400;

    initscreen (W, H);
//    initmaxscreen (W, H);

    int nbline = H/5;
    int x, y;

double t = 0.0;
while (mousek() == 0) {
clear();

    int miny[W], maxy[W];
    for (int i=0 ; i<W ; i++) {
        miny[i]=H;
        maxy[i]=-1;
    }

    int Wplot = W-W/3;

    for (int line=0 ; line<nbline ; line++) {
        y=H/20 + (line*(H-H/10))/nbline;

        double yy = ((double)line/nbline) * 6.4 -3.2;

	    for (x=0 ; x<Wplot ; x++) {
	        double xx = ((double)x/Wplot) * 6.4 - 3.2;

//	        double zz = cos (4.0*sqrt(xx*xx+yy*yy));

            double d = sqrt(xx*xx+yy*yy);
	        double zz = cos (t + 7.0*d)*exp(-(d*d/4.0));



	        int yfinal = y + zz*(H/10);
            int xfinal = x + ((W-Wplot)*line)/nbline;

            if ((xfinal>0) && (xfinal<W) && (yfinal>0) && (yfinal<H)) {
                bool weplotit = false;

                if (yfinal > maxy[xfinal]) {
                    maxy[xfinal] = yfinal;
                    weplotit = true;
                }
                if (yfinal < miny[xfinal]) {
                    miny[xfinal] = yfinal;
                    weplotit = true;
                }

                if (weplotit)
	                putpixel (xfinal, yfinal);
            }
	    }
    }
t -= 0.01;
vsync();
}
    while (mousek() == 0) {}
}
