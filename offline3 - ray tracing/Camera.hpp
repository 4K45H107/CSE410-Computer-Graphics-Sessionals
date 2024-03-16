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
    Vector3D* POSITION = new Vector3D(0,0,0);
    Vector3D* UP = new Vector3D(0,0,0);
    Vector3D* RIGHT = new Vector3D(0,0,0);
    Vector3D* LEFT = new Vector3D(0,0,0);
    double angularSpeed;

    Camera(double posX = 115.0, double posY = 115.0, double posZ = 50.0) {
        POSITION = new Vector3D(posX, posY, posZ);
        LEFT = new Vector3D(-1 / sqrt(2), -1 / sqrt(2), 0);
        RIGHT = new Vector3D(-1 / sqrt(2), 1 / sqrt(2), 0);
        UP = new Vector3D(0, 0, 1);
        angularSpeed = 1.0 * M_PI / 180;
    }

    void rotateLeft() {
        Vector3D* temp = new Vector3D(0,0,0);
        temp = LEFT->Mul2(LEFT, cos(angularSpeed));
        temp = temp->Add2Vec(temp, UP->Cross2Vec(UP, LEFT->Mul2(LEFT, sin(angularSpeed))));
        LEFT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = RIGHT->Mul2(RIGHT, cos(angularSpeed));
        temp2 = temp2->Add2Vec(temp2, UP->Cross2Vec(UP, RIGHT->Mul2(RIGHT, sin(angularSpeed)))); 
        RIGHT = temp2;

    }

    void rotateRight() {

        Vector3D* temp = new Vector3D(0,0,0);
        temp = LEFT->Mul2(LEFT, cos(-angularSpeed));
        temp = temp->Add2Vec(temp, UP->Cross2Vec(UP, LEFT->Mul2(LEFT, sin(-angularSpeed))));
        LEFT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = RIGHT->Mul2(RIGHT, cos(-angularSpeed));
        temp2 = temp2->Add2Vec(temp2, UP->Cross2Vec(UP, RIGHT->Mul2(RIGHT, sin(-angularSpeed)))); 
        RIGHT = temp2;

    }

    void rotateUp() {
        Vector3D* temp = new Vector3D(0,0,0);
        temp = LEFT->Mul2(LEFT, cos(angularSpeed));
        temp = temp->Add2Vec(temp, RIGHT->Cross2Vec(RIGHT, LEFT->Mul2(LEFT, sin(angularSpeed))));
        LEFT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = UP->Mul2(UP, cos(angularSpeed));
        temp2 = temp2->Add2Vec(temp2, RIGHT->Cross2Vec(RIGHT, UP->Mul2(UP, sin(angularSpeed)))); 
        UP = temp2;
    }

    void rotateDown() {
        Vector3D* temp = new Vector3D(0,0,0);
        temp = LEFT->Mul2(LEFT, cos(-angularSpeed));
        temp = temp->Add2Vec(temp, RIGHT->Cross2Vec(RIGHT, LEFT->Mul2(LEFT, sin(-angularSpeed))));
        LEFT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = UP->Mul2(UP, cos(-angularSpeed));
        temp2 = temp2->Add2Vec(temp2, RIGHT->Cross2Vec(RIGHT, UP->Mul2(UP, sin(-angularSpeed)))); 
        UP = temp2;

    }

    void tiltClock() {

        Vector3D* temp = new Vector3D(0,0,0);
        temp = RIGHT->Mul2(RIGHT, cos(angularSpeed));
        temp = temp->Add2Vec(temp, LEFT->Cross2Vec(LEFT, RIGHT->Mul2(RIGHT, sin(angularSpeed))));
        RIGHT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = UP->Mul2(UP, cos(angularSpeed));
        temp2 = temp2->Add2Vec(temp2, LEFT->Cross2Vec(LEFT, UP->Mul2(UP, sin(angularSpeed)))); 
        UP = temp2;
    }

    void tiltCounterClock() {

        Vector3D* temp = new Vector3D(0,0,0);
        temp = RIGHT->Mul2(RIGHT, cos(-angularSpeed));
        temp = temp->Add2Vec(temp, LEFT->Cross2Vec(LEFT, RIGHT->Mul2(RIGHT, sin(-angularSpeed))));
        RIGHT = temp;

        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = UP->Mul2(UP, cos(-angularSpeed));
        temp2 = temp2->Add2Vec(temp2, LEFT->Cross2Vec(LEFT, UP->Mul2(UP, sin(-angularSpeed)))); 
        UP = temp2;
    }

    void moveForward() {
        POSITION = POSITION->Add2Vec(POSITION,LEFT);
    }

    void moveBackward() {
        POSITION = POSITION->Sub2Vec(POSITION,LEFT);
    }

    void moveLeft() {
        POSITION = POSITION->Sub2Vec(POSITION,RIGHT);
    }

    void moveRight() {
        POSITION = POSITION->Add2Vec(POSITION,RIGHT);
    }

    void moveUp() {
        POSITION = POSITION->Add2Vec(POSITION,UP);
    }

    void moveDown() {
        POSITION = POSITION->Sub2Vec(POSITION,UP);
    }

    void SCENE_UP() {
        POSITION = POSITION->Add2Vec(POSITION, UP->Mul2(UP,0.3));
    }

    void SCENE_DOWN() {
        POSITION = POSITION->Sub2Vec(POSITION, UP->Mul2(UP,0.3));
    }
};


#endif