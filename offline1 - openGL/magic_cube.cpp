#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>
#include "Camera.h"
#include "MagicCube.h"
#include <GL/glut.h>

using namespace std;

Camera* camera;;
MagicCube* magicCube;

void init() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

	camera = new Camera();
    magicCube = new MagicCube();

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

	magicCube->draw();
	
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
            magicCube->rotateClockWise();
			break;

		case 'a':
            magicCube->rotateCounterClockWise();
			break;
        case ',':
            magicCube->sphereToOctahedron();
			break;
        case '.':
            magicCube->octahedronToSphere();
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
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");


	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutIdleFunc(idle);
	glutDisplayFunc(display);	//display callback function

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
