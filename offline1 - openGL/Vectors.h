#ifndef VECTORS
#define VECTORS

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <cmath>


using namespace std;


class Vectors{
public:
	// x,y,z co-ordinates
	float x;
	float y;
	float z;

	// initialize with x,y,z
	Vectors(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// initialize with vector
	Vectors(Vectors* v){
		this->x = v->x;
		this->y = v->y;
		this->z = v->z;
	}

	/// own operations
	//-----------------------------------------------------------------------------------
	// Modulus
	float Modular(){
		float modValue = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		return modValue;
	}

	// Normalize
	Vectors* normalize() {
        float modValue = Modular();
        this->x /= modValue;
        this->y /= modValue;
        this->z /= modValue;

        return this;
    }
	
	// ADD
	Vectors* Add(Vectors* v){
		this->x += v->x;
		this->y += v->y;
		this->z += v->z; 

		return this;
	}

	// SUB
	Vectors* Sub(Vectors* v){
		this->x -= v->x;
		this->y -= v->y;
		this->z -= v->z; 

		return this;
	}

	// MUL
	Vectors* Mul(float f){
		this->x *= f;
		this->y *= f;
		this->z *= f; 

		return this;	
	}

	// SUB
	Vectors* SubWithMul(Vectors* v){
		this->Add(v->Mul(-1));
		
		return this;
	}

	// DOT
	Vectors* DotProduct(Vectors* v){
		this->x *= v->x;
		this->y *= v->y;
		this->z *= v->z;
		
		return this;
	}

	// CROSS
	Vectors* CrossProduct(Vectors* v){
		float tempX = this->x;
		float tempY = this->y;
		float tempZ = this->z;
		
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
	
	// add two vectors
	Vectors* Add2Vec(Vectors* v1, Vectors* v2){
		Vectors* temp = new Vectors(v1->x, v1->y, v1->z);
		temp->x += v2->x;
		temp->y += v2->y;
		temp->z += v2->z;

		return temp;	
	}
	
	// sub two vectors
	Vectors* Sub2Vec(Vectors* v1, Vectors* v2){
		Vectors* temp = new Vectors(v1->x, v1->y, v1->z);
		temp->x -= v2->x;
		temp->y -= v2->y;
		temp->z -= v2->z;

		return temp;	
	}
	
	// mul with scaler
	Vectors* Mul2(Vectors* v1, float f){
		Vectors* temp = new Vectors(v1->x, v1->y, v1->z);
		temp->x *= f;
		temp->y *= f;
		temp->z *= f;

		return temp;	
	}
	
	// dot two vectors
	Vectors* Dot2Vec(Vectors* v1, Vectors* v2){
		Vectors* temp = new Vectors(v1->x, v1->y, v1->z);
		temp->x *= v2->x;
		temp->y *= v2->y;
		temp->z *= v2->z;

		return temp;
	}

	// cross with two
	Vectors* Cross2Vec(Vectors* v1, Vectors* v2){
		Vectors* temp = new Vectors(v1->x, v1->y, v1->z);
		
		temp->x = v1->y * v2->z - v2->y * v1->z;
		temp->y = v2->x * v1->z - v1->x * v2->z;
		temp->z = v1->x * v2->y - v2->x * v1->y;
		
		return temp;
	}
};

#endif