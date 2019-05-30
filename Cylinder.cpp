/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>
#include <iostream>

/**
* Cylinder's intersection method.  The input is a ray (pos, dir).
*/
float Cylinder::intersect(glm::vec3 posn, glm::vec3 dir) {
    float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2 * (dir.x * (posn.x - center.x) + dir.z * (posn.z - center.z));
    float c = (posn.x - center.x) * (posn.x - center.x) + (posn.z - center.z) * (posn.z - center.z) - (radius * radius);

    float root = (b * b) - (4 * a * c);

    if (fabs(root) < 0.001) return -1;

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
    float p = posn.y + (t0 * dir.y);
    if ((p >= center.y) && (p <= center.y + height)) {
        return t0;
    }

    p = posn.y + (t1 * dir.y);
    if ((p >= center.y) && (p <= center.y + height)) {
        return t1;
    }


    return -1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p) {
    glm::vec3 n = glm::vec3((p.x - center.x), 0, (p.z - center.z));
    n = glm::normalize(n);
    return n;
}
