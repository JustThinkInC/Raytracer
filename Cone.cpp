/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>
#include <iostream>

/**
* Cylinder's intersection method.  The input is a ray (pos, dir).
*/
float Cone::intersect(glm::vec3 pos, glm::vec3 dir) {
    //http://lousodrome.net/blog/light/2017/01/03/intersection-of-a-ray-and-a-cone/
    //https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html
    //http://www.ctralie.com/PrincetonUGRAD/Projects/COS426/Assignment3/part1.html#raycone
    //http://www.ctralie.com/PrincetonUGRAD/Projects/COS426/Assignment3/part1.html#raycone

    float const A = pos.x - center.x;
    float const B = pos.z - center.z;
    float const C = height - pos.y + center.y;
    float const K = (radius / height) * (radius / height);

    float a = (dir.x * dir.x) + (dir.z * dir.z) - (dir.y * dir.y * K);
    float b = 2 * ((A * dir.x) + (B * dir.z) + (K * C * dir.y));
    float c = (A * A) + (B * B) - (K * C * C);

    float root = (b * b) - (4 * a * c);
    if (fabs(root) < 0.001) return -1;
    if (root < 0.0) return -1;

    float t0 = (-b - sqrt(root)) / (2 * a);
    float t1 = (-b + sqrt(root)) / (2 * a);


    if (t0 < 0.01) t0 = -1;
    if (t1 < 0.01) t1 = -1;

    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // P = P0 + td;
    float p = pos.y + (t0 * dir.y);
    if ((p >= center.y) && (p <= center.y + height)) {
        return t0;
    }

    p = pos.y + (t1 * dir.y);
    if ((p >= center.y) && (p <= center.y + height)) {
        return t1;
    }


    return -1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cone::normal(glm::vec3 p) {
    //https://stackoverflow.com/questions/13792861/surface-normal-to-a-cone

    glm::vec3 n = glm::vec3((p.x - center.x) * height / radius, radius / height,
                            (p.z - center.z) * height / radius);
    n = glm::normalize(n);
    return n;
}
