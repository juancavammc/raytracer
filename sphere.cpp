#include "sphere.h"

/*-<==>-----------------------------------------------------------------
/ Constructor
/----------------------------------------------------------------------*/
CSphere::CSphere(SCALAR a_radius) 
: radius( a_radius ) {
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
bool CSphere::hits (const CLine &line, SCALAR &t_hit) {
	// Pendiente de implementar correctamente
	VECTOR p = line.loc - loc;
	double b = p.dot(line.dir);
	double b2 = pow(b,2);
	double c = (p.dot(p)) - (radius*radius);
	if(c > b2)
		return false;
	double aux = sqrt(b2 - c);
	double t0 = -b + aux;
	double t1 = -b - aux;
	if(t1 > SMALL_AMOUNT)
	{
		t_hit = t1;
		return true;
	}
	else if(t0 > SMALL_AMOUNT)
	{
		t_hit = t0;
		return true;
	}
	return false;
}

VECTOR CSphere::getNormal(const VECTOR &hit_loc) 
{
	VECTOR normal = hit_loc - loc;
	normal.normalize();
	return normal;
}
