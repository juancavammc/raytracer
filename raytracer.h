#ifndef INC_RAY_TRACER_H_
#define INC_RAY_TRACER_H_

#include <iostream>
#include "geometry.h"
#include "material.h"
#include "rtobject.h"
#include "camera.h"
#include "light.h"
#include "line.h"
#include "checker.h"
#define SMALL_AMOUNT      1e-5
#define POINT_SAMPLING      0
#define UNIFORM_SAMPLING	1
#define JITTERED_SAMPLING	2

// Una estructura para guardar cada sample con su peso
struct TSample {
	float w;
	COLOR color;
	TSample( float aw, const COLOR &c ) : w(aw), color(c) { }
};


/*-<==>-----------------------------------------------------------------
/ Raytracer
/---------------------------------------------------------------------*/
class CRayTracer {
	
	
	// Maximum recursion level allowed while computing 
	// lines 
	int         max_recursion_level;
	
	//	CCamera     camera; 			// The camera to render the scene from
	LRTObjects  objects; 			// List of objects defined in the scene
	MMaterials  materials; 		// Map of materials defined in the scene
	LLights     lights; 			// List of lights defined in the world
	COLOR       background_color;
	
	void trace (CLine &line);
	bool intersects (CLine &line);
	void background (CLine &line);
public:
	CCamera     camera; 			// The camera to render the scene from

	long lineas_camara;
	long lineas_sombra;
	long lineas_reflejadas;
	long lineas_refractadas;
	long lineas_jitter;
	long intersecciones;
	long intersecciones_positivas;

	CRayTracer();
	void render();
	void load();              // defined in main.cpp
	bool loadSnowflake (const char *filename);
	void setMaxRecursionLevel(int max){max_recursion_level=max;};
	void imprimirEstadisticas();
	
	VECTOR unit_vector_random();
	float unit_random();
	float unit_random2();
	float half_random();
	COLOR computeColorForPixel( float i, float j, int mode, int nsamples );
	COLOR aplanarColor(COLOR _color);
	VECTOR getReflexedVector(CLine line, VECTOR pi);
	void calcularColorLuz(CLine &line, VECTOR & pi);
	VECTOR getRefractedVector(CLine & line, VECTOR pi);
};

// Send a vector to a output stream
std::ostream &operator << (std::ostream &os, const VECTOR &v);

#endif
