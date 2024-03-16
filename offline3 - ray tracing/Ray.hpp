#ifndef RAY
#define RAY

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "vector3D.hpp"
#include <cmath>

class Ray {

public:
    Vector3D* start;
    Vector3D* direction;

    Ray() {
        this->start = new Vector3D(0,0,0);
        this->direction = new Vector3D(0,0,0);
    }

    Ray(Vector3D* start, Vector3D* direction) {
        this->start = new Vector3D(0,0,0);
        this->direction = new Vector3D(0,0,0);
        this->start = start;
        this->direction = direction;
    }

    Vector3D* pointAtT(double t) {
        Vector3D* temp = new Vector3D(0,0,0);
        temp = this->start->Add2Vec(this->start, this->direction->Mul2(this->direction, t));
        return temp;
    }
};

#endif

