#include <iostream>
#include <cmath>

#include "1905107_classes.hpp"
#include "Ray.hpp"
#include "bitmap_image.hpp"



using namespace std;

#define WIDTH_MAX 500
#define HEIGHT_MAX 500
#define ANGLE_OF_VIEW 80.0


extern vector<Object*> objectList;
extern vector<PointLight*> pointLights;

#define MAXX 10000000
#define NEG -1


Camera* camera;

int imageNo = 11;
int imageW;
int imageH;

extern int recurssion;

extern vector<SpotLight*> spotLightList;


void drawBackground(bitmap_image image){
    // set the bnackground for capturing 
     for (int i = 0; i < imageH; i++) {
        for (int j = 0; j < imageW; j++) {
            image.set_pixel(i, j, 0, 0, 0);
       }
    }
}


void drawObjects() {
    for(Object *object: objectList){
        object->draw();
    }

    for (auto pl: pointLightList) pl->draw();
    for (auto sl: spotLightList) sl->draw();
}

void viewTransformation(){
    double eyeX, eyeY, eyeZ;
    double centerX, centerY, centerZ;
    double upX, upY, upZ;

    eyeX = camera->POSITION->x;
    eyeY = camera->POSITION->y;
    eyeZ = camera->POSITION->z;

    centerX = camera->POSITION->x + camera->LEFT->x;
    centerY = camera->POSITION->y + camera->LEFT->y;
    centerZ = camera->POSITION->z + camera->LEFT->z;

    upX = camera->UP->x;
    upY = camera->UP->y;
    upZ = camera->UP->z;


    gluLookAt(
        eyeX, eyeY, eyeZ,
        centerX, centerY, centerZ,
        upX, upY, upZ
    );
}

void display() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    viewTransformation();
    drawObjects();

    glutSwapBuffers();

}

void animate() {
    glutPostRedisplay();
}



void capture() {
    
    bitmap_image image(imageW, imageH);
    drawBackground(image);
    
    double planeDistance;
    double du, dv;


    planeDistance = (HEIGHT_MAX / 2.0) / tan((ANGLE_OF_VIEW / 2.0) * (M_PI / 180.0) );

    du = WIDTH_MAX / (double) imageW;
    dv = HEIGHT_MAX / (double) imageH;


    Vector3D* topLeft = new Vector3D(0,0,0);
    topLeft = camera->POSITION->Add2Vec(camera->POSITION, camera->LEFT->Mul2(camera->LEFT, planeDistance));
    topLeft = topLeft->Sub2Vec(topLeft, camera->RIGHT->Mul2(camera->RIGHT, (WIDTH_MAX / 2)));
    topLeft = topLeft->Add2Vec(topLeft, camera->UP->Mul2(camera->UP, (HEIGHT_MAX / 2)));


    Vector3D* temp = new Vector3D(0,0,0);
    temp = topLeft->Add2Vec(topLeft, camera->RIGHT->Mul2(camera->RIGHT, (du / 2)));
    topLeft = temp->Sub2Vec(temp, camera->UP->Mul2(camera->UP, (dv / 2)));
  

    
    double t_int, tMin;

    Color color(0,0,0);

    for (int i = 0; i < imageW; i++) {
        for (int j = 0; j < imageH; j++) {

            // cout << i << " " << j << endl;
            
            Vector3D* pixel = new Vector3D(0,0,0);
            pixel = topLeft->Add2Vec(topLeft, camera->RIGHT->Mul2(camera->RIGHT, i* du));
            pixel = pixel->Sub2Vec(pixel, camera->UP->Mul2(camera->UP, j* dv));

            Vector3D* dir = new Vector3D(0,0,0);
            dir = dir->Sub2Vec(pixel, camera->POSITION)->normalize();
            Ray* ray = new Ray(camera->POSITION, dir);

           

            int closestObject = NEG;
            tMin = MAXX;

            for (int k = 0; k < objectList.size(); k++) {

                // intersection point
                t_int = objectList[k]->intersect(ray, color, 0);
                
                if (t_int > 0) {

                    if (t_int - tMin < 0){
                        
                        // close than previous closest object
                        tMin = t_int;
                        closestObject = k;
                    }
                    
                }
            }

            if (closestObject != NEG && closestObject >= 0 && closestObject < objectList.size()) 
            {
                // closest object intersection point
                tMin = objectList[closestObject]->intersect(ray, color, 1);
                
                Vector3D* xPoint = new Vector3D(0,0,0);
                xPoint = ray->pointAtT(tMin);
                
                //color = clr;
                
                double red, green, blue;
                red = color.red*255;
                green = color.green*255;
                blue = color.blue*255;

                // cout << red << " " << green << " " << blue << endl;
                 
                image.set_pixel(i, j, red, green, blue);
            }
        }
    }
    
    image.save_image("Output_" + to_string(imageNo) + ".bmp");

}

void keyboardHandler(unsigned char key, int mouseX, int mouseY) {
    switch(key) {
        case '1':
            camera->rotateLeft();
            break;
        case '2':
            camera->rotateRight();
            break;
        case '3':
            camera->rotateUp();
            break;
        case '4':
            camera->rotateDown();
            break;
        case '5':
            camera->tiltClock();
        case '6':
            camera->tiltCounterClock();
            break;
        case 'w':
            camera->SCENE_UP();
            break;
        case 's':
            camera->SCENE_DOWN();
            break;
        case '0':
            capture();
            imageNo++;
            break;

        default:
            cout << "ulta palta press kora bondho koren" << endl;
            break;
    }
}

void specialKeyHandler(int key, int mouseX, int mouseY) {

    switch(key) {
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
            cout << "ulta palta press kora bondho koren" << endl;
            break;
        
    }
}

void init() {

    camera = new Camera();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 1000);
}

void loadData() {
    ifstream in("scene.txt");
    Object* inputObject;

    in >> recurssion;
    in >> imageH;
    imageW = imageH;

    int objectCount;
    in >> objectCount;

   


    
    

    for (int i = 0; i < objectCount; i++) {
        string type;
        in >> type;

        if (type == "sphere") {
            
            double cx, cy, cz;
            in >> cx;
            in >> cy;
            in >> cz;

            Vector3D* temp = new Vector3D(cx, cy, cz);
            
            double radius;
            in >> radius;

            inputObject = new Sphere(temp, radius);
        } 
        else if (type == "triangle") 
        {
            double ax, ay, az;
            in >> ax;
            in >> ay;
            in >> az;
            Vector3D* tempA = new Vector3D(ax, ay, az);
            
            double bx, by, bz;
            in >> bx;
            in >> by;
            in >> bz;
            Vector3D* tempB = new Vector3D(bx, by, bz);

            double cx, cy, cz;
            in >> cx;
            in >> cy;
            in >> cz;
            Vector3D* tempC = new Vector3D(cx, cy, cz);
            
            inputObject = new Triangle(tempA, tempB, tempC);
        } 
        else if (type == "general") 
        {
            double a, b, c, d, e, f, g, h, i, j;
            in >> a;
            in >> b;
            in >> c;
            in >> d;
            in >> e;
            in >> f;
            in >> g;
            in >> h;
            in >> i;
            in >> j;

            
            double ccRefX, ccRefY, ccRefZ;
            in >> ccRefX;
            in >> ccRefY;
            in >> ccRefZ;
            
            Vector3D* ccRef = new Vector3D(ccRefX, ccRefY, ccRefZ);
            
            double length, width, height;
            in >> length;
            in >> width;
            in >> height;
            
            double coeff[10];
            coeff[0] = a;
            coeff[1] = b;
            coeff[2] = c;
            coeff[3] = d;
            coeff[4] = e;
            coeff[5] = f;
            coeff[6] = g;
            coeff[7] = h;
            coeff[8] = i;
            coeff[9] = j;


            inputObject = new General(coeff, ccRef, length, width, height); 
        }

        double red, green, blue;
        in >> red;
        in >> green;
        in >> blue;
        
        double ambient, diffuse, specular, reflection;
        in >> ambient;
        in >> diffuse;
        in >> specular;
        in >> reflection;
        
        int shine;
        in >> shine;
        
        inputObject->setColor(red, green, blue);
        inputObject->setCoefficients(ambient, diffuse, specular, reflection);
        inputObject->setShine(shine);

        objectList.push_back(inputObject);
    }

    //cout << objectList.size() << endl;
    
    int pointLightCount;
    in >> pointLightCount;

    for (int i = 0; i < pointLightCount; i++) 
    {
        double x;
        double y;
        double z;
        double red;
        double green;
        double blue;


        in >> x;
        in >> y;
        in >> z;
        in >> red;
        in >> green;
        in >> blue;
        pointLightList.push_back(
            new PointLight(
                x, y, z, 
                red, green, blue
            )
        );
    }

    int spotLightCount;
    in >> spotLightCount;

    for (int i = 0; i < spotLightCount; i++) {
        double x;
        double y;
        double z;
        double red;
        double green;
        double blue;
        double dx, dy, dz;
        double angle;

        in >> x;
        in >> y;
        in >> z;

        in >>  dx >> dy >> dz;

        in >> red;
        in >> green;
        in >> blue;


        in >> angle;

        spotLightList.push_back(
            new SpotLight(
                x, y, z, 
                dx, dy, dz, 
                red, green, blue, 
                angle
            )
        );
    }

    inputObject = new Floor(500, 20);

    double tempCoeff = 0.4;
    double tempShine = 10;
    

    inputObject->setShine(tempShine);
    inputObject->setCoefficients(tempCoeff, tempCoeff/2, tempCoeff/2, tempCoeff/2);
    
    objectList.push_back(inputObject);
}



int main(int argc, char** argv) {

    loadData();
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH_MAX, HEIGHT_MAX);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Offline-3");


    init();

    glutDisplayFunc(display);
    glutIdleFunc(animate);

    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);

    glutMainLoop();

    return 0;
}

