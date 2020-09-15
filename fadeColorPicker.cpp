#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    HSLAPixel n = p.c.color;

    int x = p.x;
    int y = p.y;

    int distx = p.c.x - x;
    int disty = p.c.y - y;
    int distSq = sqrt(pow(distx, 2) + pow(disty, 2));

    n.l = p.c.color.l * pow(fadeFactor, distSq);

  //  cout <<"distx  " << distx << endl;
  //  cout <<  distSq << endl;





    return n;
}
