#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
CSolidMaterial::CSolidMaterial(const COLOR &diffuse, SCALAR reflectance) {
  diffuse_color = diffuse;
  reflectance_factor = reflectance;
  refraction_factor = 0;
  density = 1;
}

CSolidMaterial::CSolidMaterial(const COLOR &diffuse, SCALAR reflectance, SCALAR refraction, SCALAR coeficient){
	diffuse_color = diffuse;
	reflectance_factor = reflectance;
	refraction_factor = refraction;
	density = coeficient;
}

/*-<==>-----------------------------------------------------------------
/ Diffuse and reflectance parameters are independent of the position
/----------------------------------------------------------------------*/
COLOR  CSolidMaterial::getDiffuseColor(const VECTOR &loc) const { 
  return diffuse_color; 
}

SCALAR CSolidMaterial::getReflectance(const VECTOR &loc)  const { 
  return reflectance_factor;
}

SCALAR CSolidMaterial::getRefraction(const VECTOR &loc)	  const {
	return refraction_factor;
}

SCALAR CSolidMaterial::getDensity(const VECTOR &loc) const {
	return density;
}