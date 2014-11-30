/*--------------------------------------------------------------------
#	Computer Graphics - COMP 371 - Final Project #1
# 	Students: Agustin Mariojouls, Mateus Machado Luna and Nicolas
#
#	PLEASE READ THE readme.txt FILE FOR MORE INFO
#
--------------------------------------------------------------------*/

// agu-pc includes
#include <GL/glut.h>
#include "imageloader.h"

// OpenGL includes
#include <GL/freeglut.h>
#include <GL/glext.h>

// Other libraries
#include <stdio.h>
#include <iostream>
#include <math.h> // needed for sin() and cos()

// Used for loading textures
#include "imageloader.h"
#include <png.h>


// Global Variables
GLfloat X = 0.0f;			// Translate screen to x direction (left or right)
GLfloat Y = 0.0f;			// Translate screen to y direction (up or down)
GLfloat Z = 0.0f;			// Translate screen to z direction (zoom in or out)
GLfloat rotX = 0.0f;		// Rotate screen on x axis 
GLfloat rotY = 0.0f;		// Rotate screen on y axis
GLfloat rotZ = 0.0f;		// Rotate screen on z axis

// Camera position
float x = 0.0, y = 0.0, z = -4.0f;
float deltaMove = 0.0; // initially camera doesn't move
float strafe = 0.0;

// Camera direction
float lx = 0.0, ly = 0.0, lz = 1.0f, hx =1.0, hz = 0.0f; // camera points initially along y-axis
float angle = 0.0, angleZ = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0, deltaAngleZ = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts
int zDragStart = 0;

float zoom = 0.0; // Acumulated zoom factor
float orthoSize = 2;

// For building the walls of Isengard
float isengardRadius = 26;
float isengardAngle = 0;

// vectors to hold textures
GLfloat _textureId[6]; //skybox
GLubyte *_textureShadow;

int width, wh;
int height, hw;

// Flags for collision detection
// bool collisionX = false;
// bool collisionZ = false;

// Flags for visualization
bool wireFrame  = false;		// Flag used for changing the glPolygonMode 
bool showAxis   = true;			// Flag used for showing or not the axis lines
bool enableFog  = false;		// Flag used for enable the fog (starts deactivated)
bool enableSnow = false;		// Flag used for enable the snow (starts deactivated)

// STRUCTS------------------------------------------------------------------------------//
typedef struct _pilasterStruct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat depth;
	GLfloat height;
	GLfloat width;

} pilasterStruct;

//-FUNCTIONS----------------------------------------------------------------------------//

// This function create the symetric Crux on a y-1.326 and a y-Height position
// The ext refer to the widt of the external points of the crux, and the int to
// the internal points of the crux 

// Camera movement
void update(void)
{
	if (deltaMove) 
	{ 				// update camera position
		x += deltaMove * lx * 0.4;
		z += deltaMove * lz * 0.4;
	}
	if (strafe) 
		{ 			// update camera position
		x += strafe * hx * 0.4;
		z += strafe * hz * 0.4;
	}

	glutPostRedisplay(); // redisplay everything
}

// Load png with trasnparency by libpng
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
 
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
 
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
 
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
 
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
 
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
 
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
    /* Close the file */
    fclose(fp);
 
    /* That's it */
    return true;
}

// Load textures by imageloader (bmp, 24 bits)
GLuint loadTexture(Image* image) 
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGB,
					image->width, image->height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					image->pixels
				);

	return textureId;
}

// Creating skybox function
float boxSize = 50.0;
void createSkybox()
{
	glPushMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		// Render the front quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[3]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); 
			glVertex3f(boxSize, -boxSize, -boxSize);
			glTexCoord2f(1, 0); 
			glVertex3f(-boxSize, -boxSize, -boxSize);
			glTexCoord2f(1, 1); 
			glVertex3f(-boxSize, boxSize, -boxSize);
			glTexCoord2f(0, 1); 
			glVertex3f(boxSize, boxSize, -boxSize);
		glEnd();

		//left
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[5]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 0); glVertex3f(boxSize, -boxSize, -boxSize);
			glTexCoord2f(1, 1); glVertex3f(boxSize, boxSize, -boxSize);
			glTexCoord2f(0, 1); glVertex3f(boxSize, boxSize, boxSize);
		glEnd();


		// Render the back quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[2]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 0); glVertex3f(boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 1); glVertex3f(boxSize, boxSize, boxSize);
			glTexCoord2f(0, 1); glVertex3f(-boxSize, boxSize, boxSize);
		glEnd();

		// Render the right quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[1]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-boxSize, -boxSize, -boxSize);
			glTexCoord2f(1, 0); glVertex3f(-boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 1); glVertex3f(-boxSize, boxSize, boxSize);
			glTexCoord2f(0, 1); glVertex3f(-boxSize, boxSize, -boxSize);
		glEnd();

		// Render the 1.326 quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[4]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex3f(-boxSize, boxSize, -boxSize);
			glTexCoord2f(0, 0); glVertex3f(-boxSize, boxSize, boxSize);
			glTexCoord2f(1, 0); glVertex3f(boxSize, boxSize, boxSize);
			glTexCoord2f(1, 1); glVertex3f(boxSize, boxSize, -boxSize);
		glEnd();


		// Render the bottom quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, _textureId[0]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(-boxSize, -boxSize, -boxSize);
			glTexCoord2f(0, 1); glVertex3f(-boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 1); glVertex3f(boxSize, -boxSize, boxSize);
			glTexCoord2f(1, 0); glVertex3f(boxSize, -boxSize, -boxSize);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);  //turn everything back, which we turned on, and turn everything off, which we have turned on.
		glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}

// -- Particles System
const int ParticleCount = 8000;

typedef struct
{
	double Xpos;
	double Ypos;
	double Zpos;
	double Xmov;
	double Zmov;
	double Red;
	double Green;
	double Blue;
	double Direction;
	double Acceleration;
	double Deceleration;
	double Scalez;
	bool Visible;
}PARTICLES;

PARTICLES Particle[ParticleCount];

void glCreateParticles (void)
{
	int i;
	for (i = 1; i < ParticleCount; i++)
	{
		Particle[i].Xpos = 0;
		Particle[i].Ypos = boxSize - 1;
		Particle[i].Zpos = 0;
		Particle[i].Xmov = (((((((2 - 1 + 1) * rand()%((int)boxSize*4)) + 1) - 1 + 1) * rand()%((int)boxSize*4))) + 1) * 0.005 - (((((((((2 - 1 + 1) * rand()%((int)boxSize*4))) + 1) - 1 + 1) * rand()%((int)boxSize*4))) + 1) * 0.005);
		Particle[i].Zmov = (((((((((2 - 1 + 1) * rand()%((int)boxSize*4))) + 1) - 1 + 1) * rand()%((int)boxSize*4))) + 1) * 0.005) - (((((((((2 - 1 + 1) * rand()%((int)boxSize*4))) + 1) - 1 + 1) * rand()%((int)boxSize*4))) + 1) * 0.005);
		Particle[i].Red = 1;
		Particle[i].Green = 1;
		Particle[i].Blue = 1;
		Particle[i].Scalez = 0.15;
		Particle[i].Direction = 0;
		Particle[i].Acceleration = -((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.03;
		Particle[i].Deceleration = 0.0025;
	}
}

void glUpdateParticles (void)
{
	int i;
	for (i = 1; i < ParticleCount; i++)
	{
		glColor3f (Particle[i].Red, Particle[i].Green, Particle[i].Blue);

		Particle[i].Ypos = Particle[i].Ypos + Particle[i].Acceleration -  Particle[i].Deceleration;
		Particle[i].Deceleration = Particle[i].Deceleration + 0.0025;
		Particle[i].Xpos = Particle[i].Xpos + Particle[i].Xmov;
		Particle[i].Zpos = Particle[i].Zpos + Particle[i].Zmov;
		Particle[i].Direction = Particle[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%((int)boxSize*4)) + 1) - 1 + 1) * rand()%((int)boxSize*4)) + 1);

		if (Particle[i].Ypos < -boxSize || Particle[i].Ypos > boxSize || Particle[i].Xpos > boxSize || Particle[i].Xpos < -boxSize || Particle[i].Zpos > boxSize || Particle[i].Zpos < -boxSize)
		{
			Particle[i].Xpos = 0;
			Particle[i].Ypos = boxSize-1;
			Particle[i].Zpos = 0;
			Particle[i].Red = 1;
			Particle[i].Green = 1;
			Particle[i].Blue = 1;
			Particle[i].Direction = 0;
			Particle[i].Acceleration = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
			Particle[i].Deceleration = 0.0025;
		}
	}
}

void glDrawParticles (void)
{
	int i;

	for (i = 1; i < ParticleCount; i++)
	{
		glPushMatrix();

			glTranslatef(Particle[i].Xpos, Particle[i].Ypos, Particle[i].Zpos);
			glRotatef(Particle[i].Direction - 90, 0, 0, 1);
	   
			glScalef (Particle[i].Scalez, Particle[i].Scalez, Particle[i].Scalez);

			glBegin(GL_QUADS);
				glVertex3f(-0.1, -0.1, 0);
				glVertex3f (0.1, -0.1, 0);
				glVertex3f (0.1, 0.1, 0);
				glVertex3f (-0.1, 0.1, 0);
			glEnd();

		glPopMatrix();
	}
}

// ---- End of Particles

// Draw the Ground
void drawGround()
{
	glColor3f(0.1, 0.1, 0.1);

	// Draw the foor, with a mesh
	glPushMatrix();
		
		glTranslatef(-boxSize, 0, -boxSize);				

		glBegin(GL_TRIANGLES);

			int x, z;
			
			for (x = 0; x < boxSize*2; x += 2)
			{
				for (z = 0; z < boxSize*2; z += 2)
				{
					glVertex3f(x,0,z);
					glVertex3f(x+2,0,z+2);
					glVertex3f(x,0,z+2);
				}

			}

			for (x = 0; x < boxSize*2; x += 2)
			{
				for (z = 0; z < boxSize*2; z += 2)
				{
					glVertex3f(x+2,0,z+2);
					glVertex3f(x,0,z);
					glVertex3f(x+2,0,z);
				}

			}

		glEnd();

	glPopMatrix();

	glPushMatrix(); // TOWER SHADOW PROJECTION

		glDisable(GL_LIGHTING);
		// glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
    	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		glTexImage2D(GL_TEXTURE_2D, 0, 4, 1151,
                 1151, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 _textureShadow);
   		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glColor3f(1, 1, 1);

		glRotatef(45, 0, 1, 0);
		glTranslatef( 0.55, 0.0, -5.15);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); 
			glVertex3f(0, 0.001, 0);
			glTexCoord2f(1, 0); 
			glVertex3f(0, 0.001, 10.1);
			glTexCoord2f(1, 1); 
			glVertex3f( -10.1, 0.001, 10.1);
			glTexCoord2f(0, 1); 
			glVertex3f( -10.1, 0.001, 0);
		glEnd();	

		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
		// glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();

}

// Light Ambient
void lAmbient()
{
	GLfloat lightDiffuse [] = { 1.0f, 1.0f , 1.0f };
	GLfloat lightAmbient [] = { 1.0f, 1.0f , 1.0f };
	GLfloat lightPosition[] = { 1.0f, 1.0f, -1.0f, 0.1f };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	return;
}

void fog()
{
	GLfloat fogColor[] = {0.9f, 0.9f, 1.0f, 1};	//Sets the color for the fog

	glFogfv(GL_FOG_COLOR, fogColor);			//Modes for the fog are linear, exp and exp2
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 40.0f);
	glFogf(GL_FOG_DENSITY, 0.015f);
}


void createCrux(GLfloat internal, GLfloat external, GLfloat height)
{

	internal /= 2;
	external /= 2;

	glBegin(GL_POLYGON);
		// Points	 X          Y  	     Z	
		glVertex3f(-internal, height, -internal);
		glVertex3f(-external, height, -internal);
		glVertex3f(-external, height, internal);
		glVertex3f(-internal, height, internal);
		glVertex3f(-internal, height, external);
		glVertex3f(internal, height, external);
		glVertex3f(internal, height, internal);
		glVertex3f(external, height, internal);
		glVertex3f(external, height, -internal);
		glVertex3f(internal, height, -internal);
		glVertex3f(internal, height, -external);
		glVertex3f(-internal, height, -external);
	glEnd();
}

// This function create the symetric 12-sided wall between a y-1.326 and a y-Bottom
// The ext refer to the widt of the external points of the crux, and the int to
// the internal points of the crux 
void createWalls(GLfloat intB, GLfloat extB, GLfloat intT, GLfloat extT, GLfloat bottom, GLfloat top)
{

	intB /= 2;
	extB /= 2;
	intT /= 2;
	extT /= 2;

	// Laterals
	//  Face A
	glBegin(GL_POLYGON);
		glVertex3f(-intT, top, -intT);
		glVertex3f(-extT, top, -intT);
		glVertex3f(-extB, bottom, -intB);
		glVertex3f(-intB, bottom, -intB);
	glEnd();

	//  Face B
	glBegin(GL_POLYGON);
		glVertex3f(-extT, top, intT);
		glVertex3f(-extT, top, -intT);
		glVertex3f(-extB, bottom, -intB);
		glVertex3f(-extB, bottom, intB);
	glEnd();

	//  Face C
	glBegin(GL_POLYGON);
		glVertex3f(-extT, top, intT);
		glVertex3f(-intT, top, intT);
		glVertex3f(-intB, bottom, intB);
		glVertex3f(-extB, bottom, intB);
	glEnd();

	//  Face D
	glBegin(GL_POLYGON);
		glVertex3f(-intT, top, extT);
		glVertex3f(-intT, top, intT);
		glVertex3f(-intB, bottom, intB);
		glVertex3f(-intB, bottom, extB);
	glEnd();

	//  Face E
	glBegin(GL_POLYGON);
		glVertex3f(-intT, top, extT);
		glVertex3f(intT, top, extT);
		glVertex3f(intB, bottom, extB);
		glVertex3f(-intB, bottom, extB);
	glEnd();

	//  Face F
	glBegin(GL_POLYGON);
		glVertex3f(intT, top, intT);
		glVertex3f(intT, top, extT);
		glVertex3f(intB, bottom, extB);
		glVertex3f(intB, bottom, intB);
	glEnd();

	//  Face G
	glBegin(GL_POLYGON);
		glVertex3f(intT, top, intT);
		glVertex3f(extT, top, intT);
		glVertex3f(extB, bottom, intB);
		glVertex3f(intB, bottom, intB);
	glEnd();

	// Face H
	glBegin(GL_POLYGON);
		glVertex3f(extT, top, intT);
		glVertex3f(extT, top, -intT);
		glVertex3f(extB, bottom, -intB);
		glVertex3f(extB, bottom, intB);
	glEnd();

	// Face I
	glBegin(GL_POLYGON);
		glVertex3f(intT, top, -intT);
		glVertex3f(extT, top, -intT);
		glVertex3f(extB, bottom, -intB);
		glVertex3f(intB, bottom, -intB);
	glEnd();

	// Face J
	glBegin(GL_POLYGON);
		glVertex3f(intT, top, -extT);
		glVertex3f(intT, top, -intT);
		glVertex3f(intB, bottom, -intB);
		glVertex3f(intB, bottom, -extB);
	glEnd();

	//  Face K
	glBegin(GL_POLYGON);
		glVertex3f(intT, top, -extT);
		glVertex3f(-intT, top, -extT);
		glVertex3f(-intB, bottom, -extB);
		glVertex3f(intB, bottom, -extB);
	glEnd();

	//  Face L
	glBegin(GL_POLYGON);
		glVertex3f(-intT, top, -extT);
		glVertex3f(-intT, top, -intT);
		glVertex3f(-intB, bottom, -intB);
		glVertex3f(-intB, bottom, -extB);
	glEnd();
}

// Creates a single simple rectangular pilaster
// x, y, and z refer to the inferior-left-insidetower point
void createPilaster(pilasterStruct p)
{
	// Internal Face
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z);
	glEnd();

	// 1.326
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y + p.height, p.z);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);
	glEnd();

	// Lateral 1
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
	glEnd();

	// Lateral 2
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y + p.height, p.z);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);
		glVertex3f(p.x + p.depth, p.y, p.z);
	glEnd();

	// Bottom
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y, p.z);
	glEnd();

	// External Face
	glBegin(GL_POLYGON);
		glVertex3f(p.x + p.depth, p.y, p.z);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);
	glEnd();
}

// Creates a single simple pointing pilaster (1.326 thin, bottom width-size)
// x, y, and z refer to the inferior-left-insidetower point
void createPilasterPointy(pilasterStruct p)
{
	// Internal Face
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);// thin
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);// thin		
	glEnd();

	// Lateral 1
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
	glEnd();

	// Lateral 2
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glVertex3f(p.x + p.depth, p.y, p.z);
	glEnd();

	// Bottom
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y, p.z);
	glEnd();

	// External Face
	glBegin(GL_POLYGON);
		glVertex3f(p.x + p.depth, p.y, p.z);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
	glEnd();
}

// Multiply (and create) pilasters created with the struct pilasterStruct
// Uses two loops for creating exactly from the center.
void multiplyPilasters(int times, GLfloat space, pilasterStruct p)
{
	int i;

	for (i = 0; i < times / 2 + 1; i++)
	{
		p.z = 0 - i*space;
		createPilaster(p);
	}
	for (i = 0; i < times / 2; i++)
	{
		p.z = space + i*space;
		createPilaster(p);
	}
}

// Multiply (and create) pointy pilasters created with the struct pilasterStruct
// Uses two loops for creating exactly from the center.
void multiplyPointyPilasters(int times, GLfloat space, pilasterStruct p)
{
	int i;

	for (i = 0; i < times / 2 + 1; i++)
	{
		p.z = 0 - i*space;
		createPilasterPointy(p);
	}
	for (i = 0; i < times / 2; i++)
	{
		p.z = space + i*space;
		createPilasterPointy(p);
	}
}

// Creates a bezier path in a 2D space;
// The first point is the base point, the next two are the extremes points, 
// and the last two are control ones;
void bezierPath(GLfloat Bx, GLfloat By,
	GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y,
	GLfloat C1x, GLfloat C1y, GLfloat C2x, GLfloat C2y)
{

	GLfloat ctrlpoints[4][3] = { { P1x, P1y, 0.0 }, { C1x, C1y, 0.0 },
	{ C2x, C2y, 0.0 }, { P2x, P2y, 0.0 } };

	glEnable(GL_MAP1_VERTEX_3);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(Bx, By, 0.0);
		for (int i = 0; i <= 30; i++)
			glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();

	glDisable(GL_MAP1_VERTEX_3);
}

// Creates a single small thorn face
void createThorn(GLfloat   Tx, GLfloat  Ty, 							// Tip
	GLfloat  P1x, GLfloat  P1y, GLfloat  P2x, GLfloat  P2y,
	GLfloat  C1x, GLfloat  C1y, GLfloat  C2x, GLfloat  C2y,
	GLfloat CT1x, GLfloat CT1y, GLfloat CT2x, GLfloat CT2y)
{
	bezierPath(P2x, P2y,
		Tx, Ty, P1x, P1y,
		CT1x, CT1y, C1x, C1y);

	GLfloat ctrlpoints[4][3] = { { P2x, P2y, 0.0 }, { C2x, C2y, 0.0 },
	{ CT2x, CT2y, 0.0 }, { Tx, Ty, 0.0 } };

	glEnable(GL_MAP1_VERTEX_3);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

	glBegin(GL_POLYGON);
		for (int i = 0; i <= 30; i++)
			glEvalCoord1f((GLfloat)i / 30.0);
	glEnd();

	glDisable(GL_MAP1_VERTEX_3);
}

void mainThornFace()
{
	glPushMatrix();

	// Two polygons for the non-curvy and non-convex part
	glBegin(GL_POLYGON);

		glVertex3f(0.000, 0.000, 0.0);
		glVertex3f(0.510, 0.000, 0.0);
		glVertex3f(0.548, 0.106, 0.0);
		glVertex3f(0.515, 0.311, 0.0);
		glVertex3f(0.515, 0.864, 0.0);
		glVertex3f(0.319, 1.326, 0.0);
		glVertex3f(0.000, 1.326, 0.0);

	glEnd();

	glBegin(GL_POLYGON);

		glVertex3f(0.515, 0.864, 0.0);
		glVertex3f(0.687, 0.864, 0.0);
		glVertex3f(0.687, 1.820, 0.0);
		glVertex3f(0.319, 1.820, 0.0);
		glVertex3f(0.319, 1.326, 0.0);

	glEnd();

	// Top of the bigger Thorn
	bezierPath(0.687, 1.820,				// Base Point
		0.319, 1.820, 0.687, 2.781,	// Curve Extremes
		0.570, 2.110, 0.640, 2.380);// Curve 

	// Curve between the bigger Thorn and the center
	bezierPath(0.515, 0.864,				// Base Point
		0.687, 0.864, 0.515, 0.595,	// Curve Extremes
		0.580, 0.810, 0.540, 0.720);// Curve 

	// Lateral Thorn
	createThorn(0.687, 0.478,
		0.515, 0.311, 0.548, 0.106,
		0.600, 0.260, 0.630, 0.230,
		0.660, 0.400, 0.660, 0.330);

	glPopMatrix();

}

void mainThornBody()
{

	// Main Thorns
	glPushMatrix();
	glTranslatef(0.6875, 0, 0);
	glRotatef(2.880, 0, 1, 0);
	glTranslatef(-0.6875, 0, 0);
	mainThornFace();
	glPopMatrix();

	// Top of the bigger Thorn
	// glPushMatrix();
	// 	glColor3f(0.2,0.8,0.8);

	// 	glEnable(GL_MAP1_VERTEX_3);	   			

	// 	GLfloat ctrlpoints1[4][3] = { { 0.319, 1.820, 0.0198}, { 0.4795, 2.13215, 0.01225}, 
	// 								     { 0.640, 2.380, 0.0}, { 0.687, 2.781, 0.0} };

	// 	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints1[0][0]);

	// 	glBegin(GL_TRIANGLE_FAN);


	// 		glVertex3f(0.687, 2.781, 0.0);
	// 		//glVertex3f(0.319, 1.820, 0.0198);

	// 	   	for (int i = 0; i <= 30; i++) 
	// 	   	{
	// 			    glEvalCoord1f((GLfloat) i/30.0);
	// 	   	}
	// 		glVertex3f(0.319, 1.820, 0.0000);


	// 	glEnd();

	// 	GLfloat ctrlpoints2[4][3] = { { 0.319, 1.820, -0.0198}, { 0.4795, 2.13215, -0.01225}, 
	// 								     { 0.640, 2.380, 0.0}, { 0.687, 2.781, 0.0} };

	// 	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints2[0][0]);

	// 	glBegin(GL_TRIANGLE_FAN);

	// 		//glVertex3f(0.319, 1.820, -0.0198);
	// 		glVertex3f(0.319, 1.820,  0.0000);
	// 	   	for (int i = 0; i <= 30; i++) 
	// 		    glEvalCoord1f((GLfloat) i/30.0);



	// 	// glEnd();

	// 	glDisable(GL_MAP1_VERTEX_3);

	//  		glPopMatrix(); 
	glColor3f(0.5, 0.4, 0.8);

	glBegin(GL_POLYGON);
		glVertex3f(0.319, 1.326, 0.0198);
		glVertex3f(0.319, 1.820, 0.0198);
		glVertex3f(0.319, 1.820, -0.0198);
		glVertex3f(0.319, 1.326, -0.0198);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(0.3195, 1.326, 0.0198);
		glVertex3f(0.3195, 1.326, -0.0198);
		glVertex3f(0.0000, 1.326, -0.036);
		glVertex3f(0.0000, 1.326, 0.036);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(0.000, 0.000, 0.036);
		glVertex3f(0.000, 1.326, 0.036);
		glVertex3f(0.000, 1.326, -0.036);
		glVertex3f(0.000, 0.000, -0.036);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(0.510, 0.000, 0.0108);
		glVertex3f(0.510, 0.000, -0.0108);
		glVertex3f(0.000, 0.000, -0.0360);
		glVertex3f(0.000, 0.000, 0.0360);
	glEnd();

	glPushMatrix();
		glTranslatef(0.6875, 0, 0);
		glRotatef(-2.880, 0, 1, 0);
		glTranslatef(-0.6875, 0, 0);
		mainThornFace();
	glPopMatrix();
}

void createCorePilar(GLfloat top, GLfloat bottom, GLfloat sWall, GLfloat bWall)
{

	glBegin(GL_POLYGON);
		glVertex3f( bWall, bottom,  sWall);
		glVertex3f( bWall, bottom, -sWall);
		glVertex3f( bWall, top,    -sWall);
		glVertex3f( bWall, top,     sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f( bWall, bottom, -sWall);
		glVertex3f( sWall, bottom, -bWall);
		glVertex3f( sWall, top,    -bWall);
		glVertex3f( bWall, top,    -sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f( sWall, bottom, -bWall);
		glVertex3f(-sWall, bottom, -bWall);
		glVertex3f(-sWall, top,    -bWall);
		glVertex3f( sWall, top,    -bWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-bWall, bottom, -sWall);
		glVertex3f(-sWall, bottom, -bWall);
		glVertex3f(-sWall, top,    -bWall);
		glVertex3f(-bWall, top,    -sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-bWall, bottom,  sWall);
		glVertex3f(-bWall, bottom, -sWall);
		glVertex3f(-bWall, top,    -sWall);
		glVertex3f(-bWall, top,     sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f(-bWall, bottom, sWall);
		glVertex3f(-sWall, bottom, bWall);
		glVertex3f(-sWall, top,    bWall);
		glVertex3f(-bWall, top,    sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f( bWall, bottom, sWall);
		glVertex3f( sWall, bottom, bWall);
		glVertex3f( sWall, top,    bWall);
		glVertex3f( bWall, top,    sWall);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3f( sWall, bottom, bWall);
		glVertex3f(-sWall, bottom, bWall);
		glVertex3f(-sWall, top,    bWall);
		glVertex3f( sWall, top,    bWall);
	glEnd();
}

// Draw the Circle Wall, the roads to the tower
void drawIsengard()
{

	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
    gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

	// Draw the wall 
	glPushMatrix();

		glColor3f(0.9,0.9,0.9);
		glTranslatef(-isengardRadius, 1.125, 0);
		glScalef(1,5,1);
			
		glPushMatrix();
			do 
			{	
				isengardAngle += 0.1;

				glTranslatef(isengardRadius,0,0);
				glRotatef(isengardAngle, 0,1,0);
				glTranslatef(-isengardRadius,0,0);
				glutSolidCube(0.45); 
				
			}
			while (isengardAngle < 360);

		glPopMatrix();

		
		isengardAngle = 0;
		
	glPopMatrix();

	glPushMatrix();
		// Draw the roads to the tower

		glColor3f(0.8,0.8,0.8);

		glPushMatrix();	
			glScalef(isengardRadius*2, 0.025, 0.25);
			glutSolidCube(1);
		glPopMatrix();

		glRotatef(45, 0, 1, 0);
		glPushMatrix();	
			glScalef(isengardRadius*2, 0.025, 0.25);
			glutSolidCube(1);
		glPopMatrix();

		glRotatef(45, 0, 1, 0);
		glPushMatrix();	
			glScalef(isengardRadius*2, 0.025, 0.25);
			glutSolidCube(1);
		glPopMatrix();

		glRotatef(45, 0, 1, 0);
		glPushMatrix();	
			glScalef(isengardRadius*2, 0.025, 0.25);
			glutSolidCube(1);
		glPopMatrix();

	glPopMatrix();
}

// Draw the Tower itself, with the help of external functions
void drawTower(void)
{
	// Orthanc
	glPushMatrix();

		// Main base (dodecahedron)
		glPushMatrix();

			glColor3f(0.3, 0.2, 0.4);

			createCrux(0.750, 2.250, 0.000); // Base, floor	
			createWalls(0.750, 2.250, 0.325, 0.889, 0.000, 9.570);
			glColor3f(0.4, 0.3, 0.6);
			createCrux(0.325, 0.889, 9.570); // 1.326		
			glColor3f(0.3, 0.2, 0.4);
		
		glPopMatrix();

		glColor3f(0.4, 0.3, 0.6);
		
		glPushMatrix();
			int i;
			int space = 0.092;
			int times = 42;
			int endZ = 0;

			for (i = 0; i < times / 2 + 1; i++)
			{
				endZ = 0 - i*space;
					
				glBegin(GL_POLYGON);
					glVertex3f(1.123, 0.000, endZ);
					glVertex3f(1.403, 0.000, endZ);
					glVertex3f(1.346, 0.298, endZ);
					glVertex3f(1.252, 0.538, endZ);
					glVertex3f(1.235, 0.666, endZ);
					glVertex3f(1.235, 0.740, endZ);
					glVertex3f(1.217, 0.942, endZ);
					glVertex3f(1.180, 1.089, endZ);
					glVertex3f(1.142, 1.291, endZ);
					glVertex3f(1.020, 1.467, endZ);
				glEnd();
			
			}
			for (i = 0; i < times / 2; i++)
			{
				endZ = space + i*space;

				glBegin(GL_POLYGON);
					glVertex3f(1.123, 0.000, endZ);
					glVertex3f(1.403, 0.000, endZ);
					glVertex3f(1.346, 0.298, endZ);
					glVertex3f(1.252, 0.538, endZ);
					glVertex3f(1.235, 0.666, endZ);
					glVertex3f(1.235, 0.740, endZ);
					glVertex3f(1.217, 0.942, endZ);
					glVertex3f(1.180, 1.089, endZ);
					glVertex3f(1.142, 1.291, endZ);
					glVertex3f(1.020, 1.467, endZ);
				glEnd();
			
			}
		glPopMatrix();

		// Horizontal  Divisions, Bottom to 1.326
		glPushMatrix();

			glColor3f(0.4, 0.3, 0.5);

			// First (4-faced)
			createCrux(0.713, 2.470, 0.665); // Bottom
			createWalls(0.713, 2.470, 0.713, 2.470, 0.665, 0.739);
			createCrux(0.713, 2.470, 0.739); // 1.326

			// First
			createCrux(0.820, 2.280, 1.695); // Bottom
			createWalls(0.820, 2.280, 0.764, 2.185, 1.695, 1.750);
			createCrux(0.764, 2.185, 1.750); // 1.326

			// Second (4-faced)
			createCrux(0.650, 2.470, 1.972); // Bottom
			createWalls(0.650, 1.978, 0.650, 1.978, 1.972, 2.008);
			createCrux(0.650, 1.978, 2.008); // 1.326

			// Second
			createCrux(0.855, 2.143, 2.360); // Bottom
			createWalls(0.855, 2.143, 0.855, 2.143, 2.360, 2.380);
			createCrux(0.855, 2.143, 2.380); // 1.326

			// Third
			createCrux(0.845, 2.103, 2.708); // Bottom
			createWalls(0.845, 2.103, 0.845, 2.103, 2.708, 2.745);
			createCrux(0.845, 2.103, 2.745); // 1.326

			// Fourth
			createCrux(0.793, 2.000, 3.655); // Bottom
			createWalls(0.793, 2.000, 0.783, 1.885, 3.655, 3.715);
			createCrux(0.783, 1.885, 3.715); // 1.326

			// Fifth
			createCrux(0.774, 1.913, 3.998); // Bottom
			createWalls(0.774, 1.913, 0.770, 1.832, 3.998, 4.035);
			createCrux(0.770, 1.832, 4.035); // 1.326

			// Sixth
			createCrux(0.765, 1.913, 4.200); // Bottom
			createWalls(0.765, 1.913, 0.762, 1.794, 4.200, 4.292);
			createCrux(0.762, 1.794, 4.292); // 1.326

			// Seventh
			createCrux(0.741, 1.839, 4.734); // Bottom
			createWalls(0.741, 1.839, 0.741, 1.839, 4.734, 4.778);
			createCrux(0.741, 1.839, 4.778);

			// Eighth
			createCrux(0.722, 1.766, 5.271); // Bottom
			createWalls(0.722, 1.766, 0.718, 1.670, 5.271, 5.326);
			createCrux(0.718, 1.670, 5.326); // 1.326

			// Ninth
			createCrux(0.679, 1.662, 5.749); // Bottom
			createWalls(0.679, 1.662, 0.679, 1.662, 5.749, 5.786);
			createCrux(0.679, 1.662, 5.786); // 1.326

			// Tenth
			createCrux(0.635, 1.692, 6.098); // Bottom
			createWalls(0.635, 1.692, 0.631, 1.370, 6.098, 6.263);
			createCrux(0.631, 1.370, 6.263); // 1.326

			// Eleventh
			createCrux(0.550, 1.367, 7.846); // Bottom
			createWalls(0.550, 1.367, 0.550, 1.367, 7.846, 7.889);
			createWalls(0.550, 1.367, 0.408, 1.138, 7.889, 7.926);
			createCrux(0.408, 1.138, 7.926); // 1.326

			// Twelfth
			createCrux(0.502, 1.257, 8.657); // Bottom
			createWalls(0.502, 1.257, 0.502, 1.257, 8.657, 8.700);
			createCrux(0.502, 1.257, 8.700); // 1.326

		glPopMatrix(); // End of Horizontal Pieces

		// Vertical pieces
		glPushMatrix();

			glColor3f(0.4, 0.3, 0.6);

			// Between horizontals 1 and 2
			glPushMatrix();
				//X      Y      Z  	   depth  height width			
				pilasterStruct p12 = { 0.940, 2.040, 0.000, 0.100, 0.333, 0.010 };

				multiplyPilasters(13, 0.045, p12);	// Creates first group				
				p12.x = 0.0 - p12.x;				// move to the oposite
				p12.depth = 0.0 - p12.depth;		// x-side.
				multiplyPilasters(13, 0.045, p12);	// creates the second

				glRotatef(90, 0, 1, 0);				// Rotates 90 degrees

				multiplyPilasters(13, 0.045, p12);	// To apply the same
				p12.x = 0.0 - p12.x;				// process to the
				p12.depth = 0.0 - p12.depth;		// other two sides.
				multiplyPilasters(13, 0.045, p12);
	
			glPopMatrix();

			// Between horizontals 2 and 3
			glPushMatrix();
				//X      Y      Z  	   depth  height width			
				pilasterStruct p23 = { 0.932, 2.380, 0.000, 0.050, 0.333, 0.010 };

				multiplyPilasters(13, 0.045, p23);	// Creates first group				
				p23.x = 0.0 - p23.x;				// move to the oposite
				p23.depth = 0.0 - p23.depth;		// x-side.
				multiplyPilasters(13, 0.045, p23);	// creates the second

				glRotatef(90, 0, 1, 0);				// Rotates 90 degrees

				multiplyPilasters(13, 0.045, p23);	// To apply the same
				p23.x = 0.0 - p23.x;				// process to the
				p23.depth = 0.0 - p23.depth;		// other two sides.
				multiplyPilasters(13, 0.045, p23);

				pilasterStruct p23_2 = { 0.350, 2.380, 0, 0.050, 0.333, 0.010 };

				multiplyPilasters(42, 0.045, p23_2);// Same here, for the
				p23_2.x = 0.0 - p23_2.x;			// 8 internal faces
				p23_2.depth = 0.0 - p23_2.depth;
				multiplyPilasters(42, 0.045, p23_2);

				glRotatef(90, 0, 1, 0);				// Rotates 90 degrees

				multiplyPilasters(42, 0.045, p23_2);
				p23_2.x = 0.0 - p23_2.x;
				p23_2.depth = 0.0 - p23_2.depth;
				multiplyPilasters(42, 0.045, p23_2);

			glPopMatrix();

			glPushMatrix();
			// Between horizontals 6 and 7

				pilasterStruct p67 = { 0.800, 4.260, 0, 0.100, 0.511, 0.010 };

				multiplyPilasters(11, 0.045, p67);
				p67.x = 0.0 - p67.x;
				p67.depth = 0.0 - p67.depth;
				multiplyPilasters(11, 0.045, p67);

				glRotatef(90, 0, 1, 0);

				multiplyPilasters(11, 0.045, p67);
				p67.x = 0.0 - p67.x;
				p67.depth = 0.0 - p67.depth;
				multiplyPilasters(11, 0.045, p67);

				pilasterStruct p67_2 = { 0.250, 4.260, 0, 0.100, 0.511, 0.010 };

				multiplyPilasters(34, 0.045, p67_2);
				p67_2.x = 0.0 - p67_2.x;
				p67_2.depth = 0.0 - p67_2.depth;
				multiplyPilasters(34, 0.045, p67_2);

				glRotatef(90, 0, 1, 0);

				multiplyPilasters(34, 0.045, p67_2);
				p67_2.x = 0.0 - p67_2.x;
				p67_2.depth = 0.0 - p67_2.depth;
				multiplyPilasters(34, 0.045, p67_2);

			glPopMatrix();

			glPushMatrix();
				// Between horizontals 8 and 9
				pilasterStruct p89 = { 0.770, 5.749, 0, 0.110, 0.230, 0.010 };

				glTranslatef(0, 5.749 - 0.115, 0);
				glRotatef(180, 1, 0, 0);
				glTranslatef(0, -(5.749 - 0.115), 0);

				glTranslatef(0, 0.250, 0);

				multiplyPointyPilasters(11, 0.045, p89);
				p89.x = 0.0 - p89.x;
				p89.depth = 0.0 - p89.depth;
				multiplyPointyPilasters(11, 0.045, p89);

				glRotatef(90, 0, 1, 0);

				multiplyPointyPilasters(11, 0.045, p89);
				p89.x = 0.0 - p89.x;
				p89.depth = 0.0 - p89.depth;
				multiplyPointyPilasters(11, 0.045, p89);

				pilasterStruct p89_2 = { 0.250, 5.749, 0, 0.110, 0.230, 0.010 };

				multiplyPointyPilasters(34, 0.045, p89_2);
				p89_2.x = 0.0 - p89_2.x;
				p89_2.depth = 0.0 - p89_2.depth;
				multiplyPointyPilasters(34, 0.045, p89_2);

				glRotatef(90, 0, 1, 0);

				multiplyPointyPilasters(34, 0.045, p89_2);
				p89_2.x = 0.0 - p89_2.x;
				p89_2.depth = 0.0 - p89_2.depth;
				multiplyPointyPilasters(34, 0.045, p89_2);

			glPopMatrix();

			glPushMatrix();
				// Between horizontals 10 and 11
				pilasterStruct p1011 = { 0.510, 7.848, 0, 0.110, 0.230, 0.010 };

				glTranslatef(0, 7.848 - 0.110, 0);
				glRotatef(180, 1, 0, 0);
				glTranslatef(0, -(7.848 - 0.110), 0);

				glTranslatef(0, -0.218, 0);

				multiplyPointyPilasters(11, 0.045, p1011);
				p1011.x = 0.0 - p1011.x;
				p1011.depth = 0.0 - p1011.depth;
				multiplyPointyPilasters(11, 0.045, p1011);

				glRotatef(90, 0, 1, 0);

				multiplyPointyPilasters(11, 0.045, p1011);
				p1011.x = 0.0 - p1011.x;
				p1011.depth = 0.0 - p1011.depth;
				multiplyPointyPilasters(11, 0.045, p1011);

				pilasterStruct p1011_2 = { 0.160, 7.848, 0, 0.110, 0.230, 0.010 };

				multiplyPointyPilasters(24, 0.045, p1011_2);
				p1011_2.x = 0.0 - p1011_2.x;
				p1011_2.depth = 0.0 - p1011_2.depth;
				multiplyPointyPilasters(24, 0.045, p1011_2);

				glRotatef(90, 0, 1, 0);

				multiplyPointyPilasters(24, 0.045, p1011_2);
				p1011_2.x = 0.0 - p1011_2.x;
				p1011_2.depth = 0.0 - p1011_2.depth;
				multiplyPointyPilasters(24, 0.045, p1011_2);

			glPopMatrix();

		glPopMatrix();// End of Vertical Pieces

		// Pinnacle
		glPushMatrix();

			glColor3f(0.5, 0.4, 0.8);
			glTranslatef(0, 9.577, 0); // Move to the 1.326 of the tower

			// Main Thorns
			glPushMatrix();
				mainThornBody();
				glRotatef(90, 0, 1, 0);
				mainThornBody();
				glRotatef(90, 0, 1, 0);
				mainThornBody();
				glRotatef(90, 0, 1, 0);
				mainThornBody();
			glPopMatrix();

			// Core
			glColor3f(0.4, 0.4, 0.7);

			glBegin(GL_POLYGON);
				glVertex3f(0.3195, 1.326, -0.1385);
				glVertex3f(0.1385, 1.326, -0.3195);
				glVertex3f(-0.1385, 1.326, -0.3195);
				glVertex3f(-0.3195, 1.326, -0.1385);
				glVertex3f(-0.3195, 1.326, 0.1385);
				glVertex3f(-0.1385, 1.326, 0.3195);
				glVertex3f(0.1385, 1.326, 0.3195);
				glVertex3f(0.3195, 1.326, 0.1385);
			glEnd();

			// Core Walls (1st level)
			createCorePilar(1.326, 0.903, 0.1385, 0.3195);
			createCorePilar(0.903, 0.828, 0.1385, 0.2940);
			createCorePilar(0.828, 0.734, 0.1385, 0.2385);
			createCorePilar(0.734, 0.000, 0.1385, 0.2385);

		glPopMatrix(); // End of Pinnacle

	glPopMatrix();// End of Orthanc PushMatrix
}

void drawLines(void)
{
	/*------------------------------------------------------------*/
	// Draw the positive side of the lines x,y,z
	if (showAxis)
	{
		glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);			// Green for x axis
			glVertex3f(0, 0, 0);
			glVertex3f(1000, 0, 0);

			glColor3f(1.0, 0.0, 0.0);				// Red for y axis
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1000, 0);

			glColor3f(0.0, 0.0, 1.0);				// Blue for z axis
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1000);
		glEnd();

		// Dotted lines for the negative sides of x,y,z
		glEnable(GL_LINE_STIPPLE);				// Enable line stipple to use a 
		// dotted pattern for the lines
		glLineStipple(1, 0x0101);				// Dotted stipple pattern for the lines

		glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);			// Green for x axis
			glVertex3f(-1000, 0, 0);
			glVertex3f(0, 0, 0);
			glColor3f(1.0, 0.0, 0.0);				// Red for y axis
			glVertex3f(0, 0, 0);
			glVertex3f(0, -1000, 0);
			glColor3f(0.0, 0.0, 1.0);				// Blue for z axis
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, -1000);
		glEnd();

		glDisable(GL_LINE_STIPPLE);				// Disable the line stipple
	}
}

// Drawing routine.
void drawScene(void)
{

	// Place the camera view to the actual position
	glLoadIdentity();
	gluLookAt(
		x, 1.0 + y, z,
		x + lx , 1.0 + y + ly, lz + z,
		0.0, 1.0, 0.0);

	// Wireframe view Toggle
	if (!wireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//---- some attributes for skyboxtexture to happen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Activate Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Main Push,here the things are drawn
	glPushMatrix();

		// Lights
		lAmbient();
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT1);
		
		// Make sure it begins at the origin.
		glRotatef(rotX, 1.0, 0.0, 0.0);				// Rotate on x
		glRotatef(rotY, 0.0, 1.0, 0.0);				// Rotate on y
		glRotatef(rotZ, 0.0, 0.0, 1.0);				// Rotate on z
		glTranslatef(X, Y, Z);						// Translates the screen left or right,

		// Fog
		if (enableFog)
		{
			fog();
			glEnable(GL_FOG);
		}

		// Zoom adjusting
		glScalef(1 + zoom, 1 + zoom, 1 + zoom);

		glPushMatrix();
			createSkybox();
			drawLines();
			drawTower();
			drawGround();
			drawIsengard();
		glPopMatrix();

		// Particle system
		if (enableSnow)
		{
			 glPushMatrix();
				glTranslatef(0.0, 0.0, -10);
				glUpdateParticles();
				glDrawParticles();
			glPopMatrix();
		}

		glDisable(GL_FOG);

	// Modeling ends here.
	//------------------------------------------------------------------

	glFlush();
	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);		// Sets the background color
	glShadeModel(GL_FLAT);					// Set the Shading Model
	//glEnable(GL_LINE_SMOOTH);				// 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// Set Line Antialiasing

	//load images referent to each place on the texture vector
	// BMPs
	Image* image0 = loadBMP("nightsky_down.bmp");
	_textureId[0] = loadTexture(image0);
	delete image0;

	Image* image1 = loadBMP("nightsky_east.bmp");
	_textureId[1] = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("nightsky_north.bmp");
	_textureId[2] = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("nightsky_south.bmp");
	_textureId[3] = loadTexture(image3);
	delete image3;

	Image* image4 = loadBMP("nightsky_up.bmp");
	_textureId[4] = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("nightsky_west.bmp");
	_textureId[5] = loadTexture(image5);
	delete image5;
	
	// PNG
  	int width = 1151;
  	int height = 1151;
    bool hasAlpha = true;
    char filename[] = "shadow2.png";

    bool success = loadPngImage(filename, width, height, hasAlpha, &_textureShadow);
    if (!success) {
        std::cout << "Unable to load png file" << std::endl;
        return;
    }
    std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
 	 	
    glCreateParticles();

}

// OpenGL window reshape routine.
// The objects are created with dimensions relative to the paramethers
// passed on glOrtho or gluPerspective.
void resize(int w, int h)
{
	// Perspective view 
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = w / h;

	gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
	switch(key)
	{

		// x,X,y,Y,s,S uses the glRotatef() function
		case 'x':				// Rotates screen on x axis 
			rotX -= 0.5f;
		break;
		case 'X':				// Opposite way 
			rotX += 0.5f;
		break;
		case 'y':				// Rotates screen on y axis
			rotY -= 0.5f;
		break;
		case 'Y':				// Opposite way
			rotY += 0.5f;	
		break;	
		case 's':				// Rotates screen on z axis
			rotZ -= 0.5f;
		break;
		case 'S':				// Opposite way
			rotZ += 0.5f;
		break;

		// Changes the view from Fill mode to WireFrame
		case 'w':
		case 'W':
			if (!wireFrame)
				wireFrame = true;
			else
				wireFrame = false;
		break;


		// Changes the view from Fill mode to WireFrame
		case 'p':
		case 'P':
			if (!enableSnow)
				enableSnow = true;
			else
				enableSnow = false;
		break;

		// Toggles the view of the axis lines
		case 'l':
		case 'L':
			if (!showAxis)
				showAxis = true;
			else
				showAxis = false;
		break;

		// Toggles the view of the axis lines
		case 'f':
		case 'F':
			if (!enableFog)
				enableFog = true;
			else
				enableFog = false;
		break;

		// Default, resets the translations from starting view
		case 'c':				
		case 'C':		
			zoom = 0;
			X = Y = Z = x = y = z = 0.0f;
			rotX = 0.0f;
			rotY = 0.0f;
			rotZ = 0.0f;

			deltaMove = 0.0; // initially camera doesn't move
			strafe = 0.0;

			// Camera direction
			lx = 0.0, ly = 0.0, lz = 1.0f, hx =1.0, hz = 0.0f; 
			angle = 0.0, angleZ = 0.0; 
			deltaAngle = 0.0, deltaAngleZ = 0.0; 

			// Mouse drag control
			isDragging = 0;
			xDragStart = 0;
			zDragStart = 0;
	
	        glMatrixMode(GL_MODELVIEW);
	        glLoadIdentity(); 
		break;

			// Zoom in and out:
		case 'z':
			zoom += 0.02;
		break;

		case 'Z':
			zoom -= 0.02;
		break;

		// ESC for finishing the execution
		case 27:
			exit(0);
	}
}

// void detectColision()
// {

// 	// if ()
// 	// 	collisionX = true;

// 	if (!((z < boxSize - 1) && (z > -(boxSize - 1))))
// 		collisionZ = true;

// }

void pressSpecialKey(int key, int xx, int yy)
{
	switch (key) 
	{
		case GLUT_KEY_UP: 
			//if (!collisionZ) // Not coliding with the skybox
			//{
				//if(x < -2.2 || x > 2.2)
				//{
					deltaMove = 1.0;  
					lz = 1.0f; 
			//	}
				// else if ((z < -2.2 || z > 2.2) && (x > -2.2 && x < 2.2))
				// {
				// 	deltaMove = 1.0;  
				// 	lz = 1.0f; 
				// }
				
			//}
		break;

		case GLUT_KEY_DOWN:
			if (z > -(boxSize - 1))
			{
				if((x < -2.2 || x > 2.2) || ((z < -2.2 || z > 2.2) && (x > -2.2 && x < 2.2)))// Not coliding with the Tower
				{
					deltaMove = -1.0; 
					lz = 1.0f;
				}

			}  
		break;
		
		case GLUT_KEY_LEFT:
			if (x < boxSize - 1)  
				strafe = 1.0; 
		break;

		case GLUT_KEY_RIGHT: 
			if (x > -(boxSize - 1))
				strafe = -1.0; 
		break;

		case GLUT_KEY_PAGE_UP: 
			if (y < boxSize - 2)
				y++; 
		break;
		
		case GLUT_KEY_PAGE_DOWN: 
			if (y > 0)
				y--; 
		break;
	}
}

void releaseSpecialKey(int key, int x, int y)
{
	switch (key) 
	{
		case GLUT_KEY_UP: 
			deltaMove = 0.0;
		break;

		case GLUT_KEY_DOWN: 
			deltaMove = 0.0;  
		break;
		
		case GLUT_KEY_RIGHT: 
			strafe = 0.0; 
		break;

		case GLUT_KEY_LEFT: 
			strafe = 0.0; 
		break;
	}
}

void mouseMove(int x, int z)
{
	if (isDragging) 
	{   // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;
		deltaAngleZ = (z - zDragStart) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngle);
		lz = cos(angle + deltaAngle);
		ly = angleZ + deltaAngleZ;

		hx = cos(angle + deltaAngle);
		hz = hx*tan(angle + deltaAngle);

	}
}



void mouseButton(int button, int state, int x, int z)
{
	if (button == GLUT_LEFT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{ 					// left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
			zDragStart = z;
		}
		else  
		{ 						 // (state = GLUT_UP) 
			angle += deltaAngle; // update camera turning angle
			angleZ += deltaAngleZ;
			isDragging = 0; // no longer dragging
		}
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	// Version treatment
	//glutInitContextVersion(3, 0);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	// Window creation and settings
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
												// the GLUT_DEPTH causes
												// the oclusion of objects
												// when over each other
	glutInitWindowSize(1600, 1600); // redering window size
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Orthanc");

	// Cals for the created fucntions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutIdleFunc(update); // incremental update
	glutMouseFunc(mouseButton); // process mouse button push/release
	glutMotionFunc(mouseMove); // process mouse dragging motion
	//glutKeyboardFunc(keyInput);
	glutKeyboardFunc(processNormalKeys); // process standard key clicks
	glutSpecialFunc(pressSpecialKey); // process special key pressed
	glutSpecialUpFunc(releaseSpecialKey); // process special key releas

	setup();

	glutMainLoop();
}
