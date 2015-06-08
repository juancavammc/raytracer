#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"
#include <fstream>
#include <iostream>
#include "math.h"
#include <stdio.h>
#include <string.h> 
using namespace std;

/*-<==>-----------------------------------------------------------------
/ Defines the scene
/----------------------------------------------------------------------*/
void CRayTracer::load () {

  // Add the camera looking at the origin
  //camera.setView (VECTOR(300, 120, 120), VECTOR (0,0,0));
  camera.setView (VECTOR(0, 120, 450), VECTOR (0,0,0));
  camera.setRenderParameters (640,480,45);

  // Define some materials
	materials["orange"]		= new CSolidMaterial (COLOR (1, 0.331, 0.074), 0.6);
	materials["blue"]		= new CSolidMaterial (COLOR (0.0, 0.2, 0.8), 0.0, 0.9, 1.33);
	materials["black"]		= new CSolidMaterial (COLOR (0.0, 0.0, 0.0), 0.0);
	materials["white"]		= new CSolidMaterial (COLOR (1.0, 1.0, 1.0), 0.0);
	materials["checker"]	= new CCheckerMaterial(materials["white"], materials["black"], 100);

	// Add the ground
	CPlane *plane = new CPlane (VECTOR(0,1,0), 0);
	plane->setMaterial (materials["checker"]);
	objects.push_back (plane);

	// Add a sphere
	CSphere *sph = new CSphere(75);
	sph->setLocation (VECTOR(0,75,25));
	sph->setMaterial (materials["blue"]);
	objects.push_back (sph);

	// Add a cylinder
	CCylinder * cyl = new CCylinder(100,200);
	cyl->setLocation (VECTOR(75,0,-500));
	cyl->setMaterial (materials["orange"]);
	objects.push_back (cyl);

	// Add a cylinder
	/*CCylinder * cyl2 = new CCylinder(100,200);
	cyl2->setLocation (VECTOR(0,0,0));
	cyl2->setMaterial (materials["blue"]);
	objects.push_back (cyl2);*/

	// Add a single white light
	CLight *light = new CLight(VECTOR (400,400,400), COLOR (1,1,1));
	lights.push_back (light);
	//CLight *light2 = new CLight(VECTOR (0,200,-400), COLOR (0,1,0));
	//lights.push_back (light2);
	//CLight *light3 = new CLight(VECTOR (400,400,380), COLOR (0,0,1));
	//lights.push_back (light3);
}

bool CRayTracer::loadSnowflake (const char *filename) {
	FILE *f = fopen (filename, "r");
	if (!f)
		return false;
	
	// Add the camera looking at the origin
	camera.setView (VECTOR(2.1, 1.7, 1.3), VECTOR (0,0,0));
	camera.setRenderParameters (1024,1024,45);
	
	// Define background color
	background_color = COLOR( 0.078, 0.361, 0.753 );
	
	// Add a two material
	materials["txt001"]    = new CSolidMaterial (COLOR (0.8, 0.6, 0.264), 0);
	materials["txt002"]    = new CSolidMaterial (COLOR (0.5, 0.45, 0.35), 0.5);
	
	// Add the ground
	CPlane *plane = new CPlane (VECTOR(0,1,0), -0.5);
	plane->setMaterial (materials["txt001"]);
	objects.push_back (plane); 
	
	// This is a very simply parser!!
	while (!feof(f)) {
		char buf[512];
		fgets (buf, 511, f);
		if (strncmp (buf, "sphere", 6) == 0) {
			char material[64];
			double x,y,z, rad;
			sscanf (buf, "sphere %s %lf %lf %lf %lf\n", material, &rad, &x,&y,&z);
			CSphere *sph = new CSphere(rad);
			sph->setLocation (VECTOR(x,z,y));
			sph->setMaterial (materials["txt002"]);
			objects.push_back (sph);
		} 
	}
	
	lights.push_back (new CLight(VECTOR ( 4, 2, 3), COLOR (1,1,1)));
	lights.push_back (new CLight(VECTOR ( 1,4, -4), COLOR (1,1,1)));
	lights.push_back (new CLight(VECTOR (-3, 5, 1), COLOR (1,1,1)));
	
	fclose (f);
	return true;
}



/*-<==>-----------------------------------------------------------------
/ MAIN
/----------------------------------------------------------------------*/
int main(int argc, char **argv) {
	CRayTracer rt;
	rt.setMaxRecursionLevel(1);
	
	const char *filename="balls_0.txt";
	if(rt.loadSnowflake(filename)){  rt.render(); }

	/*rt.load();*/
	rt.render();
	//rt.imprimirEstadisticas();
	return 0;
}

void CRayTracer::imprimirEstadisticas()
{
	ofstream fs("Estadisticas.txt");
	fs << "Lineas lanzadas:" << endl;
	fs << "- Camara: " << lineas_camara << endl;
	fs << "- Sombra: " << lineas_sombra << endl;
	fs << "- Reflejadas: " << lineas_reflejadas << endl;
	fs << "- Refractadas: " << lineas_refractadas << endl;
	fs << "- Antialiasing: " << lineas_jitter << endl;
	long total = lineas_camara + lineas_sombra + lineas_reflejadas + lineas_refractadas + lineas_jitter;;
	fs << "- Total: " << total << endl;
	fs << "-------------------------------------------------------------------------" << endl;
	fs << "- Numero de intersecciones: " << intersecciones << endl;
	fs << "- Intersecciones positivas: " << intersecciones_positivas << endl;
	double ratio_intersecciones = ((float)intersecciones_positivas/(float)intersecciones)*100.0;
	fs << "- Ratio de acierto: " <<  ratio_intersecciones << "%" << endl;
	fs << "-------------------------------------------------------------------------" << endl;
	fs << "- Numero de luces: " <<  lights.size() << endl;
	fs << "- Numero de objetos:  " <<  objects.size() << endl;
	fs << "- Resolución: " << camera.getXRes() << "x" << camera.getYRes() << endl;

	fs.close();
}