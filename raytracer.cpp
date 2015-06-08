#include <assert.h>
#include "raytracer.h"
#include "image.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <stdlib.h>

VECTOR CRayTracer::unit_vector_random()
{
	float x = unit_random2();
	float y = unit_random2();
	float z = unit_random2();
	return VECTOR(x,y,z);
}

float CRayTracer::unit_random2()
{
	return (((float)rand()/(float)RAND_MAX)*2)-1;
}

float CRayTracer::unit_random()
{
	return (float)rand()/(float)RAND_MAX;
}

float CRayTracer::half_random()
{
	return unit_random()*0.5f;
}

COLOR CRayTracer::computeColorForPixel( float i, float j, int mode, int nsamples )

{
	if(mode == POINT_SAMPLING)
	{
		COLOR color = COLOR(0.0f,0.0f,0.0f);
		CLine line2;
		line2 = camera.getLineAt(i,j);
		trace(line2);
		color = line2.color;
		return color;
	}
	//********************************************************
	// Antialiasing
	//********************************************************
	if(mode == JITTERED_SAMPLING)
	{
		COLOR color = COLOR(0.0f,0.0f,0.0f);
		std::vector<COLOR> vectorColores;
		float n = 2.0;		// Numero de lineas al pixel
		double w = 1.0/n;
		double top_left = -0.5 + w*0.5;
		double cx, cy;
		for(int x = 0; x < n; x++)
		{
			for(int y = 0; y < n; y++)
			{
				cx = (top_left + w*x)+(half_random()*w);
				cy = (top_left + w*x)+(half_random()*w);
				double xx = cx + i;
				double yy = cy + j;
				lineas_jitter++;
				CLine line;
				line = camera.getLineAt(xx,yy);
				trace(line);
				vectorColores.push_back(line.color);
			}
		}
		// Combinar colores
		for(unsigned int k = 0; k < vectorColores.size(); k++)
		{
			color += vectorColores[k];
		}
		color.x/=vectorColores.size();
		color.y/=vectorColores.size();
		color.z/=vectorColores.size();
		return color;
	}
	return COLOR(0,0,0);
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
CRayTracer::CRayTracer() 
: max_recursion_level( 10 )
, background_color( COLOR(0.0,0.0,0.0) )
{
	lineas_camara = 0;
	lineas_sombra = 0;
	lineas_reflejadas = 0;
	lineas_refractadas = 0;
	lineas_jitter = 0;
	intersecciones = 0;
	intersecciones_positivas = 0;
}

/*-<==>-----------------------------------------------------------------
/ Create an image, and for each pixel in the screen create a line
/ and retrieve which color is seen through this line
/ Save the image to a file
/----------------------------------------------------------------------*/
void CRayTracer::render(){
	CBitmap * imagen =  new CBitmap(camera.getXRes(), camera.getYRes(), 24);
	COLOR colorPixel;
	int sampling_mode = JITTERED_SAMPLING;
	//int sampling_mode = POINT_SAMPLING;
	int n = 4;

	for(int i=0; i<imagen->getXRes(); i++)
	{
		for(int j=0; j<imagen->getYRes(); j++){
			lineas_camara++;
			colorPixel = computeColorForPixel((float)i,(float)j, sampling_mode, n);
			colorPixel = aplanarColor(colorPixel);
			imagen->setPixel(i, j, (colorPixel));
		}
		double carga = (double)i*(double)imagen->getYRes();
		carga /= (double)imagen->getYRes()*(double)imagen->getXRes();
		carga*=100.0;

		std::cout << "Completado " << (int)carga <<"%" << std::endl;
	}
	
	imagen->saveTGA("frame03.TGA", true);
	delete imagen;
}

COLOR CRayTracer::aplanarColor(COLOR _color){
	COLOR color = _color;
	if(_color.x > 1.0f)
		color.x = 1.0f;
	if(_color.y > 1.0f)
		color.y = 1.0f;
	if(_color.z > 1.0f)
		color.z = 1.0f;
	return color;
}


/*-<==>-----------------------------------------------------------------
/ Find which object and at which 't' the line hits and object
/ from the scene.
/ Returns true if the object hits some object
/----------------------------------------------------------------------*/

bool CRayTracer::intersects(CLine &line) {
	// ...
	intersecciones++;
	bool intersecta = false;
	SCALAR t_min = 0;
	bool primero = true;
	line.obj = NULL;
	// Example of traversing all the objects registered in the scene
	// Same thing for lights
	LRTObjects::iterator i = objects.begin();
	while( i != objects.end() ) 
	{
		CRTObject *obj = *i++;
		// At this point we can use obj->method...
		if(obj->hits(line, t_min))
		{
			intersecta = true;
			if(primero)
			{
				line.t = t_min;
				line.obj= obj; 
				primero = false;
			}
			else if( t_min < line.t)
			{
				line.t = t_min;
				line.obj = obj;	// Guardamos el objeto contra el que ha intersectado la linea
				line.color=(obj->getMaterial()->getDiffuseColor(line.getIntersection()));
			}
		}
	}
	if(line.obj != NULL){
		intersecciones_positivas++;
	}
	// Pendiente de implementar correctamente
	return intersecta;
}

/*-<==>-----------------------------------------------------------------
/ Returns in line.color the color captured by the line.
/----------------------------------------------------------------------*/
void CRayTracer::trace(CLine &line) {
	COLOR toReturn = COLOR(0.0f,0.0f,0.0f);
	if( intersects(line) ) 
	{
		VECTOR pi = line.getIntersection();
		VECTOR normal = line.obj->getNormal(pi);
		calcularColorLuz(line, pi);
		
		if(line.getLevel() < max_recursion_level)
		{
			// Reflexion
			if(line.obj->getMaterial()->getReflectance(pi) > SMALL_AMOUNT)
			{
				lineas_reflejadas++;
				VECTOR R = getReflexedVector(line, pi);
				CLine lineReflexed(pi,R,line.getLevel()+1);
				trace(lineReflexed);
				COLOR c = lineReflexed.color * line.obj->getMaterial()->getReflectance(pi);
				line.color += c;
			}
			// Refraccion
			if(line.obj->getMaterial()->getRefraction(pi) > 0)
			{
				lineas_refractadas++;
				VECTOR T = getRefractedVector(line, pi);
				CLine lineRefracted = CLine(pi,T,line.getLevel()+1);
				trace(lineRefracted);
				COLOR c = lineRefracted.color*line.obj->getMaterial()->getRefraction(pi);
				line.color += c;
			}
		}
	} 
	else
		background(line);
	
}

VECTOR CRayTracer::getRefractedVector(CLine & line, VECTOR pi)
{
	SCALAR ratio;
	VECTOR I = line.loc - pi;
	I.normalize();
	VECTOR N = line.obj->getNormal(pi);
	double IdotN = N.dot(I);
	if(IdotN > 0)
		line.entrada = true;
	else
	{
		N = -N;
		IdotN = N.dot(I);
		line.entrada = false;
	}
	if(line.entrada)
		ratio = 1.0 / line.obj->getMaterial()->getDensity(pi);
	else
		ratio = line.obj->getMaterial()->getDensity(pi);

	SCALAR aux3 = (ratio*ratio)*(1.0 - (IdotN*IdotN));
	SCALAR aux;
	if(aux3 > 1.0)
		aux = 0.0;
	else
		aux = sqrt(1.0 - aux3);
	SCALAR aux2 =  aux - (IdotN*ratio);
	VECTOR T = -I*(ratio) - N * (aux2);
	T.normalize();
	return T;
}

VECTOR CRayTracer::getReflexedVector(CLine line, VECTOR pi)
{	
	VECTOR N = line.obj->getNormal(pi);
	VECTOR E = line.dir;
	VECTOR Ep = N*(N.dot(E));
	VECTOR R = E - (2*Ep);
	R.normalize();
	return R;
}

void CRayTracer::calcularColorLuz(CLine &line, VECTOR & pi)
{
	LLights::iterator it = lights.begin();
	COLOR color_luz = COLOR(0,0,0);
	double total_difusa = 0;
	double total_especular = 0;
	while( it != lights.end() )
	{
		CLight * luz = *it++;
		VECTOR dir = luz->getLocation()- pi;

		// Calculo de las sombras
		int contador = 0;
		for(unsigned int i = 0; i < 16; i++)
		{
			VECTOR k = unit_vector_random();
			k *= 20;
			VECTOR local = luz->getLocation() + k;
			VECTOR direccion = local - pi;
			CLine sombra(pi, direccion, 0);
			lineas_sombra++;
			if(intersects(sombra))
				contador++;
		}
		SCALAR shade = 1.0f;
		if(contador > 0)
			shade = (1.0/(SCALAR)contador);

		// Calculo de la luz difusa
		dir.normalize();
		VECTOR N = line.obj->getNormal(line.getIntersection());
		SCALAR difusa = dir.dot(N);

		// Calculo de la luz especular
		VECTOR H = dir - line.dir; // H = L + E
		H.normalize();
		SCALAR especular = H.dot(N);

		// Añadimos difusa
		if(difusa > 0)
			total_difusa += difusa;
		
		// Añadimos especular
		if(especular > 0)
			total_especular += pow(especular,80);

		// Total de iluminación
		color_luz += total_difusa * luz->getColor() * shade;

		// Atenuación
		color_luz /=2;
	}
	line.color = line.obj->getMaterial()->getDiffuseColor(pi);
	line.color = line.color.filter(color_luz);
	line.addColor(VECTOR(total_especular,total_especular,total_especular));
}

/*-<==>-----------------------------------------------------------------
/ Default background 
/----------------------------------------------------------------------*/
void CRayTracer::background(CLine &line) {
  line.color = background_color;
}

