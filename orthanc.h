#ifndef _ORTHANC_
#define _ORTHANC_

// OpenGL includes
#include <GL/freeglut.h>
#include <GL/glext.h>

#include <GL/glut.h>

// Other C++ libraries
#include <stdio.h>
#include <iostream>
#include <math.h> // needed for sin() and cos()

// Used for loading textures
#include "imageloader.h"
#include <png.h> 		// libpgn

#include "snow.h"
#include "input.h"
#include "tower.h"
#include "textures.h"
#include "world.h"

//-GLOBAL-VARIABLES--------------------------------------------------------------
extern GLfloat X;			// Translate screen to x direction (left or right)
extern GLfloat Y;			// Translate screen to y direction (up or down)
extern GLfloat Z;			// Translate screen to z direction (zoom in or out)
extern GLfloat rotX;		// Rotate screen on x axis 
extern GLfloat rotY;		// Rotate screen on y axis
extern GLfloat rotZ;		// Rotate screen on z axis

// Camera position
extern float x, y, z;
extern float deltaMove; // Initially camera doesn't move
extern float strafe;

// Camera direction
extern float lx, ly, lz, hx, hz; // camera points initially along y-axis
extern float angle, angleZ; // angle of rotation for the camera direction
extern float deltaAngle, deltaAngleZ; // additional angle change when dragging

// Mouse drag control
extern int isDragging; // true when dragging
extern int xDragStart; // records the x-coordinate when dragging starts
extern int zDragStart;

extern float zoom; // Acumulated zoom factor

// Vectors to hold textures
extern GLfloat _textureId[6];       // skybox
extern GLfloat texture_octagon;     // octagon 
extern GLfloat _texture_Grass;     // grass 
extern GLfloat _texture_stoneId[2]; // stones
extern GLubyte *_textureShadow;	 // shadow (png)

static float boxSize = 50.0; // Size of the SkyBox, floor, world.

// Flags 
extern bool wireFrame  ;		// Flag used for changing the glPolygonMode 
extern bool showAxis   ;		// Flag used for showing or not the axis lines
extern bool enableFog  ;		// Flag used for enable the fog (starts deactivated)
extern bool enableSnow ;		// Flag used for enable the snow (starts deactivated)
extern bool toggleGrass;		// Activate grass texture (not working with the shadow)

#endif
