#include "borderColorPicker.h"
#include <cmath>
#include <iostream>

borderColorPicker::borderColorPicker(unsigned int borderSize, HSLAPixel fillColor, PNG &img, double tolerance)
{
    this->borderSize = borderSize;
    this->fillColor = fillColor;
    this->img = img;
    this->tolerance = tolerance;
}

HSLAPixel borderColorPicker::operator()(point p)
{
    HSLAPixel *center = img.getPixel(p.c.x, p.c.y);
    HSLAPixel *cur;
    int border = borderSize;

    for (int i = -border; i <= border; i ++) {
        for (int c = -border; c <= border; c++) {
            

            if (sqrt(pow(i, 2) + pow(c, 2)) <= border) {

                if (p.x + i >= img.width() || p.y + c >= img.height() || p.x + i < 0 || p.y + c < 0) {
                    return fillColor;
                }
                
                cur = img.getPixel(p.x + i, p.y + c);
                if (cur->dist(*center) >= tolerance) {
                    return fillColor;
                }
            }

            
        }
    }

    return *img.getPixel(p.x, p.y);
}
