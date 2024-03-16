#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>
#include "Vectors.h"
#include "Camera.h"
#include "Sphere.h"
#include <GL/glut.h>

using namespace std;

Camera* camera;
Sphere* ball;
int border = 25;
int mode;

void init() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

	camera = new Camera();
	ball = new Sphere(0.2);
	mode = 0;
}


void drawAxes(){
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);{
		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);

		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}glEnd();
}

void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();
}


void drawFloor() {
    float tileLength = 0.2;

	int windowLength, windowWidth;
	
	windowLength = 200;
	windowWidth = 200;

    int length = windowLength;
	int width = windowLength;
    
	

    // W,H - ODD
	if (length % 2 == 0){
		length = length + 1;
	}
	if (width % 2 == 0){
		width = width + 1;
	}

	bool isLastTile = false;

	// DRAW - BOARD
    glPushMatrix();
	
	float xChange;
	if (length % 2 == 1){
		xChange = -(length - 1) * tileLength;
	}
	else{
		xChange = -length * tileLength;
	}


	float yChange;
	if (width % 2 == 1){
		yChange = -(width - 1) * tileLength;
	}
	else{
		yChange = -width * tileLength;
	}
	
    glTranslatef(xChange, yChange, 0);
    
	for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            if (isLastTile) {
				// if last black draw white
                glColor3f(1, 1, 1);
                isLastTile = false;
            } else {
				// if last white draw black
                glColor3f(0, 0, 0);
                isLastTile = true;
            }

            glPushMatrix();
			// tile length 2a
            glTranslatef(2 * j * tileLength, 2 * i * tileLength, 0);
            drawSquare(tileLength);
            glPopMatrix();
        }
    }

    glPopMatrix();


	int borderLength, borderWidth;
	borderLength= border, borderWidth = border;
    
	glPushMatrix();
	
	if (borderLength % 2 == 1){
		xChange = -(borderLength - 1) * tileLength;
	}
	else{
		xChange = -borderLength * tileLength;
	}


	if (borderWidth % 2 == 1){
		yChange = -(borderWidth - 1) * tileLength;
	}
	else{
		yChange = -borderWidth * tileLength;
	}

    glTranslatef(xChange, yChange, 0);
	
    for (int i = 0; i < borderWidth; i++) {
        for (int j = 0; j < borderWidth;) {

            glColor3f(1, 0, 0);
            glPushMatrix();
            glTranslatef(2*j*tileLength, 2*i*tileLength, 0.001);
            drawSquare(tileLength);
            glPopMatrix();
            
			if (j == borderWidth - 1){
				break;
			}


            if (i == 0){
				j++;
			} 
			else if(i == borderWidth - 1) {
				j++;
			}
            else {
				j = borderWidth - 1;
        	}
    	}
	}
    glPopMatrix();
}





void display() {

	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef(camera->offsetX,camera->offsetY,camera->offsetZ);
	glRotatef(camera->rotateX, 1, 0, 0);
	glRotatef(camera->rotateY, 0, 1, 0);

	Vectors* front = camera->TARGET->Sub2Vec(camera->TARGET, camera->CAMERA_POSITION);
	glRotatef(camera->tiltZ, front->x, front->y, front->z);
	
	gluLookAt(
        camera->CAMERA_POSITION->x, camera->CAMERA_POSITION->y, camera->CAMERA_POSITION->z, 
        camera->TARGET->x, camera->TARGET->y, camera->TARGET->z, 
        camera->UP->x, camera->UP->y, camera->UP->z
        );


	drawFloor();
	glPushMatrix();
	//drawAxes();
	ball->draw();
	glPopMatrix();
	
	
	glutSwapBuffers();
	
}

void keyboardListener(unsigned char key, int x, int y) {
	switch(key){
        case '1':
            camera->rotateRight();
			break;
		case '2':
            camera->rotateLeft();
			break;
		case '3':
            camera->rotateUp();
			break;
		case '4':
            camera->rotateDown();
			break;
		case '5':
            camera->tiltClock();
			break;
		case '6':
            camera->tiltCounterClock();
			break;
		case 'i':
            ball->goForward();
			break;
		case 'k':
            ball->goBackward();
			break;
		case 'j':
            ball->rotateClock();
			break;
		case 'l':
            ball->rotateCounterClock();
			break;
		case ' ':
            if (mode == 0){
				mode = 1;
			}
			else {
				mode = 0;
			}
            break;
		case 'w':
            camera->upFix();
			break;

		case 's':
            camera->downFix();
			break;
		case 'd':
            camera->rightFix();
			break;

		case 'a':
            camera->leftFix();
			break;
        default:
            printf("ulta palta key press kora bondho koren!\n");
            break;
    }

}

void specialKeyListener(int key, int y, int z) {
	switch(key){
        case GLUT_KEY_UP:
            camera->moveForward();
			break;
		case GLUT_KEY_DOWN:
            camera->moveBackward();
			break;
		case GLUT_KEY_RIGHT:
            camera->moveRight();
			break;
		case GLUT_KEY_LEFT:
            camera->moveLeft();
			break;
		case GLUT_KEY_PAGE_UP:
            camera->moveUp();
			break;
		case GLUT_KEY_PAGE_DOWN:
            camera->moveDown();
			break;
        default:
            printf("ulta palta key press kora bondho koren!\n");
            break;
    }
	
}

void idle(){
    glutPostRedisplay();
}

void timerHandler(int value) {
	if (mode == 1){
		ball->goForward();
	}
    glutTimerFunc(100, timerHandler, 0);
}



int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 2");


	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutIdleFunc(idle);
	glutDisplayFunc(display);	//display callback function

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	
	glutTimerFunc(200, timerHandler, 0);
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
