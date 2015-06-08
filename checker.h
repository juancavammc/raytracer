#ifndef INC_CHECKER_H_
#define INC_CHECKER_H_

#include "raytracer.h"

class CCheckerMaterial : public CMaterial {
	CMaterial *tile_white;
	CMaterial *tile_black;
	SCALAR     side_size;
	bool is_white(const VECTOR &loc) const;
	SCALAR density;
public:
	CCheckerMaterial (CMaterial *white, CMaterial *black, SCALAR size);
	COLOR  getDiffuseColor(const VECTOR &loc) const;
	SCALAR getReflectance(const VECTOR &loc) const;
	SCALAR getRefraction(const VECTOR &loc) const;
	SCALAR getDensity(const VECTOR &loc) const;
};

#endif