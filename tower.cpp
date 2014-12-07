#include "orthanc.h"

// INITIALIZATION OF THE VARIABLES RELATED TO THE DRAWING AND VIEW
bool wireFrame   = false;		// Flag used for changing the glPolygonMode 
bool showAxis    = false;		// Flag used for showing or not the axis lines
bool enableFog   = false;		// Flag used for enable the fog (starts deactivated)
bool enableSnow  = true;		// Flag used for enable the snow (starts deactivated)
bool toggleGrass = false;		// Activate grass texture (not working with the shadow)

// Enviroment default
GLfloat X = 0.0f;			// Translate screen to x direction (left or right)
GLfloat Y = 0.0f;			// Translate screen to y direction (up or down)
GLfloat Z = 0.0f;			// Translate screen to z direction (zoom in or out)
GLfloat rotX = 0.0f;		// Rotate screen on x axis 
GLfloat rotY = 0.0f;		// Rotate screen on y axis
GLfloat rotZ = 0.0f;		// Rotate screen on z axis

// Camera position
float x = 0.0, y = 5.0, z = -18.0f;
float deltaMove = 0.0; // Initially camera doesn't move
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


//----FUNCTIONS-RELATED-TO-THE-TOWER-MODELING--------------------------------------

// Create a Dodecahedron with a Crux form
void createCrux(GLfloat internal, GLfloat external, GLfloat height)
{

	internal /= 2;
	external /= 2;

	// Stone Texture
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);

	// Dodecahedron
	glBegin(GL_POLYGON);

		// Points	 X          Y  	     Z	
		glTexCoord2f(0, 0);
		glVertex3f(-internal, height, -internal);
		glTexCoord2f(1, 0);
		glVertex3f(-external, height, -internal);
		glTexCoord2f(1, 1);
		glVertex3f(-external, height,  internal);
		glTexCoord2f(0, 1);
		glVertex3f(-internal, height,  internal);

		glTexCoord2f(0, 0);
		glVertex3f(-internal, height,  internal);
		glTexCoord2f(1, 0);
		glVertex3f(-internal, height,  external);
		glTexCoord2f(1, 1);
		glVertex3f( internal, height,  external);
		glTexCoord2f(0, 1);
		glVertex3f( internal, height,  internal);

		glTexCoord2f(1, 1);
		glVertex3f( internal, height,  internal);
		glTexCoord2f(1, 0);
		glVertex3f( external, height,  internal);
		glTexCoord2f(1, 1);
		glVertex3f( external, height, -internal);
		glTexCoord2f(0, 1);
		glVertex3f( internal, height, -internal);

		glTexCoord2f(1, 1);
		glVertex3f( internal, height, -internal);
		glTexCoord2f(1, 0);
		glVertex3f( internal, height, -external);
		glTexCoord2f(1, 1);
		glVertex3f(-internal, height, -external);
		glTexCoord2f(0, 1);
		glVertex3f(-internal, height, -internal);
	
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// This function create the symetric 12-sided wall between a y-1.326 and a y-Bottom
// The ext refer to the width of the external points of the crux, and the int to
// the internal points of the crux 
void createWalls(GLfloat intB, GLfloat extB, GLfloat intT, GLfloat extT, GLfloat bottom, GLfloat top)
{

	intB /= 2;
	extB /= 2;
	intT /= 2;
	extT /= 2;

	// Stone Texture
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);

	// Face A
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-intT, top, -intT);
		glTexCoord2f(2, 0);
		glVertex3f(-extT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(-extB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(-intB, bottom, -intB);
	glEnd();

	//  Face B
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[1]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-extT, top, intT);
		glTexCoord2f(2, 0);
		glVertex3f(-extT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(-extB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(-extB, bottom, intB);
	glEnd();

	//  Face C
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-extT, top, intT);
		glTexCoord2f(2, 0);
		glVertex3f(-intT, top, intT);
		glTexCoord2f(2, 8);
		glVertex3f(-intB, bottom, intB);
		glTexCoord2f(0, 8);
		glVertex3f(-extB, bottom, intB);
	glEnd();

	//  Face D
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-intT, top, extT);
		glTexCoord2f(2, 0);
		glVertex3f(-intT, top, intT);
		glTexCoord2f(2, 8);
		glVertex3f(-intB, bottom, intB);
		glTexCoord2f(0, 8);
		glVertex3f(-intB, bottom, extB);
	glEnd();

	//  Face E
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[1]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-intT, top, extT);
		glTexCoord2f(2, 0);
		glVertex3f(intT, top, extT);
		glTexCoord2f(2, 8);
		glVertex3f(intB, bottom, extB);
		glTexCoord2f(0, 8);
		glVertex3f(-intB, bottom, extB);
	glEnd();

	//  Face F
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(intT, top, intT);
		glTexCoord2f(2, 0);
		glVertex3f(intT, top, extT);
		glTexCoord2f(2, 8);
		glVertex3f(intB, bottom, extB);
		glTexCoord2f(0, 8);
		glVertex3f(intB, bottom, intB);
	glEnd();

	//  Face G
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(intT, top, intT);
		glTexCoord2f(2, 0);
		glVertex3f(extT, top, intT);
		glTexCoord2f(2, 8);
		glVertex3f(extB, bottom, intB);
		glTexCoord2f(0, 8);
		glVertex3f(intB, bottom, intB);
	glEnd();

	// Face H
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[1]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(extT, top, intT);
		glTexCoord2f(2, 0);
		glVertex3f(extT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(extB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(extB, bottom, intB);
	glEnd();

	// Face I
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(intT, top, -intT);
		glTexCoord2f(2, 0);
		glVertex3f(extT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(extB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(intB, bottom, -intB);
	glEnd();

	// Face J
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(intT, top, -extT);
		glTexCoord2f(2, 0);
		glVertex3f(intT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(intB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(intB, bottom, -extB);
	glEnd();

	//  Face K
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[1]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(intT, top, -extT);
		glTexCoord2f(2, 0);
		glVertex3f(-intT, top, -extT);
		glTexCoord2f(2, 8);
		glVertex3f(-intB, bottom, -extB);
		glTexCoord2f(0, 8);
		glVertex3f(intB, bottom, -extB);
	glEnd();

	//  Face L
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[1]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(-intT, top, -extT);
		glTexCoord2f(2, 0);
		glVertex3f(-intT, top, -intT);
		glTexCoord2f(2, 8);
		glVertex3f(-intB, bottom, -intB);
		glTexCoord2f(0, 8);
		glVertex3f(-intB, bottom, -extB);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// Creates a single simple rectangular pilaster
// x, y, and z refer to the inferior-left-insidetower point
void createPilaster(pilasterStruct p)
{
	// Stone Texture
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Internal Face
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y, p.z);
		glVertex3f(p.x, p.y, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glVertex3f(p.x, p.y + p.height, p.z);
	glEnd();

	// Top
	glBegin(GL_POLYGON);
		glVertex3f(p.x, p.y + p.height, p.z);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);
	glEnd();

	// Lateral 1
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x, p.y, p.z - p.width);
		glTexCoord2f(1, 0);
		glVertex3f(p.x, p.y + p.height, p.z - p.width);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glTexCoord2f(0, 1);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
	glEnd();

	// Lateral 2
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x, p.y, p.z);
		glTexCoord2f(1, 0);
		glVertex3f(p.x, p.y + p.height, p.z);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);
		glTexCoord2f(0, 1);
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
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x + p.depth, p.y, p.z);
		glTexCoord2f(1, 0);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width);
		glTexCoord2f(0, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// Creates a single simple pointing pilaster (1.326 thin, bottom width-size)
// x, y, and z refer to the inferior-left-insidetower point
void createPilasterPointy(pilasterStruct p)
{
	// Stone Texture
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Internal Face
	glBegin(GL_POLYGON);
	glVertex3f(p.x, p.y, p.z);
	glVertex3f(p.x, p.y, p.z - p.width);
	glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);// thin
	glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);// thin		
	glEnd();

	// Lateral 1
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x, p.y, p.z - p.width);
		glTexCoord2f(1, 0);
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glTexCoord2f(0, 1);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
	glEnd();

	// Lateral 2
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x, p.y, p.z);
		glTexCoord2f(1, 0);
		glVertex3f(p.x, p.y + p.height, p.z - p.width / 2);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glTexCoord2f(0, 1);
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
	glBindTexture(GL_TEXTURE_2D, _texture_stoneId[0]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(p.x + p.depth, p.y, p.z);
		glTexCoord2f(1, 0);
		glVertex3f(p.x + p.depth, p.y, p.z - p.width);
		glTexCoord2f(1, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);
		glTexCoord2f(0, 1);
		glVertex3f(p.x + p.depth, p.y + p.height, p.z - p.width / 2);

	glEnd();

	glDisable(GL_TEXTURE_2D);
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

	//glColor3f(0.15, 0.15, 0.15);

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

// NOT IMPLEMENTED
// void createBasePilaster(int endZ)
// {

// 	glBegin(GL_POLYGON);
// 		glVertex3f(1.123, 0.000, endZ);
// 		glVertex3f(1.403, 0.000, endZ);
// 		glVertex3f(1.346, 0.298, endZ);
// 		glVertex3f(1.252, 0.538, endZ);
// 		glVertex3f(1.235, 0.666, endZ);
// 		glVertex3f(1.235, 0.740, endZ);
// 		glVertex3f(1.217, 0.942, endZ);
// 		glVertex3f(1.180, 1.089, endZ);
// 		glVertex3f(1.142, 1.291, endZ);
// 		glVertex3f(1.020, 1.467, endZ);
// 	glEnd();
// }

// void multiplyBasePilasters(int times, GLfloat space, int endZ)
// {
// 	int i;

// 	for (i = 0; i < times / 2 + 1; i++)
// 	{
// 		endZ = 0 - i*space;
// 		createBasePilaster(endZ);			
// 	}
// 	for (i = 0; i < times / 2; i++)
// 	{
// 		endZ = space + i*space;
// 		createBasePilaster(endZ);			
// 	}
// }


// Draw the Tower itself, with the help of external functions
void drawTower(void)
{
	// for Texture blending with shades
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	// Orthanc
	glPushMatrix();

		// Main base (dodecahedron)
		glPushMatrix();

			glColor3f(0.15, 0.15, 0.15);

			createCrux(0.750, 2.250, 0.000); // Base, floor	
			createWalls(0.750, 2.250, 0.325, 0.889, 0.000, 9.570);
			glColor3f(0.4, 0.3, 0.6);
			createCrux(0.325, 0.889, 9.570); // 1.326		
			glColor3f(0.3, 0.2, 0.4);
		
		glPopMatrix();

		// Base pilasters NOT IMPLEMENTED
		// glPushMatrix();

		// 	multiplyBasePilasters(42, 0.0092, 0.0);

		// glPopMatrix();

		// Horizontal  Divisions, Bottom to 1.326
		glPushMatrix();

			glColor3f(0.12, 0.12, 0.12);

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

			glColor3f(0.15, 0.15, 0.15);

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

			glColor3f(0.1, 0.1, 0.1);
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
			glColor3f(0.15, 0.15, 0.15);

			// Octagon Texture
			glEnable(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, texture_octagon);
			glBegin(GL_POLYGON);
				glTexCoord2f(0.4334, 0);
				glVertex3f(0.3195, 1.326, -0.1385);  
				glTexCoord2f(0.5666, 1);
				glVertex3f(0.1385, 1.326, -0.3195);  
				glTexCoord2f(0, 1);
				glVertex3f(-0.1385, 1.326, -0.3195);
				glTexCoord2f(1, 0);
				glVertex3f(-0.3195, 1.326, -0.1385);
				glTexCoord2f(1, 0.5666);
				glVertex3f(-0.3195, 1.326, 0.1385);
				glTexCoord2f(0, 0.4334);
				glVertex3f(-0.1385, 1.326, 0.3195);
				glTexCoord2f(0.5666, 0.4334);
				glVertex3f(0.1385, 1.326, 0.3195);
				glTexCoord2f(0.4334, 0.5666);
				glVertex3f(0.3195, 1.326, 0.1385);
			glEnd();

			glDisable(GL_TEXTURE_2D);

			// Core Walls (1st level)
			createCorePilar(1.326, 0.903, 0.1385, 0.3195);
			createCorePilar(0.903, 0.828, 0.1385, 0.2940);
			createCorePilar(0.828, 0.734, 0.1385, 0.2385);
			createCorePilar(0.734, 0.000, 0.1385, 0.2385);

			glPopMatrix();
		glPopMatrix(); // End of Pinnacle


	glPopMatrix();// End of Orthanc PushMatrix
}
