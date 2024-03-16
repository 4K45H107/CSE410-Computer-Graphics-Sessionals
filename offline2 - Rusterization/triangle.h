#ifndef TRIANGLE
#define TRIANGLE

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

#include "vector3D.h"
using namespace std;

#define INF 1e7



static unsigned long int g_seed = 1;
void seedRandom() {
    g_seed = static_cast<unsigned long int>(std::time(nullptr));
}
inline int randomNum() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}



class Triangle{
public:
    Vector3D* vertices[3];
    int color[3];

    Triangle(){
        Vector3D* temp = new Vector3D();
        for(int i = 0; i < 3; i++){
            vertices[i] = temp;
        }
    }

     Triangle(Vector3D* v1, Vector3D* v2, Vector3D* v3) {
        this->vertices[0] = v1;
        this->vertices[1] = v2;
        this->vertices[2] = v3;

        for(int i = 0; i < 3; i++){
            this->color[i] = randomNum() % 255;
        } 
    }


    Triangle(Triangle* triangle) {
        for(int i = 0; i < 3; i++){
            this->vertices[i] = triangle->vertices[i];
            this->color[i] = triangle->color[i];
        } 
    }


    double getMaxY() {
        double maxY = -INF;
        for (int i = 0; i < 3; i++) {
            maxY = max(vertices[i]->y, maxY);
        }
        return maxY;
    }

   double getMinY() {
        double minY = INF;
        for (int i = 0; i < 3; i++) {
            minY = min(vertices[i]->y, minY);
        }
        return minY;
    }

    double getMaxX() {
        double maxX = -INF;

        for (int i = 0; i < 3; i++) {
            maxX = max(vertices[i]->x, maxX);
        }
        
        return maxX;
    }

   double getMinX() {
        double minX = INF;

        for (int i = 0; i < 3; i++) {
            minX = min(vertices[i]->x, minX);
        }
        return minX;
    }

    
    double getZ(double x, double y) {
        Vector3D* temp1 = new Vector3D(vertices[0]);
        Vector3D* temp2 = new Vector3D(vertices[1]);
        Vector3D* temp3 = new Vector3D(vertices[2]);

        temp1->Sub(temp2);
        temp3->Sub(temp2);

        Vector3D* normal = temp1->Cross2Vec(temp1,temp3);


        double z = normal->x * vertices[0]->x + normal->y * vertices[0]->y + normal->z * vertices[0]->z - normal->x * x - normal->y * y;

        z /= normal->z;

        return  z;
    }

    void printTriangle() {
        vertices[0]->printVector();
        vertices[1]->printVector();
        vertices[2]->printVector();
        cout << endl;
    }


    vector<double> getIntersections(double y) {
        vector<double> intersections;
        
        double x0;
        double x1;
        double x2;

        x0 = ((y - vertices[0]->y) * (vertices[0]->x - vertices[1]->x)) / (vertices[0]->y - vertices[1]->y) + vertices[0]->x; 
        
        x1 = ((y - vertices[2]->y) * (vertices[2]->x - vertices[1]->x)) / (vertices[2]->y - vertices[1]->y) + vertices[2]->x; 

        x2 = ((y - vertices[0]->y) * (vertices[0]->x - vertices[2]->x)) / (vertices[0]->y - vertices[2]->y) + vertices[0]->x; 


        if (x0 >= getMinX()) {
            if(x0 <= getMaxX()){
                intersections.push_back(x0);
            }
        }
        if (x1 >= getMinX()) {
            if(x1 <= getMaxX()){
                intersections.push_back(x1);
            }
        }
        if (x2 >= getMinX()) {
            if(x2 <= getMaxX()){
                intersections.push_back(x2);
            }
        }

        return intersections;
    }

    

};


#endif