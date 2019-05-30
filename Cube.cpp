/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cube class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cube.h"
#include <math.h>
#include <iostream>


template<typename T>
int SIGN(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
* Cube's intersection method.  The input is a ray (pos, dir).
*/
float Cube::intersect(glm::vec3 posn, glm::vec3 dir) {
    float tx1, tx2, ty1, ty2, tz1, tz2, tNear = -1 * INFINITY, tFar = INFINITY;
    tx1 = (xMin - posn.x) / dir.x;
    tx2 = (xMax - posn.x) / dir.x;
    ty1 = (yMin - posn.y) / dir.y;
    ty2 = (yMax - posn.y) / dir.y;
    tz1 = (zMin - posn.z) / dir.z;
    tz2 = (zMax - posn.z) / dir.z;

    tNear = std::max(std::min(tx1, tx2), std::max(std::min(ty1, ty2), std::min(tz1, tz2)));
    tFar = std::min(std::max(tx1, tx2), std::min(std::max(ty1, ty2), std::max(tz1, tz2)));

    if (tNear > tFar || tFar < 0) {
        return -1;
    }

    return tNear;
}


/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cube.
*/
glm::vec3 Cube::normal(glm::vec3 p) {
    glm::vec3 vMax = glm::vec3(xMax, yMax, zMax);
    glm::vec3 vMin = glm::vec3(xMin, yMin, zMin);
    glm::vec3 center = (vMax + vMin) * 0.5f;
    glm::vec3 n = glm::vec3(p.x - center.x, p.y - center.y, p.z - center.z);
    return glm::normalize(n);
}
