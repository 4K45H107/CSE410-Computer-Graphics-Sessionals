#ifndef MATRIX
#define MATRIX

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>

using namespace std;

class Matrix {
public:
    double matrix4D[4][4];

    // default constructor
    Matrix(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                matrix4D[i][j] = 0;
            }
        }
    }

    // initialize with value
    Matrix(int value, int n = 4){
        for(int i = 0;i < n; i++){
            for(int j = 0;j < n; j++){
                matrix4D[i][j] = value;
            }
        }
    }

    //initialize with another matrix
    Matrix(Matrix* matrix, int n = 4){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                matrix4D[i][j] = matrix->matrix4D[i][j];
            }
        }
    }


    // identity helper function
    void makeIndentity(int n = 4){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if (i == j) {
                    matrix4D[i][j] = 1;
                }
                else {
                    matrix4D[i][j] = 0;
                }
            }
        }
    }


    // create identity matrix
    static Matrix* identity(int n = 4){
        Matrix* m = new Matrix();
        m->makeIndentity(n);
        return m;
    }

    // multiphy two matrix
    Matrix* multiply(Matrix* m, int n = 4){
        Matrix* temp = new Matrix();
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    temp->matrix4D[i][j] += this->matrix4D[i][k] * m->matrix4D[k][j];
                }
            }
        }
        return temp;
    }

    
    // print matrix
    void printMatrix(int n = 4){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << matrix4D[i][j] << " ";
            }
            cout << endl;
        }
    }

};

#endif

