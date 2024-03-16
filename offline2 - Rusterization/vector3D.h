#ifndef VECTORS
#define VECTORS

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>


using namespace std;


class Vector3D{
public:
	// x,y,z co-ordinates
	double x;
	double y;
	double z;

    // defualt constructor
	Vector3D(){
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}


	// initialize with x,y,z
	Vector3D(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// initialize with vector
	Vector3D(Vector3D* v){
		this->x = v->x;
		this->y = v->y;
		this->z = v->z;
	}

	/// own operations
	//-----------------------------------------------------------------------------------
	// Modulus
	double Modular(){
		double modValue = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		return modValue;
	}

	// Normalize
	Vector3D* normalize() {
        double modValue = Modular();
        this->x /= modValue;
        this->y /= modValue;
        this->z /= modValue;

        return this;
    }
	
	// ADD
	Vector3D* Add(Vector3D* v){
		this->x += v->x;
		this->y += v->y;
		this->z += v->z; 

		return this;
	}

	// SUB
	Vector3D* Sub(Vector3D* v){
		this->x -= v->x;
		this->y -= v->y;
		this->z -= v->z; 

		return this;
	}

	// MUL
	Vector3D* Mul(double f){
		this->x *= f;
		this->y *= f;
		this->z *= f; 

		return this;	
	}

	// SUB
	Vector3D* SubWithMul(Vector3D* v){
		this->Add(v->Mul(-1));
		
		return this;
	}

	// DOT
	Vector3D* DotProduct(Vector3D* v){
		this->x *= v->x;
		this->y *= v->y;
		this->z *= v->z;
		
		return this;
	}

	// CROSS
	Vector3D* CrossProduct(Vector3D* v){
		double tempX = this->x;
		double tempY = this->y;
		double tempZ = this->z;
		
		this->x = tempY * v->z - v->y * tempZ;
		this->y = v->x * tempZ - tempX * v->z;
		this->z = tempX * v->y - v->x * tempY;
		
		return this;
	}

	// PRINT
	void printVector(){
		cout << "x = " << x << " y = " << y << " z = " << z << endl;
	}


	/// others operations
	//-----------------------------------------------------------------------------------
	
	// add two Vector3D
	Vector3D* Add2Vec(Vector3D* v1, Vector3D* v2){
		Vector3D* temp = new Vector3D(v1->x, v1->y, v1->z);
		temp->x += v2->x;
		temp->y += v2->y;
		temp->z += v2->z;

		return temp;	
	}
	
	// sub two Vector3D
	Vector3D* Sub2Vec(Vector3D* v1, Vector3D* v2){
		Vector3D* temp = new Vector3D(v1->x, v1->y, v1->z);
		temp->x -= v2->x;
		temp->y -= v2->y;
		temp->z -= v2->z;

		return temp;	
	}
	
	// mul with scaler
	Vector3D* Mul2(Vector3D* v1, double f){
		Vector3D* temp = new Vector3D(v1->x, v1->y, v1->z);
		temp->x *= f;
		temp->y *= f;
		temp->z *= f;

		return temp;	
	}
	
	// dot two Vector3D  
	double Dot2Vec(Vector3D* v1, Vector3D* v2){
		return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
	}

	// cross with two
	Vector3D* Cross2Vec(Vector3D* v1, Vector3D* v2){
		Vector3D* temp = new Vector3D(v1->x, v1->y, v1->z);
		
		temp->x = v1->y * v2->z - v2->y * v1->z;
		temp->y = v2->x * v1->z - v1->x * v2->z;
		temp->z = v1->x * v2->y - v2->x * v1->y;
		
		return temp;
	}

	Vector3D* applyRodriguesFormula(Vector3D* p, Vector3D* a, double theta){
        // R(p, a, theta) = p * cos(theta) +  a * (a dot p)(1 - cos(theta)) + (a cross p)sin(theta)
		
		Vector3D* temp = new Vector3D(p->x, p->y, p->z);
		temp->Mul(cos(theta));

		Vector3D* temp2 = new Vector3D(a->x, a->y, a->z);
		double k = temp2->Dot2Vec(a,p);
		k = k * (1 - cos(theta));
		temp2->Mul(k);

		temp->Add(temp2);

		Vector3D* temp3 = new Vector3D(a->x, a->y, a->z);
		temp3 = temp3->Cross2Vec(a,p);
		temp3->Mul(sin(theta));

		temp->Add(temp3);

        return temp;
    }
};

#endif