#ifndef CAMERA
#define CAMERA

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "vector3D.hpp"
#include <cmath>


using namespace std;


class Camera {

public:
    // double height = 40.0, angle = 1.32;
    Vector3D* pos;
	Vector3D* u;
	Vector3D* r;
	Vector3D* l;
    double angularSpeed;

    Camera(double posX = 115.0, double posY = 115.0, double posZ = 50.0) {
        pos = Vector3D(posX, posY, posZ);
        l = Vector3D(-1 / sqrt(2), -1 / sqrt(2), 0);
        r = Vector3D(-1 / sqrt(2), 1 / sqrt(2), 0);
        u = Vector3D(0, 0, 1);
        angularSpeed = 1.0 * M_PI / 180;
    }

    void lookLeft() {
        l = l*cos(angularSpeed) + u%l*sin(angularSpeed);
        r = r*cos(angularSpeed) + u%r*sin(angularSpeed);
    }

    void lookRight() {
        l = l*cos(-angularSpeed) + u%l*sin(-angularSpeed);
        r = r*cos(-angularSpeed) + u%r*sin(-angularSpeed);
    }

    void lookUp() {
        l = l*cos(angularSpeed) + r%l*sin(angularSpeed);
        u = u*cos(angularSpeed) + r%u*sin(angularSpeed);
    }

    void lookDown() {
        l = l*cos(-angularSpeed) + r%l*sin(-angularSpeed);
        u = u*cos(-angularSpeed) + r%u*sin(-angularSpeed);
    }

    void tiltClockWise() {
        r = r*cos(angularSpeed) + l%r*sin(angularSpeed);
        u = u*cos(angularSpeed) + l%u*sin(angularSpeed);
    }

    void tiltCounterClockWise() {
        r = r*cos(-angularSpeed) + l%r*sin(-angularSpeed);
        u = u*cos(-angularSpeed) + l%u*sin(-angularSpeed);
    }

    void moveForward() {
        pos = pos + l;
    }

    void moveBackward() {
        pos = pos - l;
    }

    void moveLeft() {
        pos = pos - r;
    }

    void moveRight() {
        pos = pos + r;
    }

    void moveUp() {
        pos = pos + u;
    }

    void moveDown() {
        pos = pos - u;
    }

    void upRef() {
        pos = pos + u*0.3;
    }

    void downRef() {
        pos = pos - u*0.3;
    }
};


#endif