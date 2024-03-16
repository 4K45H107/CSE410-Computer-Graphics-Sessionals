#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>
#include <stack>
#include <vector>
#include <string>

#include "matrix.h"
#include "vector3D.h"
#include "triangle.h"
#include "bitmap_image.hpp"

#define Pii 3.1415926535897932384626433832795028841971
#define INF 1e7

// open file
fstream openFile(string filename, ios_base::openmode mode = ios_base::in){
    
    fstream file;
    file.open(filename, mode);
    
    if(!file.is_open()){
        cout << "Error: ";
        cout << filename <<endl;
        exit(1);
    }

    return file;
}


// output in relevant file
void writeTraingle(ostream &os, Matrix* triangle){
    // print 1st 3 element of each row
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3;j++){
            // normalized value
            double temp;
            temp = triangle->matrix4D[j][i]/triangle->matrix4D[3][i];         
            os  << fixed << setprecision(7) << temp;
            if (j < 2) {
                os<<" ";
            }
        }
        os << endl;
    }
    os << endl;
}


// view
Matrix* generateView(Vector3D* eyeVector, Vector3D* lookVector, Vector3D* upVector){
    
    // generate look, righ and up vectors
    Vector3D* look = new Vector3D();
    look = lookVector->Sub2Vec(lookVector,eyeVector);
    look->normalize();

    Vector3D* right  = new Vector3D();
    right = look->Cross2Vec(look,upVector);
    right->normalize();
    
    Vector3D* up = new Vector3D();
    up = right->Cross2Vec(right,look);
    up->normalize();

    
    // move  to origin
    Matrix* translateView = new Matrix();
    translateView = translateView->identity();
    

    translateView->matrix4D[0][3] = -eyeVector->x; 
    translateView->matrix4D[1][3] = -eyeVector->y; 
    translateView->matrix4D[2][3] = -eyeVector->z;

    // align the camera with the world axes
    // l with the -Z axis, r with X axis, and u with Y axis.
    Matrix* rotateView = new Matrix();
    rotateView = rotateView->identity();
    
    rotateView->matrix4D[0][0] = +right->x; 
    rotateView->matrix4D[0][1] = +right->y; 
    rotateView->matrix4D[0][2] = +right->z;
    rotateView->matrix4D[1][0] = +up->x; 
    rotateView->matrix4D[1][1] = +up->y; 
    rotateView->matrix4D[1][2] = +up->z;
    rotateView->matrix4D[2][0] = -look->x; 
    rotateView->matrix4D[2][1] = -look->y; 
    rotateView->matrix4D[2][2] = -look->z;

    // view transformation matrix V = Rotation * Translate
   Matrix* viewTransformation = new Matrix();
   viewTransformation = rotateView->multiply(translateView);

   return viewTransformation;
}




// |rear/r 0                    0                          0            |
// |0      rear/t               0                          0            |
// |0      0       -(far + rear)/(far - rear) -(2*far*rear)/(far - rear)|
// |0      0                   -1                          0            |

// projection
Matrix* generateProjection(double fovY, double asepect, double zNear, double zFar){
    
    // field of view 
    double fovX;
    fovX = fovY * asepect;

    // then determine r and t
    double t = zNear * tan((fovY/2) * (Pii/180));
    double r = zNear * tan((fovX/2) * (Pii/180));

    
    // projection
    Matrix* projection = new Matrix(); 
    projection->matrix4D[2][2] = -(zFar + zNear)/(zFar - zNear);
    projection->matrix4D[2][3] = -(2*zFar*zNear)/(zFar - zNear); 
    projection->matrix4D[0][0] = zNear/r; 
    projection->matrix4D[1][1] = zNear/t; 
    projection->matrix4D[3][2] = -1;
   
    return projection;
}


int main(){
    string fileName = "scene.txt";
    fstream scene = openFile(fileName,ios_base::in);


    Vector3D* eyeVector = new Vector3D();
    Vector3D* lookVector = new Vector3D();;
    Vector3D* upVector = new Vector3D();;

    
    // "scene.txt" input takin
    
    // glutLookAt
    // line-1 eyeVector vector
    scene >> eyeVector->x;
    scene >> eyeVector->y;
    scene >> eyeVector->z;


    // line-2 lookVector vector
    scene >> lookVector->x;
    scene >> lookVector->y;
    scene >> lookVector->z;

    // line-4 upVector vector
    scene >> upVector->x;
    scene >> upVector->y;
    scene >> upVector->z;

 
    // glutPerspective
    double fovY;
    double aspect;
    double zNear;
    double zFar;

    scene >> fovY;
    scene >> aspect;
    scene >> zNear;
    scene >> zFar;

    // output file
    string fileS1 = "stage1.txt";
    string fileS2 = "stage2.txt";
    string fileS3 = "stage3.txt";

    ofstream stage1(fileS1);
    ofstream stage2(fileS2);
    ofstream stage3(fileS3);


    // matrices
    Matrix* M = new Matrix();
    Matrix* identityMatrix = new Matrix();
    Matrix* viewMatrix = new Matrix();
    Matrix* projectionMatrix = new Matrix();


    // stack 
    stack <Matrix*> stackOfMatrices;

    
    identityMatrix = identityMatrix->identity();
    viewMatrix = generateView(eyeVector,lookVector,upVector);
    projectionMatrix = generateProjection(fovY,aspect,zNear,zFar);

    // initially 
    M = identityMatrix;
    
    string command;
    while(true){

        scene >> command;

        if(command == "triangle"){
            
            Matrix* temp = new Matrix();

            // |x1 x2 x3 1|
            // |y1 y2 y3 1|
            // |z1 z2 z3 1|
            // | 1  1  1 1|

            // read three points
            scene >> temp->matrix4D[0][0];
            scene >> temp->matrix4D[1][0];
            scene >> temp->matrix4D[2][0];
            scene >> temp->matrix4D[0][1];
            scene >> temp->matrix4D[1][1];
            scene >> temp->matrix4D[2][1];
            scene >> temp->matrix4D[0][2];
            scene >> temp->matrix4D[1][2];
            scene >> temp->matrix4D[2][2];

            for (int i = 0; i < 4; i++) {
                temp->matrix4D[i][3] = 1;
            }

            for (int i = 0; i < 4; i++) {
                temp->matrix4D[3][i] = 1;
            }
            
            
            // Stage 1
            temp = M->multiply(temp);
            writeTraingle(stage1, temp);
            
            // Stage 2
            temp = viewMatrix->multiply(temp);
            writeTraingle(stage2, temp);

           
            // Stage 3
            temp = projectionMatrix->multiply(temp);
            writeTraingle(stage3, temp);


        }

        else if(command == "translate"){
            
            Matrix* tanslateMatrix = new Matrix();
            tanslateMatrix = tanslateMatrix->identity();

            scene >> tanslateMatrix->matrix4D[0][3];
            scene >> tanslateMatrix->matrix4D[1][3];
            scene >> tanslateMatrix->matrix4D[2][3];

            M = M->multiply(tanslateMatrix);
        }
        
        else if(command == "scale"){

            Matrix* scaleMatrix = new Matrix();
            scaleMatrix = scaleMatrix->identity();
            
            scene >> scaleMatrix->matrix4D[0][0];
            scene >> scaleMatrix->matrix4D[1][1];
            scene >> scaleMatrix->matrix4D[2][2];

            M = M->multiply(scaleMatrix);
        }
        
        else if(command == "rotate"){
          
            double rotateAngle; 
            Vector3D* roateAxis = new Vector3D();
            
            scene >> rotateAngle;
            scene >> roateAxis->x;
            scene >> roateAxis->y;
            scene >> roateAxis->z;
            
            

            rotateAngle = rotateAngle * Pii / 180.0;

            
            Vector3D* i = new Vector3D(1,0,0);
            Vector3D* j = new Vector3D(0, 1, 0);
            Vector3D* k = new Vector3D(0, 0, 1);
            
           
            roateAxis->normalize();


            Vector3D* c1 = new Vector3D(); 
            Vector3D* c2 = new Vector3D();
            Vector3D* c3 = new Vector3D();

            c1 = i->applyRodriguesFormula(i, roateAxis, rotateAngle);
            c2 = j->applyRodriguesFormula(j, roateAxis, rotateAngle);
            c3 = k->applyRodriguesFormula(k, roateAxis, rotateAngle);

           
            // then use those to generate the rotation matrix 
            Matrix* rotator = new Matrix();
            rotator = rotator->identity();

            rotator->matrix4D[0][0] = c1->x;
            rotator->matrix4D[0][1] = c2->x; 
            rotator->matrix4D[0][2] = c3->x;
            rotator->matrix4D[1][0] = c1->y; 
            rotator->matrix4D[1][1] = c2->y; 
            rotator->matrix4D[1][2] = c3->y;
            rotator->matrix4D[2][0] = c1->z; 
            rotator->matrix4D[2][1] = c2->z; 
            rotator->matrix4D[2][2] = c3->z;

            
            M = M->multiply(rotator);

        }
        else if(command == "push"){
            stackOfMatrices.push(M);
        }

        else if(command == "pop"){
            M = stackOfMatrices.top();
            stackOfMatrices.pop();
        }

        else if(command == "end"){
            break;
        }

    }

    stage1.close();
    stage2.close();
    stage3.close();


    // Vector3D* t1 = new Vector3D(0,0,0);
    // Vector3D* t2 = new Vector3D(4,0,-1);
    // Vector3D* t3 = new Vector3D(0,4,3);

    // Triangle* t = new Triangle(t1,t2,t3);
    //cout << t->getMaxX() << t->getMinX() << t->getMaxY() << t->getMinY();
    // for(auto x : t->getIntersections(2)) {
    //     cout << x << endl;
    // }


    fstream stage3i = openFile(fileS3,ios_base::in);
    fstream config = openFile("config.txt",ios_base::in);
    fstream zBufferO = openFile("z_buffer.txt",ios_base::out);


    int screenWidth;
    int screenHeight;

    config >> screenWidth;
    config >> screenHeight;

    double** zBuffer;
    int*** colorBuffer;

    zBuffer = new double*[screenHeight];
    colorBuffer = new int**[screenHeight];


    for(int i = 0; i < screenHeight; i++){
        zBuffer[i] = new double[screenWidth];
        for (int j = 0; j < screenWidth; j++){
            zBuffer[i][j] = INF;
        }
    }

    for (int i = 0; i < screenHeight; i++) {
        colorBuffer[i] = new int*[screenWidth];
        for (int j = 0; j < screenWidth; j++) {
            colorBuffer[i][j] = new int[3];
            for (int k = 0; k < 3; k++) {
                colorBuffer[i][j][k] = 0;
            }
        }
    }

    vector<Triangle*> triangleList;

    while(!stage3i.eof()){
        vector<Vector3D*> vertexList;
        for(int i = 0; i < 3; i++){
            Vector3D* temp = new Vector3D();
            
            stage3i >> temp->x;
            stage3i >> temp->y;
            stage3i >> temp->z;

            vertexList.push_back(temp);
        }

        Triangle* temp2 = new Triangle(vertexList[0],vertexList[1],vertexList[2]);
        triangleList.push_back(temp2);
        vertexList.clear();
    }

    triangleList.pop_back();

    // for (auto t : triangleList){
    //     t->printTriangle();
    // }

    double top;
    double bottom;
    double right;
    double left;

    double dx; 
    double dy;

    double topY;
    double bottomY;
    double leftX;
    double rightX;

    top = 1.0;
    bottom = -1.0;
    right = 1.0;
    left = -1.0;

    dx = (right - left) / (screenWidth*1.0); 
    dy = (top - bottom) / (screenHeight*1.0); 

    topY = top - dy/2.0;
    bottomY = bottom + dy/2.0;
    leftX = left + dx/2.0;
    rightX = right - dx/2.0;

    

    for (auto triangle: triangleList) {
        
        // ROW
        int topScan;
        int bottomScan;

        topScan = 0;
        bottomScan = screenHeight - 1;
        
        
        
        // CLIPPING
        if (triangle->getMaxY() < topY) {
            double temp = triangle->getMaxY();
            temp =  topY - temp;
            temp /= dy;
            temp = round(temp);

            topScan = temp;
        }

        if (triangle->getMinY() > bottomY) {
            double temp = triangle->getMinY();
            temp =  topY - temp;
            temp /= dy;
            temp = round(temp);

            bottomScan = temp;
        }
            
        
        for (int i = topScan; i < bottomScan; i++) {
            
            // COLLUMN
            int leftScan;
            int rightScan;
            float y;

            leftScan = 0;
            rightScan = screenWidth - 1;
            y = topY - dy * i;

            vector<double> intersections;
            intersections = triangle->getIntersections(y);

            if (intersections.size() == 2) {
                double minIntX;
                double maxIntX;

                if (intersections[0] < intersections[1]) {
                    minIntX = intersections[0];
                    maxIntX = intersections[1];
                }
                else {
                    minIntX = intersections[1];
                    maxIntX = intersections[0];
                }


                int potentialLeft;
                int potentialRight;

                potentialLeft = round((minIntX - leftX) / dx);
                potentialRight = round((maxIntX - leftX) / dx);

                leftScan = max(potentialLeft,leftScan);
                rightScan = min(potentialRight, rightScan);

            } else if (intersections.size() == 1) {
                int tempCol = round((intersections[0] - leftX) / dx);
                leftScan = max(tempCol, leftScan);
                rightScan = min(rightScan, tempCol);
            }

            for (int j = leftScan; j < rightScan; j++) {
                
                float x = leftX + dx * j;
                float z = triangle->getZ(x, y);

                
                if (zBuffer[i][j] > z && z > -1.0 && z < 1.0) {
                    zBuffer[i][j] = z;
                    colorBuffer[i][j][0] = triangle->color[0];
                    colorBuffer[i][j][1] = triangle->color[2];
                    colorBuffer[i][j][2] = triangle->color[3];
                }
            }
        }
    }


    bitmap_image image(screenWidth, screenHeight);

    for (int y = 0; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            if (zBuffer[x][y] < 2) {
                zBufferO << zBuffer[x][y];
                zBufferO << "\t";
            }

            image.set_pixel(y, x, (unsigned char)colorBuffer[x][y][0], (unsigned char)colorBuffer[x][y][1], (unsigned char)colorBuffer[x][y][2]);
        }
        zBufferO << endl;
    }

    image.save_image("out.bmp");

    // FREE MEMORY
    for (int i = 0; i < screenHeight; i++) {
        for (int j = 0; j < screenHeight; j++) {
            free(colorBuffer[i][j]);
        }
        free(colorBuffer[i]);
        free(zBuffer[i]);
    }
    
    free(colorBuffer);
    free(zBuffer);



    stage3i.close();
    config.close();
    zBufferO.close();

    return 0;





} 