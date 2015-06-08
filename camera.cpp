/*#include "raytracer.h"
// return a line which starts on camera position and goes through the pixel
// (x,y) from the screen
CLine CCamera::getLineAt (SCALAR x, SCALAR y) const {
  // Pendiente de implementar correctamente
	// ...
	SCALAR beta = (yres/2)- y - 0.5;
	SCALAR alpha = (xres/2) - x - 0.5;
	VECTOR dir = (viewd * front) + (beta * up) + (left * alpha);
	return CLine(loc, dir, 0);
}

*/
#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ 
/---------------------------------------------------------------------*/
CCamera::CCamera() {
	// Initialize with some default parameters
  	setRenderParameters(320, 240, 60.0f);
	setView(VECTOR(0,0,0), VECTOR(0,0,1));
}

CCamera::~CCamera() {
}

/*-<==>-----------------------------------------------------------------
/ Save render parameters for later use
/ fov is in degrees
/---------------------------------------------------------------------*/
void CCamera::setRenderParameters (int axres, int ayres, SCALAR afov_in_deg) 
{
	xres = axres;
	yres = ayres;
	SCALAR afov_in_rads = convertirGradosRadianes(afov_in_deg);
	viewd = (ayres/2)/tan(afov_in_rads/2);
}
SCALAR CCamera::convertirGradosRadianes(SCALAR afov_in_deg)
{
	return (afov_in_deg*M_PI)/180;
}
/*-<==>-----------------------------------------------------------------
/ Define the axis of the camera (front, up, left) in world coordinates 
/ based on the current values of the vectors target & loc 
/---------------------------------------------------------------------*/
void CCamera::initAxis() {
	front = target - loc;
	front.normalize();
	VECTOR aux = VECTOR(0,1,0);
	left = aux.cross(front);
	left.normalize();
	up = -left.cross(front);
}

/*-<==>-----------------------------------------------------------------
/ Save the new camera position and target point
/---------------------------------------------------------------------*/
void CCamera::setView(const VECTOR &src_point, const VECTOR &dst_point) {
	loc = src_point;
	target = dst_point;
	initAxis();
}

/*-<==>-----------------------------------------------------------------
/ return a line which starts on camera position and goes through the pixel
/ (x,y) from the screen
/---------------------------------------------------------------------*/
CLine CCamera::getLineAt (SCALAR x, SCALAR y) const 
{
	SCALAR beta = (yres/2)- y - 0.5;
	SCALAR alpha = (xres/2) - x - 0.5;
	VECTOR dir = (viewd * front) + (beta * up) + (left * alpha);
	return CLine(loc, dir, 0);
}

