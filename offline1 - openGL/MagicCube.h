#ifndef MAGICCUBE
#define MAGICCUBE

#include "Sphere.h"
#include <iostream>
#include <cmath>
#include <GL/glut.h>

using namespace std;

const double math_PI = 3.14;
const double baseLengthSpeed = 0.01;
const double ANGULAR_SPEED = 3;



class MagicCube {
    
public:

    Vectors* startAxis;
    double angularSpeed, radius, angularOffset, baseLength;

    // constructor
    MagicCube() {
        startAxis = new Vectors(0, -1, 0);
        baseLength = 0;
        angularOffset = 0;
        angularSpeed = ANGULAR_SPEED;
        radius = 1;
    }


    // triangleDraw
    void drawTriangle() {
        glBegin(GL_TRIANGLES); {
            
            float upX,upY,upZ;
            float rightX,rightY,rightZ;
            float leftX,leftY,leftZ;

            upX = 0 + baseLength;
            upY = 0 + baseLength;
            upZ =  1 - 2*baseLength;

            rightX = 0 + baseLength;
            rightY = 1 - 2*baseLength;
            rightZ = 0 + baseLength;

            leftX = 1 - 2*baseLength;
            leftY = 0 + baseLength;
            leftZ = 0 + baseLength;


            glVertex3f(leftX, leftY, leftZ);
            glVertex3f(rightX, rightY, rightZ);
            glVertex3f(upX, upY, upZ);
        } glEnd();
    }


    // sphereHelper
    void drawSphereHelper(double cx, double cy, double cz, double radius, int mode) {
        glPushMatrix();
        Sphere* temp = new Sphere(radius);
        temp->changeCenter(cx,cy,cz - radius);
        temp->taskSetter(3);
        temp->draw(int(mode));
        
        glPopMatrix();
    }


    // ShpereDraw
    void drawSphere(){
        double cF = 3;
        double rF = tan(60 * math_PI / 180.0);

        double centerBuffer = 1-baseLength*cF;
        double radiusBuffer = baseLength*rF;
        
        drawSphereHelper(0, 0, centerBuffer, radiusBuffer, 1);
        drawSphereHelper(0, 0, -centerBuffer, radiusBuffer, 1);
        drawSphereHelper(0, centerBuffer, 0, radiusBuffer, 2);
        drawSphereHelper(0, -centerBuffer, 0, radiusBuffer, 2);
        drawSphereHelper(centerBuffer, 0, 0, radiusBuffer, 3);
        drawSphereHelper(-centerBuffer, 0, 0, radiusBuffer, 3);
    }



    // color
    void Color(string color){
        if (color == "red"){
            glColor3f(1, 0, 0);
        }
        if (color == "green"){
            glColor3f(0, 1, 0);
        }
        if (color == "blue"){
            glColor3f(0, 0, 1);
        }
        if (color == "yellow"){
            glColor3f(1, 1, 0);
        }

        if (color == "black"){
            glColor3f(0, 0, 0);
        }
        if (color == "white"){
            glColor3f(1, 1, 1);
        }

        if(color == "pink"){
            glColor3f(1, 0, 1);
        }

        if(color == "cyan"){
            glColor3f(0, 1, 1);
        }


        
    }


    // cylinderDraw
    void drawCylinder(double r, double h) {
        int cylStack = 100;
        int cylSector = 100;
        
        for (int i = 0; i < cylStack; i++) {
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= cylSector; j++) {
                Color("yellow");
                
                glVertex3f(r * cos(2 * math_PI * j / cylSector),
                    r * sin(2 * math_PI * j / cylSector),
                    h * i / cylStack);


                glVertex3f(r * cos(2 * math_PI * j / cylSector), 
                    r * sin(2 * math_PI * j / cylSector), 
                    h * (i + 1) / cylStack);
            }
            glEnd();
        }
    }

  
    // main Draw
    void draw() {
        glPushMatrix();
        
        glRotatef(angularOffset, startAxis->x, startAxis->y, startAxis->z);

        bool alternate = false;
        int sides = 2;
        int traingles= 4;
        

        // draw the octahedran
        for (int j = 0; j < sides; j++) {
            glPushMatrix();
            glRotatef(j * 180, 1, 0, 0);
            
            for (int i = 0; i < traingles; i++) {
                if (alternate == false) {
                    Color("pink");
                    alternate = true;
                } else {
                    Color("cyan");
                    alternate = false;
                }
                glPushMatrix();

                // base triangle with rotation
                glRotatef(90 * i, 0, 1, 0);
                drawTriangle();

                glPopMatrix();
            }

            glPopMatrix();
        }

        // draw sphere
        drawSphere();

        //draw cylinder
        double cylinderR = baseLength*tan(60/ 180.0 * math_PI);
        double cylinderH = 1.414*(0.99-3*baseLength);

        sides = 8;
        // middle
        glPushMatrix();
        for (int i = 0; i < sides / 2; i++) {
            glPushMatrix();
           
            glRotatef(90 * i + 45, 0, 1, 0);
            
            double halfH = cylinderH / 2.0; 
            glTranslatef(0, 0, -halfH);
            
            glTranslatef(sin(math_PI/4) - 1.2248 * cylinderR, 0, 0);
            drawCylinder(cylinderR, cylinderH);
            
            glPopMatrix();
        }
        glPopMatrix();

        
        // upper
        glPushMatrix();
        for (int i = 0; i < sides / 2; i++) {
            glPushMatrix();
            
            glRotatef(90*i, 0, 1, 0);
            
            glTranslatef(0, 1-3*baseLength, 0);
            glRotatef(45, 1, 0, 0);
            
            drawCylinder(cylinderR, cylinderH);
            
            glPopMatrix();
        }
        glPopMatrix();


        // lower
        glPushMatrix();
        for (int i = 0; i < sides / 2; i++) {
            glPushMatrix();
            glRotatef(90*i, 0, 1, 0);
            
            glTranslatef(0, -1+3*baseLength, 0);
            glRotatef(45, -1, 0, 0);
            
            drawCylinder(cylinderR, cylinderH);
            glPopMatrix();
        }
        glPopMatrix();


        glPopMatrix();

    }


    // sphereOctahedron
    void sphereToOctahedron() {
        baseLength += baseLengthSpeed;

        if (baseLength > 1.0/3) {
            baseLength = 0.33;
        }
    }

    // octahedranShere
    void octahedronToSphere() {
        baseLength -= baseLengthSpeed;
        if (baseLength < 0){
            baseLength = 0;
        }
    }



    // rotateClockWise
    void rotateClockWise() {
        angularOffset += angularSpeed;

        if (angularOffset > 360){
            int temp = (int) angularOffset;
            temp = temp % 360;
            angularOffset = temp;
        }
        
    }

    // rotateCounterClockWise
    void rotateCounterClockWise() {
        angularOffset -= angularSpeed;

        if (angularOffset < -360) {
            int temp = (int) angularOffset;
            temp += 360; 
            angularOffset = temp;
        }
    }


    

  

};

#endif