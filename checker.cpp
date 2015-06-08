#include "checker.h"

CCheckerMaterial::CCheckerMaterial (CMaterial *white, CMaterial *black, SCALAR size){
	tile_white = white;
	tile_black = black;
	side_size = size;
}

COLOR  CCheckerMaterial::getDiffuseColor(const VECTOR &loc) const{
	if(is_white(loc)){
		return tile_white->getDiffuseColor(loc);
	}else{
		return tile_black->getDiffuseColor(loc);
	}
}

SCALAR CCheckerMaterial::getReflectance(const VECTOR &loc) const{
	if(is_white(loc)){
		return tile_white->getReflectance(loc);
	}else{
		return tile_black->getReflectance(loc);
	}
}

SCALAR CCheckerMaterial::getRefraction(const VECTOR &loc) const{
	if(is_white(loc))
		return tile_white->getRefraction(loc);
	else
		return tile_black->getRefraction(loc);
}

SCALAR CCheckerMaterial::getDensity(const VECTOR &loc) const {
	if(is_white(loc))
		return tile_white->getDensity(loc);
	else
		return tile_black->getDensity(loc);
}

bool CCheckerMaterial::is_white(const VECTOR &loc) const{	
	bool x=true;	bool z=true;
	
	if(fmod(fabs(loc.x), side_size)< side_size/2) x= false;
	if(fmod(fabs(loc.z), side_size)< side_size/2) z= false;
	if(!x&&!z&& loc.x>0&&loc.z>0) return false;
	if(x&&z&& loc.x>0&&loc.z>0) return false;
	if(!x&&!z&& loc.x<0&&loc.z<0) return false;
	if(x&&z &&loc.x<0&&loc.z<0) return false;
	if(!x&&z&& loc.x>0&&loc.z<0) return false;
	if(x&&!z&& loc.x<0&&loc.z>0) return false;
	if(!x&&z&& loc.x<0&&loc.z>0) return false;
	if(x&&!z &&loc.x>0&&loc.z<0) return false;
	return true;	
}