#ifndef GRAPHICSTUFF
#define GRAPHICSTUFF

#include "capd/krak/krak.h"

using namespace capd;
using namespace std;

const int DIMENSION=3;
double minx=-2.2;
double maxx=1.6;
double miny=-2.6;
double maxy=2.;

Frame fr, txt;

// -----------------------------------------------------------------

inline void initGraphics()
{
   openGW(900,700,WHITE,BLACK);
   rootFrame.Clear();
   txt = Frame(0,0,595,130);
   fr = Frame(5,135,555,585,WHITE,BLACK);
   fr.setWorldCoord(minx/2,miny/2,maxx/4,maxy/2);
   fr.line(0.0,miny,0.0,maxy,BLACK);
   fr.line(minx,0.0,maxx,0.0,BLACK);
}

#endif
