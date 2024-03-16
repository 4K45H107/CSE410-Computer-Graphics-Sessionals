#ifndef CLASSES
#define CLASSES

#include <iostream>
#include <cmath>
#include <vector>
#include "vector3D.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include <GL/glut.h>

int recurssion;
const double MY_PI = 3.14;
int sectors = 150;
int stacks = 150; 

Color* clr;



#define MAXX 10000000
#define NEG -1

#define ERROR_HANDLER 1e-5
#define AMBIENT_INDX 0
#define DIFFUSE_INDX 1
#define SPECULAR_INDX 2
#define REFLECTION_INDX 3


class Object {

public:
    Vector3D* referencePoint;
    Color* color; 
    
    double height;
    double width;
    double length;

    
    double coeff_ADSR[4];
    int shine; 

    Object() {
        referencePoint = new Vector3D(0,0,0);
        color = new Color();
        clr = new Color();
    }

    void setColor(double red, double green, double blue) {
        Color* temp = new Color(red,green, blue);
        color = temp;
    }

    void setShine(int shine) {
        this->shine = shine;
    }  


    void setAmbient(double ambient){
         coeff_ADSR[0] = ambient;
    }
    
    void setDiffuse(double diffuse){
         coeff_ADSR[1] = diffuse;
    }

    void setSpecular(double specular){
         coeff_ADSR[2] = specular;
    }

    void setReflection(double reflection){
         coeff_ADSR[3] = reflection;
    }

    void setCoefficients(double ambient, double diffuse, double specular, double reflection) {
       setAmbient(ambient);
       setDiffuse(diffuse);
       setSpecular(specular);
       setReflection(reflection);
    }

    virtual void draw() = 0;

    virtual Color* getColorAtPoint(Vector3D* point) = 0;

    virtual Vector3D* getNormalAtPoint(Ray* ray, Vector3D* xPoint) = 0;

    virtual double getIntersection(Ray* ray) = 0;

    double intersect(Ray* ray, Color &color, int level);

    // getColor by png model
};



double determinant(double matrix[3][3]){
    double v1 = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    double v2 = matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
    double v3 = matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return v1 - v2 + v3;
}

class Sphere : public Object {

public:
    Sphere(Vector3D* center, double radius) {
        referencePoint = center;
        length = radius;
    }

    void draw() {
        double radius; 

        radius = this->length;
        stacks = 60;
        sectors = 60;

        
        glPushMatrix();
        glTranslatef(referencePoint->x, referencePoint->y, referencePoint->z);
        
        for (int i = 0; i < stacks; i++) {

			glColor3f(color->red, color->green, color->blue);

            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= sectors; j++) {

                glVertex3f(
                    radius * sin(i * MY_PI / stacks) * cos(2 * j * MY_PI / sectors),
                    radius * sin(i * MY_PI / stacks) * sin(2 * j * MY_PI / sectors),
                    radius * cos(i * MY_PI / stacks)
                 );

            
                glVertex3f(
                    radius * sin((i+1) * MY_PI / stacks) * cos(2 * j * MY_PI / sectors), 
                    radius * sin((i+1) * MY_PI / stacks) * sin(2 * j * MY_PI / sectors),  
                    radius * cos((i+1) * MY_PI / stacks));
            }

            glEnd();
        }

        glPopMatrix();
    }

    Color* getColorAtPoint(Vector3D* point) {
        Color* temp = new Color();
        temp = this->color;
        return temp;
    }

    Vector3D* getNormalAtPoint(Ray* ray, Vector3D* xPoint) {
        
        Vector3D* temp = new Vector3D(0,0,0);
        temp = temp->Sub2Vec(xPoint, referencePoint);
        temp = temp->normalize();

        // in/out
        Vector3D* temp2 = new Vector3D(0,0,0);
        temp2 = temp2->Sub2Vec(ray->start, referencePoint);
        double dist = temp2->Modular();
        
        
        if (dist < length) 
        {
            temp = temp->Mul2(temp, -1);
        }

        return temp;

    }

    
    double getIntersection(Ray* ray) {

        double a = ray->direction->Dot2Vec(ray->direction, ray->direction);
        double b = 2 * ray->direction->Dot2Vec(ray->direction, ray->start->Sub2Vec(ray->start, referencePoint));
        double c;

        Vector3D* temp = new Vector3D(0,0,0);
        temp = ray->start->Sub2Vec(ray->start, referencePoint);

        c = temp->DotProduct(ray->start->Sub2Vec(ray->start, referencePoint));
        c -= length * length;
        
        double rootCheck;
        rootCheck = b*b - 4*a*c;

        if (rootCheck < 0) {
            // imaginary - no intersection
            // cout << "imaginary point" << endl;
            return -1.0;
        }

        rootCheck = sqrt(rootCheck);
        
        // solution of quadratic equation
        double t1, t2;
        
        t1 = (-b - rootCheck) / (2*a);
        t2 = (-b + rootCheck) / (2*a);
        
        if (t1 < 0 && t2 < 0) {
            return -1.0;
        }
        
        if (t1 > 0) {
            return t1;
        }
        
        if (t2 > 0) {
            return t2;
        }

        return -1.0;
    }

};

class Triangle : public Object {

public:

    Vector3D* A = new Vector3D(0,0,0);
    Vector3D* B = new Vector3D(0,0,0);
    Vector3D* C = new Vector3D(0,0,0);

    Triangle(){
        this->A = new Vector3D(0,0,0);
        this->B = new Vector3D(0,0,0);
        this->C = new Vector3D(0,0,0);
    }

    Triangle(Vector3D* A, Vector3D* B, Vector3D* C) {
        this->A = A;
        this->B = B;
        this->C = C;
    }

    void draw() {
        glPushMatrix();
        glColor3f(color->red, color->green, color->blue);
        
        glBegin(GL_TRIANGLES); {
            glVertex3f(A->x, A->y, A->z);
            glVertex3f(B->x, B->y, B->z);
            glVertex3f(C->x, C->y, C->z);
        }
        
        glEnd();
        glPopMatrix();
    }

    Color* getColorAtPoint(Vector3D* point) {
        Color* tempColor = new Color();
        tempColor = this->color;
        return tempColor;
    }



    Vector3D* getNormalAtPoint(Ray* ray, Vector3D* xPoint) 
    {
        Vector3D* normal = new Vector3D(0,0,0);

        Vector3D* temp1 = new Vector3D(0,0,0);
        Vector3D* temp2 = new Vector3D(0,0,0);
        temp1 = temp1->Sub2Vec(this->A, this->B);
        temp2 = temp2->Sub2Vec(this->C, this->B);

        normal = normal->Cross2Vec(temp1, temp2);
        normal = normal->normalize();

        // in/out
        double check = ray->direction->Dot2Vec(ray->direction, normal);
        if (check > 0) {
            normal = normal->Mul2(normal, -1);
        }

        return normal;

    }



    double getIntersection(Ray* ray) {
        double betaMatrix[3][3] = {
            {A->x - ray->start->x, A->x - C->x, ray->direction->x},
            {A->y - ray->start->y, A->y - C->y, ray->direction->y},
            {A->z - ray->start->z, A->z - C->z, ray->direction->z}
        };
        double gammaMatrix[3][3] = {
            {A->x - B->x, A->x - ray->start->x, ray->direction->x},
            {A->y - B->y, A->y - ray->start->y, ray->direction->y},
            {A->z - B->z, A->z - ray->start->z, ray->direction->z}
        };
        double tMatrix[3][3] = {
            {A->x - B->x, A->x - C->x, A->x - ray->start->x},
            {A->y - B->y, A->y - C->y, A->y - ray->start->y},
            {A->z - B->z, A->z - C->z, A->z - ray->start->z}
        };
        double areaMatrix[3][3] {
            {A->x - B->x, A->x - C->x, ray->direction->x},
            {A->y - B->y, A->y - C->y, ray->direction->y},
            {A->z - B->z, A->z - C->z, ray->direction->z}
        };

        double AreaDet, beta, gamma, t;
        AreaDet = determinant(areaMatrix);
        beta = determinant(betaMatrix) / AreaDet;
        gamma = determinant(gammaMatrix) / AreaDet;
        t = determinant(tMatrix) / AreaDet;
        
        if (t > 0){
            if (beta + gamma < 1 && beta > 0 && gamma > 0){
                return t;
            }
            else {
                return -1;
            }
        } 
        
        return -1;
    }
};


class General : public Object {

public:

    double coeff[10];

    Vector3D* ccRef = new Vector3D(0,0,0);

    General(double coeff[10],
        Vector3D* ccRef,
        double length, double width, double height    
    ) {
        for(int i = 0; i < 10; i++){
            this->coeff[i] = coeff[i];
        }
        this->ccRef = ccRef;
        this->length = length;
        this->width = width;
        this->height = height;
    }

    bool check(Vector3D* point) {
        double lengthCheck, widthCheck, heightCheck;
        lengthCheck = fabs(length);
        widthCheck = fabs(width);
        heightCheck = fabs(height);


        if(lengthCheck > 1e-5){
            if(point->x < referencePoint->x) {
                return false;
            }
            if(point->x > referencePoint->x + length) {
                return false;
            }
        }
        

        if(widthCheck > 1e-5){
            if(point->y < referencePoint->y) {
                return false;
            }
            if(point->y > referencePoint->y + width) {
                return false;
            }
        }
        

        if(heightCheck > 1e-5){
            if(point->z < referencePoint->z) {
                return false;
            }
            if(point->z > referencePoint->z + height) {
                return false;
            }
        }
    
        return true;
    }

    void draw() {}

    Color* getColorAtPoint(Vector3D* point) {
        return this->color;
    }

    Vector3D* getNormalAtPoint(Ray* ray, Vector3D* xPoint) {
      
        Vector3D* point = new Vector3D(0,0,0);
        point = xPoint;

        double A, B, C, D, E, F, G, H, I, J;
        A = coeff[0];
        B = coeff[1];
        C = coeff[2];
        D = coeff[3];
        E = coeff[4];
        F = coeff[5];
        G = coeff[6];
        H = coeff[7];
        I = coeff[8];
        J = coeff[9];

        double x, y, z;
        x = 2*A*point->x + D*point->y + E*point->z + G;
        y = 2*B*point->y + D*point->x + F*point->z + H;
        z = 2*C*point->z + E*point->x + F*point->y + I;


        Vector3D* normal = new Vector3D(x,y,z);
        normal = normal->normalize();


        Vector3D* temp = new Vector3D(0,0,0);
        temp = temp->Sub2Vec(ray->start, referencePoint);

        // in/out
        double check = temp->Dot2Vec(temp, normal);
        if (check > 0) {
            normal = normal->Mul2(normal, -1);
        }

        return normal;
    }

    double getIntersection(Ray* ray) {

       double A, B, C, D, E, F, G, H, I, J;
        A = coeff[0];
        B = coeff[1];
        C = coeff[2];
        D = coeff[3];
        E = coeff[4];
        F = coeff[5];
        G = coeff[6];
        H = coeff[7];
        I = coeff[8];
        J = coeff[9];

        double X0, Y0, Z0;
        double X1, Y1, Z1;

        X0 = ray->start->x;
        Y0 = ray->start->y;
        Z0 = ray->start->z;

        X1 = ray->direction->x;
        Y1 = ray->direction->y;
        Z1 = ray->direction->z;

        double C1, C2, C0;

        C0 = A * X1 * X1 + B * Y1 * Y1 + C * Z1 * Z1 + D * X1 * Y1 + E * X1 * Z1 + F * Y1 * Z1;
        C1 = 2 * A * X0 * X1 + 2 * B * Y0 * Y1 + 2 * C * Z0 * Z1 + D * (X0 * Y1 + X1 * Y0) + E * (X0 * Z1 + X1 * Z0) + F * (Y0 * Z1 + Y1 * Z0) + G * X1 + H * Y1 + I * Z1;
        C2 = A * X0 * X0 + B * Y0 * Y0 + C * Z0 * Z0 + D * X0 * Y0 + E * X0 * Z0 + F * Y0 * Z0 + G * X0 + H * Y0 + I * Z0 + J;


        double rootCheck = C1*C1 - 4*C0*C2;
        
        if(rootCheck < 0) {
            //cout << "imaginary point" << endl;
            return -1.0;
        }
        if(fabs(C0) < 1e-5) 
        {
            return -C2/C1;
        }

        double t1 = (-C1 - sqrt(rootCheck))/(2*C0);
        double t2 = (-C1 + sqrt(rootCheck))/(2*C0);
        
        if (t1 <= 0 && t2 <= 0){
            return -1.0;
        }
        
        if(t1 > 0) {
            Vector3D* xPoint = new Vector3D(0,0,0);
            xPoint = ray->start->Add2Vec(ray->start, ray->direction->Mul2(ray->direction, t1));
            if(check(xPoint)){
                return t1;
            }
        }

        if (t2 > 0) {
            Vector3D* xPoint = new Vector3D(0,0,0);
            xPoint = ray->start->Add2Vec(ray->start, ray->direction->Mul2(ray->direction, t2));
            if(check(xPoint)){
                return t2;
            }
        }

        return -1.0;
    }
};


class Floor : public Object {

public:
    double floorW, tileW;
    Color* tileBlack = new Color(0, 0, 0);
    Color* tileWhite = new Color(0, 0, 0);

    Floor(double floorW, double tileW) {
        this->floorW = floorW;
        this->tileW = tileW;
        this->referencePoint = new Vector3D(-floorW / 2, -floorW / 2, 0);
        this->length = floorW;

        this->tileBlack = new Color(1, 1, 1);
        this->tileWhite = new Color(0, 0, 0);
    }

    void draw() {
        glPushMatrix();
        glTranslatef(this->referencePoint->x, this->referencePoint->y, this->referencePoint->z);
        
        int numberOfTiles = floorW / tileW;


        for(int i = 0; i < numberOfTiles; i++) {
            for(int j = 0; j < numberOfTiles; j++) {

                int index = i + j;
                int flag = index % 2;

                if(!flag) {
                    glColor3f(tileBlack->red, tileBlack->green, tileBlack->blue);
                } 

                if(flag) {
                    glColor3f(tileWhite->red, tileWhite->green, tileWhite->blue);
                }

                int currI = i, currJ = j;
                int nextI= i + 1, nextJ = j + 1;

                glBegin(GL_QUADS); {
                    glVertex3f(currI * tileW, currJ * tileW, 0);
                    glVertex3f(currI * tileW, nextJ * tileW, 0);
                    glVertex3f(nextI * tileW, nextJ * tileW, 0);
                    glVertex3f(nextI * tileW, currJ * tileW, 0);
                } glEnd();
            }
        }
        glPopMatrix();
    }

    Color* getColorAtPoint(Vector3D* p) {
        
        int tileX, tileY;
        tileX = (p->x - referencePoint->x) / tileW;
        tileY = (p->y - referencePoint->y) / tileW;

        if((tileX + tileY) % 2 == 0) {
            return tileBlack;
        }
        if((tileX + tileY) % 2 == 1) {
            return tileWhite;
        }

        return tileWhite;
    }

    Vector3D* getNormalAtPoint(Ray* ray, Vector3D* xPoint) {

        Vector3D* normal = new Vector3D(0,0,1);

        // in/out
        if (ray->start->z < 0) 
        {
            normal = normal->Mul2(normal, -1);
        }

        return normal;
    }

    double getIntersection(Ray* ray) {

        Vector3D* normal = new Vector3D(0, 0, 1);
        if (ray->direction->Dot2Vec(ray->direction, normal) == 0) {
            return -1.0;
        }

        double t = -ray->start->Dot2Vec(ray->start, normal) / ray->direction->Dot2Vec(ray->direction, normal);
        Vector3D* p = new Vector3D(0,0,0);
        p = ray->pointAtT(t);

        if (p->x < referencePoint->x || p->x > referencePoint->x + floorW || p->y < referencePoint->y || p->y > referencePoint->y + floorW) {
            return -1.0;
        }

        return t;
    }
};

void drawSphere(double length, Vector3D* center, Color* color) {
    double radius = length; 
    stacks = 20;
    sectors = 20;

    
    glPushMatrix();
    glTranslatef(center->x, center->y, center->z);
    
    for (int i = 0; i < stacks; i++) {

        glColor3f(color->red, color->green, color->blue);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= sectors; j++) {

            glVertex3f(
                radius * sin(i * MY_PI / stacks) * cos(2 * j * MY_PI / sectors),
                radius * sin(i * MY_PI / stacks) * sin(2 * j * MY_PI / sectors),
                radius * cos(i * MY_PI / stacks)
                );

        
            glVertex3f(
                radius * sin((i+1) * MY_PI / stacks) * cos(2 * j * MY_PI / sectors), 
                radius * sin((i+1) * MY_PI / stacks) * sin(2 * j * MY_PI / sectors),  
                radius * cos((i+1) * MY_PI / stacks));
        }

        glEnd();
    }

    glPopMatrix();
}

class PointLight {

public:
    Vector3D* posXn = new Vector3D(0,0,0);
    Color* color = new Color();

    PointLight() {}

    PointLight(
        double posX, double posY, double posZ, 
        double red, double green, double blue
    ) {
        this->posXn = new Vector3D(posX, posY, posZ);
        this->color = new Color(red, green, blue);
    }
    void draw() {
        drawSphere(0.5, posXn, color);
    }

    
};

class SpotLight {

public:
    PointLight* pointLight = new PointLight();
    Vector3D* dirXn;
    double cutAngle;

    SpotLight(
        double posX, double posY, double posZ, 
        double dirX, double dirY, double dirZ, 
        double red, double green, double blue, 
        double angle
    ) {
        this->pointLight = new PointLight(
            posX, posY, posZ, 
            red, green, blue
        );
        this->dirXn = new Vector3D(dirX, dirY, dirZ);
        this->cutAngle = angle;
        }

    void draw() {
        pointLight->draw();
    }
};


vector<Object *> objectList;
vector<PointLight *> pointLightList;
vector<SpotLight *> spotLightList;


double Object::intersect(Ray* ray, Color &color, int level) {
    double t;
    t = getIntersection(ray);
    
    if (t < 0) {
        return t;
    }

    if(level == 0) {
        return t;
    }



    Vector3D* xPoint = new Vector3D(0,0,0);
    xPoint = ray->pointAtT(t);
    Color* intersectionColor = new Color();
    intersectionColor = getColorAtPoint(xPoint);

    // update color with ambience
    Color* tmp = new Color();
    tmp = getColorAtPoint(xPoint)->Mul2(coeff_ADSR[AMBIENT_INDX]);
    color.red = tmp->red;
    color.green = tmp->green;
    color.blue = tmp->blue;


    for (auto pointLight: pointLightList) {

        bool obscured; 
        
        Vector3D* lightDirection = new Vector3D(0,0,0);
        lightDirection = lightDirection->Sub2Vec(xPoint, pointLight->posXn);
        lightDirection = lightDirection->normalize();
        
        
        Ray* pointLightRay = new Ray(pointLight->posXn, lightDirection);
        Vector3D* normal = new Vector3D(0,0,0);
        normal = getNormalAtPoint(pointLightRay, xPoint);


        obscured = false;

        Vector3D* temp = new Vector3D(0,0,0);
        temp = temp->Sub2Vec(xPoint, pointLight->posXn);
        double t2 = temp->Modular();

        if (t2 < ERROR_HANDLER) {
            continue;
        }


        for (auto object: objectList) {
            double t3 = object->getIntersection(pointLightRay);
            if (t3 > ERROR_HANDLER && t3 < t2 - ERROR_HANDLER) {
                obscured = true;
                break;
            }
        }

        if (!obscured) {
            Vector3D* temp = new Vector3D(0,0,0);
            temp = temp->Mul2(lightDirection, -1);

            double lambert = max(0.0,  temp->Dot2Vec(temp, normal));

            temp = temp->Mul2(normal, 2.0*lambert);
            temp = temp->Add2Vec(temp, lightDirection);
            temp = temp->normalize();

            Ray* reflectingRay = new Ray(xPoint, temp);

            Vector3D* temp2 = new Vector3D(0,0,0);
            temp2 = temp2->Mul2(ray->direction, -1);
            double dotVal = temp2->Dot2Vec(reflectingRay->direction,temp2);

            double phong = max(0.0, pow(dotVal, shine));

            Color* tmp = new Color(color.red, color.green, color.blue);
            

            tmp = tmp->Add2Color(pointLight->color->Mul2(coeff_ADSR[DIFFUSE_INDX] * lambert)->Cross2Color(intersectionColor));
            tmp = tmp->Add2Color(pointLight->color->Mul2(coeff_ADSR[SPECULAR_INDX] * phong));

            color.red = tmp->red;
            color.green = tmp->green;
            color.blue = tmp->blue;

            
        }

    }

    for (auto spotLight: spotLightList) {

        bool obscured;

        Vector3D* lightDirection = new Vector3D(0,0,0);
        lightDirection = lightDirection->Sub2Vec(xPoint, spotLight->pointLight->posXn);
        lightDirection = lightDirection->normalize();


        double angle = acos(lightDirection->Dot2Vec(lightDirection,spotLight->dirXn)/(lightDirection->Modular()*spotLight->dirXn->Modular())) * (180.0 / MY_PI);
        
        if (fabs(angle) > spotLight->cutAngle) {
            continue;
        }
        
        Ray* spotLightRay = new Ray(spotLight->pointLight->posXn, lightDirection);
        Vector3D* normal = new Vector3D(0,0,0);
        normal = getNormalAtPoint(spotLightRay, xPoint);
        
        Vector3D* temp = new Vector3D(0,0,0);
        double val = temp->Dot2Vec(lightDirection, normal);
        val = val*2.0;
        temp = temp->Mul2(normal, val);
        temp = temp->Sub2Vec(lightDirection, temp);
        temp = temp->normalize();

        Ray* reflectingRay = new Ray(xPoint, temp);

        
        
        obscured = false;
        
        temp = new Vector3D(0,0,0);
        temp = temp->Sub2Vec(xPoint, spotLight->pointLight->posXn);
        double t2 = temp->Modular();

        if (t2 < ERROR_HANDLER) {
            continue;
        }


        for (auto object: objectList) {
            double t3 = object->getIntersection(spotLightRay);
            if (t3 > ERROR_HANDLER && t3 < t2 - ERROR_HANDLER) {
                obscured = true;
                break;
            }
        }

        if (!obscured) {
            Vector3D* temp = new Vector3D(0,0,0);
            temp = temp->Mul2(lightDirection, -1);

            double lambert = max(0.0,  temp->Dot2Vec(normal, temp));

            temp = temp->Mul2(ray->direction, -1);
            double dotVal = temp->Dot2Vec(reflectingRay->direction,temp);

            double phong = max(0.0, pow(dotVal, shine));

            Color* tmp = new Color(color.red, color.green, color.blue);

            tmp = tmp->Add2Color(spotLight->pointLight->color->Mul2(coeff_ADSR[DIFFUSE_INDX] * lambert)->Cross2Color(intersectionColor));
            tmp = tmp->Add2Color(spotLight->pointLight->color->Mul2(coeff_ADSR[SPECULAR_INDX] * phong));

            color.red = tmp->red;
            color.green = tmp->green;
            color.blue = tmp->blue;
        }
    }

    if (level < recurssion) {
        Vector3D* normal = new Vector3D(0,0,0);
        normal = getNormalAtPoint(ray, xPoint);
        normal = normal->normalize();

        Vector3D* temp = new Vector3D(0,0,0);
        double val = temp->Dot2Vec(ray->direction, normal);
        val = val*2.0;
        temp = temp->Mul2(normal, val);
        temp = temp->Sub2Vec(ray->direction, temp);
        temp = temp->normalize();

        Ray* reflectingRay = new Ray(xPoint, temp);
        

        // self xSect prevent
        reflectingRay->start = reflectingRay->pointAtT(ERROR_HANDLER);

        int closest = NEG;
        
        double t_intX;
        double tMin = MAXX;

        for (int k = 0; k < objectList.size(); k++) {

            t_intX = objectList[k]->intersect(reflectingRay, color, 0);
            if (t_intX > 0) {

                if (t_intX - tMin < 0) {
                    tMin = t_intX;
                    closest = k;
                }
                
            }
        }

        if (closest != NEG  && closest >= 0 && closest < objectList.size()) {
            Color temp = Color();
            tMin = objectList[closest]->intersect(reflectingRay, temp, ++level);

            Color* tmp = new Color(color.red, color.green, color.blue);

            tmp = tmp->Add2Color(temp.Mul2(coeff_ADSR[REFLECTION_INDX]));
            
            color.red = tmp->red;
            color.green = tmp->green;
            color.blue = tmp->blue;
        }
        
    }

    //clr = color;
    
    return t;
}

#endif