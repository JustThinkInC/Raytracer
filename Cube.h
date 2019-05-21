/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Cube class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_Cube
#define H_Cube
#include <glm/glm.hpp>
#include <algorithm>
#include "SceneObject.h"

/**
 * Defines a simple Sphere located at 'center'
 * with the specified radius
 */
class Cube : public SceneObject
{

private:
    float xMin, xMax, yMin, yMax, zMin, zMax;

public:
	Cube()
	{
	    glm::vec3 cornerLeft = glm::vec3(-1, -1, -1);
	    glm::vec3 cornerRight = glm::vec3(1, 1, 1);

        xMin = std::min(cornerLeft.x, cornerRight.x);
        xMax = std::max(cornerLeft.x, cornerRight.x);
        yMin = std::min(cornerLeft.y, cornerRight.y);
        yMax = std::max(cornerLeft.y, cornerRight.y);
        zMin = std::min(cornerLeft.z, cornerRight.z);
        zMax = std::max(cornerLeft.z, cornerRight.z);

		color = glm::vec3(1);
	};

    Cube(glm::vec3 cornerLeft, glm::vec3 cornerRight, glm::vec3 col)
	{
		color = col;
		xMin = std::min(cornerLeft.x, cornerRight.x);
		xMax = std::max(cornerLeft.x, cornerRight.x);
		yMin = std::min(cornerLeft.y, cornerRight.y);
		yMax = std::max(cornerLeft.y, cornerRight.y);
		zMin = std::min(cornerLeft.z, cornerRight.z);
		zMax = std::max(cornerLeft.z, cornerRight.z);
	};

	float intersect(glm::vec3 posn, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_Cube
