/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer 
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include <GL/glut.h>
#include "TextureBMP/TextureBMP.h"

using namespace std;

const float WIDTH = 20.0;
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX = WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX = HEIGHT * 0.5;

vector<TextureBMP*> textures;

vector<SceneObject *> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step) {
    glm::vec3 backgroundCol(0);

    glm::vec3 light(10, 40, -3);
    glm::vec3 lightTwo(-40.0, 50, -3.0);

    glm::vec3 ambientCol(0.2);   //Ambient color of light

    ray.closestPt(sceneObjects);        //Compute the closest point of intersetion of objects with the ray


    if (ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour
    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour

    glm::vec3 viewVector = -ray.dir;
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);

    glm::vec3 lightVector = light - ray.xpt;
    glm::vec3 lightVectorTwo = lightTwo - ray.xpt;

    lightVector = glm::normalize(lightVector);
    lightVectorTwo = glm::normalize(lightVectorTwo);


    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
    glm::vec3 reflVectorTwo = glm::reflect(-lightVectorTwo, normalVector);

    glm::vec3 colorSum(0);

    float lDotn = glm::dot(lightVector, normalVector);
    float lDotnTwo = glm::dot(lightVectorTwo, normalVector);
    float rDotv = glm::dot(reflVector, viewVector);

    // Texture the floor
    if (ray.xindex == 4) {
        float a1 = -50;
        float a2 = 50;
        float b1 = -50;
        float b2 = -200;
        float texcoords = (ray.xpt.x-a1)/(a2-a1);
        float texcoordt = (ray.xpt.z-b1)/(b2-b1);
        materialCol = textures[0]->getColorAt(texcoords, texcoordt);
    }

    // Texture for sphere:
    if (ray.xindex == 1) {
        glm::vec3 center = glm::vec3(14, 10, -90);
        glm::vec3 N = glm::normalize(ray.xpt - center);
        float s = 0.45 + atan2(N.z, N.x) / (2 * M_PI);
        float t = 0.65 + asin(N.y) / M_PI;
        materialCol = 0.7f * textures[1]->getColorAt(s, t);
    }

    // Texture another sphere
    if (ray.xindex == 2) {
        glm::vec3 center = glm::vec3(5.0, 5, -80);
        glm::vec3 N = glm::normalize(ray.xpt - center);
        float s = 0.65 - atan2(N.z, N.x) / (2 * M_PI);
        float t = 0.5 + asin(N.y) / M_PI;
        materialCol = 0.7f * textures[1]->getColorAt(s, t);
    }

    // Making a crate...
    if (ray.xindex == 5) {
        float a1 = 12;
        float a2 = 17;
        float b1 = -20;
        float b2 = -15;
        float texcoords = (ray.xpt.x-a1)/(a2-a1);
        float texcoordt = (ray.xpt.y-b1)/(b2-b1);
        materialCol = textures[2]->getColorAt(texcoords, texcoordt);

        b1 = -90;
        b2 = -95;

        texcoords = (ray.xpt.x-a1)/(a2-a1);
        texcoordt = (ray.xpt.z-b1)/(b2-b1);
        materialCol += textures[2]->getColorAt(texcoords, texcoordt);

    }

    // Procedural texture for cone
    if (ray.xindex == 7) {
        int function = int(tan(ray.xpt.x) * sin(ray.xpt.y));
        if (function % 2 == 0) {
            materialCol = glm::vec3(1, 0, 0);
        } else if (function % 2 == 1) {
            materialCol = glm::vec3(0, 0, 1);
        } else {
            materialCol = glm::vec3(0, 1, 0);
        }
    }

    //Procedural texture for cylinder
    if (ray.xindex == 6) {
        int function = int(ray.xpt.x - ray.xpt.y);
        if (function % 2 == 0) {
            materialCol = glm::vec3(0.26, 0.88, 0.81);
        } else {
            materialCol = glm::vec3(1, 0.65, 0);
        }
    }

    // Reflections
    float specular;
    if (rDotv < 0) {
        specular = 0;
    } else {
        specular = pow(rDotv, 5);
    }

    // Shadows
    Ray shadow(ray.xpt, lightVector);
    shadow.closestPt(sceneObjects);

    Ray shadowTwo(ray.xpt, lightVectorTwo);
    shadowTwo.closestPt(sceneObjects);

    // Computing the shadows
    if (lDotn <= 0 || ((shadow.xindex > -1) && (shadow.xdist < ray.xdist))) {
        colorSum += ambientCol * materialCol;
    } else {
        colorSum += ambientCol * materialCol + lDotn * materialCol + rDotv * specular;
    }

    if (lDotnTwo <= 0 || ((shadowTwo.xindex > -1) && (shadowTwo.xdist < ray.xdist))) {
        colorSum += ambientCol * materialCol;
    } else {
        colorSum += ambientCol * materialCol + lDotnTwo * materialCol;
    }

    // Compute the Reflection
    if (ray.xindex == 0 && step < MAX_STEPS) {
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
        Ray reflectedRay(ray.xpt, reflectedDir);
        glm::vec3 reflectedCol = trace(reflectedRay, step + 1); //Recursion!

        colorSum = colorSum + (0.8f * reflectedCol);
    }

    // Refraction
    if (ray.xindex == 3 && step < MAX_STEPS) {
        float eta = 1.01;
        glm::vec3 g = glm::refract(ray.dir, normalVector, 1.0f/eta);
        Ray refrRay(ray.xpt, g);
        refrRay.closestPt(sceneObjects);
        glm::vec3 m = sceneObjects[refrRay.xindex]->normal(refrRay.xpt);
        glm::vec3 h = glm::refract(g, -m, eta);
        Ray refrRay2(refrRay.xpt, h);

        colorSum += trace(refrRay2, step + 1);
    }


    return colorSum;
}


glm::vec3 antiAliasing(glm::vec3 eye, float pixel, float xp, float yp) {
    glm::vec3 colorSum(0);

    double quarterPixel = pixel * 0.25;
    double halfPixel = pixel * 0.5;
    double centerX = xp + quarterPixel;
    double centerY = yp + quarterPixel;

    Ray ray = Ray(eye, glm::vec3(centerX, centerY, -EDIST));
    ray.normalize();
    colorSum += trace(ray, 1);

    ray = Ray(eye, glm::vec3(centerX + halfPixel, centerY, -EDIST));
    ray.normalize();
    colorSum += trace(ray, 1);

    ray = Ray(eye, glm::vec3(centerX, centerY + halfPixel, -EDIST));
    ray.normalize();
    colorSum += trace(ray, 1);

    ray = Ray(eye, glm::vec3(centerX + halfPixel, centerY + halfPixel, -EDIST));
    ray.normalize();
    colorSum += trace(ray, 1);

    // Average the color
    colorSum *= glm::vec3(0.25);


    return colorSum;
}


//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display() {
    float xp, yp;  //grid point
    float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
    float cellY = (YMAX - YMIN) / NUMDIV;  //cell height

    glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a quad.

    for (int i = 0; i < NUMDIV; i++)    //For each grid point xp, yp
    {
        xp = XMIN + i * cellX;
        for (int j = 0; j < NUMDIV; j++) {
            yp = YMIN + j * cellY;

            glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);    //direction of the primary ray

            Ray ray = Ray(eye, dir);        //Create a ray originating from the camera in the direction 'dir'
            ray.normalize();                //Normalize the direction of the ray to a unit vector

            glm::vec3 col = antiAliasing(eye, cellX, xp, yp);
//            glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);                //Draw each cell with its color value
            glVertex2f(xp + cellX, yp);
            glVertex2f(xp + cellX, yp + cellY);
            glVertex2f(xp, yp + cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    //-- Create a pointer to a sphere object
    Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -5.0, -140.0), 15.0, glm::vec3(0, 0, 1));
    Sphere *sphere2 = new Sphere(glm::vec3(14.0, 10.0, -90.0), 5.0, glm::vec3(0, 1, 0));
    Sphere *sphere3 = new Sphere(glm::vec3(5.0, 5.0, -80.0), 3, glm::vec3(0, 0, 0));
    Sphere *sphere4 = new Sphere(glm::vec3(5.0, -15.0, -80.0), 4, glm::vec3(0, 0, 0));

    Plane *plane = new Plane(glm::vec3(-50, -20, -50),    //Point A
                             glm::vec3(50, -20, -50),     //Point B
                             glm::vec3(50, -20, -200),    //Point C
                             glm::vec3(-50, -20, -200),   //Point D
                             glm::vec3(0, 0, 0));      //Colour


    glClearColor(0, 0, 0, 1);

    Cube *cube = new Cube(glm::vec3(12, -15, -90), glm::vec3(17, -20, -95), glm::vec3(0, 0, 0));

    Cylinder *cylinder = new Cylinder(glm::vec3(-5, -15, -80), 2, 3, glm::vec3(1, 1, 1));


    textures.push_back(new TextureBMP((char *)"assets/wood.bmp"));
    textures.push_back(new TextureBMP((char *)"assets/R&C.bmp"));
    textures.push_back(new TextureBMP((char *)"assets/crate.bmp"));


    Cone *cone = new Cone(glm::vec3(-15, -10, -85), 5, 10, glm::vec3(0, 0, 0));

    //--Add the above to the list of scene objects.
    sceneObjects.push_back(sphere1);
    sceneObjects.push_back(sphere2);
    sceneObjects.push_back(sphere3);
    sceneObjects.push_back(sphere4);
    sceneObjects.push_back(plane);
    sceneObjects.push_back(cube);
    sceneObjects.push_back(cylinder);
    sceneObjects.push_back(cone);
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
