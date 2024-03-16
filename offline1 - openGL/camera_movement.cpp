#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>
#include "Vectors.h"
#include "Camera.h"
#include <GL/glut.h>

using namespace std;

Camera* camera;

void init() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

	camera = new Camera();
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


void drawCube(float a){

	for(int i = 0; i < 4; i++){
        glPushMatrix();
        glRotatef(i * 90, 1, 0, 0);
        glTranslatef(0, 0, a);
        glColor3f(1, 0, 0);
        drawSquare(a);
        glPopMatrix();
    }
	
	glRotatef(90, 0, 1, 0);

	for(int i = 0; i < 4; i++){
        glPushMatrix();
		float buffer = 0.001;
        glRotatef(i * 90, 0, 1, 0);
        glTranslatef(0, 0, a + buffer);
        glColor3f(0, 1, 0);
        drawSquare(a);
        glPopMatrix();
    }

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


	drawAxes();
	drawCube(0.25);
	
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



int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 1");


	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutIdleFunc(idle);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
