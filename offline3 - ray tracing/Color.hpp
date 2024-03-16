#ifndef COLOR
#define COLOR

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "vector3D.hpp"
#include <cmath>


class Color {
public:
    double red;
    double green;
    double blue;

    Color() {
        this->red = 0;
        this->green = 0;
        this->blue = 0;

    }

    Color(double red, double green, double blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Color* Mul2(double val) {
        Color* temp = new Color(this->red * val, this->green * val, this->blue * val);
        temp = temp->rangeCheck();

        return temp;
    }

    Color* Cross2Color(Color* c) {
        Color* temp = new Color(this->red * c->red, this->green * c->green, this->blue * c->blue);
        temp = temp->rangeCheck();

        return temp;
    }

    Color* Add2Color(Color* c) {
        Color* temp = new Color(this->red + c->red, this->green + c->green, this->blue + c->blue);
        temp = temp->rangeCheck();

        return temp;
    }

    Color* rangeCheck() {
        if (this->red > 1) {
            this->red = 1;
        }
        if (this->green > 1) {
            this->green = 1;
        }
        if (this->blue > 1) {
            this->blue = 1;
            }

        if (this->red < 0) {
            this->red = 0;
            }
        if (this->green < 0) {
            this->green = 0;
            }
        if (this->blue < 0) {
            this->blue = 0;
        }

        Color* temp = new Color(this->red, this->green, this->blue);

        return temp;
    }
};



#endif
