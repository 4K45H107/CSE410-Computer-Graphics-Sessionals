#ifndef SPHERE
#define SPHERE

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "Vectors.h"
#include <cmath>
#include <GL/glut.h>


using namespace std;

const float MY_PI = 3.14;
int sectors = 150;
int stacks = 150; 


class Sphere {

public:
    int task;
    float radius;
    Vectors* direction;
    Vectors* position;
    Vectors* rotationAxis;
    Vectors* up;
    int tiltZ = 0;

    int angularSpeed = 40;
    int rotationOffset = 0;

    // CONSTRUCTOR
    Sphere(float radius = 0.5) {
        this->radius = radius;
        position = new Vectors(0,0,0);
        direction = new Vectors(0.5,0,0);
        rotationAxis = new Vectors(0,0.5,0);
        up = new Vectors(0,0,0.5);
        task = 2;
    }

    void changeCenter(float x, float y, float z){
        this->position->x = x;
        this->position->y = y;
        this->position->z = z;
    }




    void demoDraw(int mode = 0) {

        glTranslatef(position->x,position->y,position->z);
        // glRotatef(this->tiltZ, 0, 0, 1);
        
        glTranslatef(0, 0, radius);
        
        glPushMatrix();
        glRotatef(rotationOffset*angularSpeed, rotationAxis->x,rotationAxis->y,rotationAxis->z);

        for (int i = 0; i < stacks; i++) {
            if (task == 2){
                sectors = 10;
                stacks = 10;
            }
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= sectors; j++) {
                if (task == 2){
                    if (i < stacks/2) {
                        if (j % 2) {
                            glColor3f(1, 0, 0);
                        } else {
                            glColor3f(0, 1, 0);
                        }
                    } else {
                        if (j % 2) {
                            glColor3f(0, 1, 0);
                        } else {
                            glColor3f(1, 0, 0);
                        }
                    }
                }

                else if (task == 3){
                    if (mode == 1){
                        glColor3f(1, 0, 0);
                    }
                    if (mode == 2){
                        glColor3f(0, 1, 0);
                    }
                    if (mode == 3){
                        glColor3f(0, 0, 1);
                    }
                }

               
                glVertex3f(radius * sin(i * MY_PI / stacks) * cos(2 * j * MY_PI / sectors),
                        radius * sin(i * MY_PI / stacks) * sin(2 * j * MY_PI / sectors), 
                        radius * cos(i * MY_PI / stacks));

                glVertex3f(radius * sin((i+1) * MY_PI / stacks) * cos(2 * j * MY_PI / sectors),
                     radius * sin((i+1) * MY_PI / stacks) * sin(2 * j * MY_PI / sectors),
                     radius * cos((i+1) * MY_PI / stacks));
            }
            glEnd();
        }
        
        glPopMatrix();
        if (task == 2){
            printAxes();
        }

    }

    void taskSetter(int task){
        this->task = task;
    }

    void draw(int mode = 0) {
        demoDraw(mode);
    }


    void goForward(){
        demoDraw();
        rotationOffset = (rotationOffset + 1) % (360 / angularSpeed);
        float linearStep = 1.0 * (angularSpeed / 180.0) * MY_PI * radius;
        
        position = position->Add2Vec(position, direction->Mul2(direction, linearStep));
        // position->x += linearStep;

        reflect();

    }

     void goBackward(){
        rotationOffset = (rotationOffset - 1) % (360 / angularSpeed);
        float linearStep = 1.0 * (angularSpeed / 180.0) * MY_PI * radius;
        position = position->Add2Vec(position, direction->Mul2(direction, -linearStep));

        reflect();

    }

    void rotateClock(){
        direction = direction->Add2Vec(rotationAxis->Mul2(rotationAxis, 0.1),direction);
        rotationAxis = up->Cross2Vec(up, direction)->normalize();
    }
    
    void rotateCounterClock(){
        direction = direction->Add2Vec(rotationAxis->Mul2(rotationAxis, -0.1),direction);
        rotationAxis = up->Cross2Vec(up, direction)->normalize();
    }


    void reflect() {
        float wall = 24 * 0.2;
        if (position->x + radius >= (wall-0.15)) {
            direction->x = -direction->x;
            rotationAxis->Mul(-1.0);
        }
        else if (position->y + radius >= wall){
            direction->y = -direction->y;
            rotationAxis->Mul(-1.0);
        }
        if (position->x - radius <= -(wall-0.15)){
            direction->x = -direction->x;
        }
        else if (position->y - radius <= -wall){
            direction->y = -direction->y;
        }
    }



    void printAxes(){
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);{
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0,0,0);
            glVertex3f(direction->x,direction->y,direction->z);

            // glColor3f(1.0, 0.0, 0.0);
            // glVertex3f(0,0,0);
            // glVertex3f(rotationAxis->x, rotationAxis->y, rotationAxis->x);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0,0,0);
            glVertex3f(up->x,up->y,up->z);
        }glEnd();
    }

    void printVector(){
        cout << "up -> ";
        up->printVector();
        cout << "position -> ";
        position->printVector();
    }


};

#endif