#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "raytracer.h"
#include "vector_template.h"

/*-<==>-----------------------------------------------------------------
 / cilindro of radius 'radius' and height 'height'
 /----------------------------------------------------------------------*/
class CCylinder : public CRTObject {
	SCALAR radius;
	SCALAR height;
public:
	CCylinder(SCALAR aradius, SCALAR aheight);
	bool hits (const CLine &line, SCALAR &hits);
	VECTOR getNormal (const VECTOR &loc);
};


#endif