#include "cylinder.h"
//#include <set>
//using namespace std; 

CCylinder::CCylinder(SCALAR a_radius, SCALAR h_height) 
: radius( a_radius ), height(h_height) {
}

/*-<==>-----------------------------------------------------------------
 / 
 /----------------------------------------------------------------------*/
bool CCylinder::hits (const CLine &line, SCALAR &t_hit) 
{	
	VECTOR pi;
	SCALAR ymin = 0;
	SCALAR ymax = height;
	
	VECTOR p = line.loc - loc;

	double a = line.dir.x * line.dir.x + line.dir.z * line.dir.z;
	double b = p.x * line.dir.x + p.z * line.dir.z;
	double c = p.x * p.x + p.z * p.z - (radius*radius);	
	double b2 = b*b;
	double ac = a*c;

	// Corta por el lado
	if(ac <= b2){
		double aux = sqrt(b2-ac);
		double t0 = (-b + aux)/a;
		double t1 = (-b - aux)/a;
		
		
		if(t1>SMALL_AMOUNT)			{t_hit = t1;}
		else if(t0>SMALL_AMOUNT)	{t_hit = t0;}
		if(t1 > SMALL_AMOUNT || t0 > SMALL_AMOUNT)
		{
			pi = p + line.dir * t_hit;
			// Si esta entre la altura del cilindro...
			if(pi.y > ymin && pi.y < ymax)	return true;

		}
	}
	
	// Es posible que corte con alguna de las tapas
	if(fabs(line.dir.y) > 0)
	{
		t_hit = (ymax-p.y)/line.dir.y;
		// Intersecta por encima
		if(t_hit > 0)
		{
			pi = p + line.dir*t_hit;
			// Si la distancia donde corta con el plano es menor al radio...
			if((pi.x*pi.x + pi.z*pi.z ) < radius*radius) return true;
		}

		t_hit = (ymin-p.y)/line.dir.y;
		// Intersecta por debajo
		if(t_hit >0)
		{
			pi = p + line.dir*t_hit;
			if((pi.x*pi.x + pi.z*pi.z ) < radius*radius)	return true;
		}
	}

	t_hit = NULL;
	return false;
}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc) {
	VECTOR dir = hit_loc - loc;
	SCALAR ymin = 0;
	SCALAR ymax = height;
	
	// Si estamos incidiendo por encima
	if(fabs(dir.y - ymax)<SMALL_AMOUNT){
		return VECTOR(0,1,0);
	}
	// Si estamos incidiendo por debajo
	if(fabs(dir.y - ymin)<SMALL_AMOUNT){
		return VECTOR(0,-1,0);
	}
	// Si estamos incidiendo por el lado
	return VECTOR((dir.x)/radius, 0, (dir.z)/radius);
}
