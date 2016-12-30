#include "stdafx.h"

#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h>

#include "camera.h"
#include "point3f.h"

/* 
		The 'origin' of a camera is the cameras actual location in 3D space. This contains 
	no information on where it is facing or it's up vector. It only controls the cameras 
	physical (virtual w/e) location. 
*/  
Point3f fOrigin;  
     
/* 
		The 'focus' of a camera is the location where the camera is looking to. This is a xyz 
	spacial coordinate and does not control the rotation of the camera. The camera 
	is always facing the focus point. 
*/  
Point3f fFocus;  
  
/* 
		The 'upvector' of a camera is what controls the rotation of the camera. By rotation I mean  
	you focus on a object, and you slowly turn it horozontally. There are other ways to manipulate 
	this using the upvector, but that is something that would be better explored through trial and error. 
		
		 Just know that your Focus and upvector should NEVER be parallel. Doing so would result in your 
	scene never being rendered, or never "shown" to you at least. You normally make them cross products 
	of each other to avoid this situation.
*/  
Point3f fUpVector;

void cameraSetup( float xOrigin, float yOrigin, float zOrigin,
				  float xFocus, float yFocus, float zFocus,
				  float xUpV, float yUpV, float zUpV
)
{
	// This just assigns the parameters to their respected positions.
	Point3f tOrigin	= { xOrigin, yOrigin, zOrigin };
	Point3f tFocus	= { xFocus, yFocus, zFocus };
	Point3f tUpV	= { xUpV, yUpV, zUpV };
	
	fOrigin		= tOrigin;
	fFocus		= tFocus;
	fUpVector	= tUpV;
}  
  
void cameraMove(float rate)
{
	/*
	fView is the view vector, or the camera "facing". You find this by subtracting each axis in the focus and origin
	from each other. The result is the facing angle.
	*/
	Point3f fView;

	fView.x = fFocus.x - fOrigin.x;
	fView.y = fFocus.y - fOrigin.y;
	fView.z = fFocus.z - fOrigin.z;
	
	/*
	Below we actually move the camera. Notice we are limiting our freedom of movement though. This is because it is a basic tutorial,  
	the next few tutorials will go more idepth and use the mouse to rotate the camera. The math to move the camera is fairly simple. 
	After figuring out what direction we are facing, we just multiple our rate by each axis to move forward. We add that to our 
	original origin. Be sure you also increase your focus point or your camera will flip as you move (cool effect for some situations). 
	*/
	fOrigin.x += fView.x * rate;
	fOrigin.z += fView.z * rate;
	fFocus.x += fView.x * rate;
	fFocus.z += fView.z * rate;
}