/*--------------------------------------------------------------------
#	Computer Graphics - COMP 371 - Final Project #1
# 	Students: Agustin Mariojouls, Mateus Machado Luna and Nicolas
#	
#	PLEASE READ THE readme.txt FILE FOR MORE INFO
#
--------------------------------------------------------------------*/

// OpenGL includes
#include <GL/freeglut.h>
#include <GL/glext.h>

// Other libraries
#include <math.h> // needed for sin() and cos()

// Global Variables
GLfloat X = 0.0f;			// Translate screen to x direction (left or right)
GLfloat Y = 0.0f;			// Translate screen to y direction (up or down)
GLfloat Z = 0.0f;			// Translate screen to z direction (zoom in or out)
GLfloat rotX = 0.0f;		// Rotate screen on x axis 
GLfloat rotY = 0.0f;		// Rotate screen on y axis
GLfloat rotZ = 0.0f;		// Rotate screen on z axis
/*
static float cx = 0.0f;   	// Camera x
static float cy = 0.0f;   	// Camera y
static float cz = 0.0f;   	// Camera z

static float ex = 0.0f;   	// Eye x
static float ey = 0.0f;   	// Eye y
static float ez = 0.0f;  	// Eye
*/
static float angle = 0.0f;
static float zoom = 0.0; // Acumulated zoom factor
static float orthoSize = 2;

int width, wh; 
int height, hw;

// Flags for visualization
bool wireFrame = false;			// Flag used for changing the glPolygonMode 
bool showAxis = true;			// Flag used for showing or not the axis lines

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

// This function create the symetric Crux on a y-Top and a y-Height position
// The ext refer to the widt of the external points of the crux, and the int to
// the internal points of the crux 
void createCrux(GLfloat internal, GLfloat external, GLfloat height)
{

	internal /= 2;
	external /= 2;
	
	glBegin(GL_POLYGON);
		// Points	 X          Y  	     Z	
		glVertex3f( -internal,  height, -internal);
		glVertex3f( -external,  height, -internal);
		glVertex3f( -external,  height,  internal);
		glVertex3f( -internal,  height,  internal);
		glVertex3f( -internal,  height,  external);
		glVertex3f(  internal,  height,  external);
		glVertex3f(  internal,  height,  internal);
		glVertex3f(  external,  height,  internal);
		glVertex3f(  external,  height, -internal);
		glVertex3f(  internal,  height, -internal);
		glVertex3f(  internal,  height, -external);
		glVertex3f( -internal,  height, -external);
	glEnd();
}

// This function create the symetric 12-sided wall between a y-Top and a y-Bottom
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
		glVertex3f( -intT,  top, -intT);
		glVertex3f( -extT,  top, -intT);	
		glVertex3f( -extB,  bottom, -intB);
		glVertex3f( -intB,  bottom, -intB);
	glEnd();

	//  Face B
	glBegin(GL_POLYGON);
		glVertex3f( -extT,  top,  intT);
		glVertex3f( -extT,  top, -intT);
		glVertex3f( -extB,  bottom, -intB);
		glVertex3f( -extB,  bottom,  intB);
	glEnd();
	
	//  Face C
	glBegin(GL_POLYGON);
		glVertex3f( -extT,  top,  intT);
		glVertex3f( -intT,  top,  intT);
		glVertex3f( -intB,  bottom,  intB);
		glVertex3f( -extB,  bottom,  intB);
	glEnd();	

	//  Face D
	glBegin(GL_POLYGON);
		glVertex3f( -intT,  top,  extT);
		glVertex3f( -intT,  top,  intT);
		glVertex3f( -intB,  bottom,  intB);
		glVertex3f( -intB,  bottom,  extB);
	glEnd();

	//  Face E
	glBegin(GL_POLYGON);
		glVertex3f( -intT,  top,  extT);
		glVertex3f(  intT,  top,  extT);
		glVertex3f(  intB,  bottom,  extB);
		glVertex3f( -intB,  bottom,  extB);
	glEnd();

	//  Face F
	glBegin(GL_POLYGON);  	
		glVertex3f(  intT,  top,  intT);
		glVertex3f(  intT,  top,  extT);
		glVertex3f(  intB,  bottom,  extB);
		glVertex3f(  intB,  bottom,  intB);
	glEnd();

	//  Face G
	glBegin(GL_POLYGON);	
		glVertex3f(  intT,  top,  intT);
		glVertex3f(  extT,  top,  intT);
		glVertex3f(  extB,  bottom,  intB);
		glVertex3f(  intB,  bottom,  intB);
	glEnd();
	
	// Face H
	glBegin(GL_POLYGON);
		glVertex3f( extT,  top,  intT);
		glVertex3f( extT,  top, -intT);
		glVertex3f( extB,  bottom, -intB);
		glVertex3f( extB,  bottom,  intB);
	glEnd();

	// Face I
	glBegin(GL_POLYGON);	
		glVertex3f( intT,  top, -intT);
		glVertex3f( extT,  top, -intT);	
		glVertex3f( extB,  bottom, -intB);
		glVertex3f( intB,  bottom, -intB);
	glEnd();

	// Face J
	glBegin(GL_POLYGON);
		glVertex3f( intT,  top, -extT);
		glVertex3f( intT,  top, -intT);
		glVertex3f( intB,  bottom, -intB);
		glVertex3f( intB,  bottom, -extB);			
	glEnd();

	//  Face K
	glBegin(GL_POLYGON);
		glVertex3f(  intT,  top, -extT);
		glVertex3f( -intT,  top,  -extT);
		glVertex3f( -intB, bottom, -extB);
		glVertex3f(  intB, bottom, -extB);
	glEnd();

	//  Face L
	glBegin(GL_POLYGON);
		glVertex3f( -intT,  top, -extT);
		glVertex3f( -intT,  top, -intT);
		glVertex3f( -intB,  bottom, -intB);
		glVertex3f( -intB,  bottom, -extB);			
	glEnd();
}

// Creates a single simple rectangular pilaster
// x, y, and z refer to the inferior-left-insidetower point
void createPilaster(pilasterStruct p)
{
	// Internal Face
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y         , p.z        );
		glVertex3f( p.x        ,  p.y         , p.z - p.width);
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x        ,  p.y + p.height, p.z        );		
	glEnd();

	// Top
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y + p.height, p.z        );
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z        );		
	glEnd();

	// Lateral 1
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y         , p.z - p.width);
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width);	
		glVertex3f( p.x + p.depth,  p.y         , p.z - p.width);
	glEnd();

	// Lateral 2
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y         , p.z        );
		glVertex3f( p.x        ,  p.y + p.height, p.z        );
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z        );	
		glVertex3f( p.x + p.depth,  p.y         , p.z        );	
	glEnd();

	// Bottom
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y         , p.z        );
		glVertex3f( p.x        ,  p.y         , p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y         , p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y         , p.z        );		
	glEnd();

	// External Face
	glBegin(GL_POLYGON);
		glVertex3f( p.x + p.depth,  p.y         , p.z        );	
		glVertex3f( p.x + p.depth,  p.y         , p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z        );

	glEnd();

}

// Creates a single simple pointing pilaster (top thin, bottom width-size)
// x, y, and z refer to the inferior-left-insidetower point
void createPilasterPointy(pilasterStruct p)
{
	// Internal Face
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y           , p.z            );
		glVertex3f( p.x        ,  p.y           , p.z - p.width  );
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width/2);// thin
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width/2);// thin		
	glEnd();

/*	// Top
	glBegin(GL_POLYGON);	
		glVertex3f( p.x        ,  p.y + p.height, p.z        );
		glVertex3f( p.x        ,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z        );		
	glEnd();
*/
	// Lateral 1
	glBegin(GL_POLYGON);	
		glVertex3f( p.x          ,  p.y           , p.z - p.width  );
		glVertex3f( p.x          ,  p.y + p.height, p.z - p.width/2);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width/2);	
		glVertex3f( p.x + p.depth,  p.y           , p.z - p.width  );
	glEnd();

	// Lateral 2
	glBegin(GL_POLYGON);	
		glVertex3f( p.x          ,  p.y           , p.z          );
		glVertex3f( p.x          ,  p.y + p.height, p.z - p.width/2);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width/2);	
		glVertex3f( p.x + p.depth,  p.y           , p.z          );	
	glEnd();

	// Bottom
	glBegin(GL_POLYGON);	
		glVertex3f( p.x          ,  p.y         , p.z          );
		glVertex3f( p.x          ,  p.y         , p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y         , p.z - p.width);
		glVertex3f( p.x + p.depth,  p.y         , p.z          );		
	glEnd();

	// External Face
	glBegin(GL_POLYGON);
		glVertex3f( p.x + p.depth,  p.y           , p.z            );	
		glVertex3f( p.x + p.depth,  p.y           , p.z - p.width  );
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width/2);
		glVertex3f( p.x + p.depth,  p.y + p.height, p.z - p.width/2);

	glEnd();

}

// Multiply (and create) pilasters created with the struct pilasterStruct
// Uses two loops for creating exactly from the center.
void multiplyPilasters(int times, GLfloat space, pilasterStruct p)
{
	int i;

	for (i = 0 ; i < times/2 + 1; i++)
	{
		p.z = 0 - i*space;
		createPilaster(p);
	}
	for (i = 0 ; i < times/2; i++)
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

	for (i = 0 ; i < times/2 + 1; i++)
	{
		p.z = 0 - i*space;
		createPilasterPointy(p);
	}
	for (i = 0 ; i < times/2; i++)
	{
		p.z = space + i*space;
		createPilasterPointy(p);
	}
}

// Creates a bezier path in a 2D space;
// The first point is the base point, the next two are the extremes points, 
// and the last two are control ones;
void bezierPath( GLfloat Bx, GLfloat By,
				 GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y,
				 GLfloat C1x, GLfloat C1y, GLfloat C2x, GLfloat C2y)
{

	GLfloat ctrlpoints[4][3] = { { P1x, P1y, 0.0}, { C1x, C1y, 0.0}, 
							     { C2x, C2y, 0.0}, { P2x, P2y, 0.0} };

	glEnable(GL_MAP1_VERTEX_3);
   	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

   	glBegin(GL_TRIANGLE_FAN);
   	glVertex3f(Bx, By, 0.0);
	   	for (int i = 0; i <= 30; i++) 
		    glEvalCoord1f((GLfloat) i/30.0);
	glEnd();
}

// Creates a thorn
void createThorn(	GLfloat   Tx, GLfloat  Ty, 							// Tip
					GLfloat  P1x, GLfloat  P1y, GLfloat  P2x, GLfloat  P2y,	
				 	GLfloat  C1x, GLfloat  C1y, GLfloat  C2x, GLfloat  C2y,
				 	GLfloat CT1x, GLfloat CT1y, GLfloat CT2x, GLfloat CT2y)
{
	bezierPath(  P2x,  P2y, 
				  Tx,   Ty, P1x, P1y,
				CT1x, CT1y, C1x, C1y);

	GLfloat ctrlpoints[4][3] = { { P2x, P2y, 0.0}, { C2x, C2y, 0.0}, 
							     { CT2x, CT2y, 0.0}, { Tx, Ty, 0.0} };

	glEnable(GL_MAP1_VERTEX_3);
   	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

   	glBegin(GL_POLYGON);
   	   	for (int i = 0; i <= 30; i++) 
		    glEvalCoord1f((GLfloat) i/30.0);
	glEnd();


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
			createCrux(0.325, 0.889, 9.570); // Top		
			glColor3f(0.3, 0.2, 0.4);
		glPopMatrix();

		// Horizontal  Divisions, Bottom to Top
		glPushMatrix();
			
			glColor3f(0.4, 0.3, 0.5);

			// First (4-faced)
			createCrux(0.713, 2.470, 0.665); // Bottom
			createWalls(0.713, 2.470, 0.713, 2.470, 0.665, 0.739);
			createCrux(0.713, 2.470, 0.739); // Top

			// First
			createCrux(0.820, 2.280, 1.695); // Bottom
			createWalls(0.820, 2.280, 0.764, 2.185, 1.695, 1.750);
			createCrux(0.764, 2.185, 1.750); // Top

			// Second (4-faced)
			createCrux(0.650, 2.470, 1.972); // Bottom
			createWalls(0.650, 1.978, 0.650, 1.978, 1.972, 2.008);
			createCrux(0.650, 1.978, 2.008); // Top

			// Second
			createCrux(0.855, 2.143, 2.360); // Bottom
			createWalls(0.855, 2.143, 0.855, 2.143, 2.360, 2.380);
			createCrux(0.855, 2.143, 2.380); // Top

			// Third
			createCrux(0.845, 2.103, 2.708); // Bottom
			createWalls(0.845, 2.103, 0.845, 2.103, 2.708, 2.745);
			createCrux(0.845, 2.103, 2.745); // Top

			// Fourth
			createCrux(0.793, 2.000, 3.655); // Bottom
			createWalls(0.793, 2.000, 0.783, 1.885, 3.655, 3.715);
			createCrux(0.783, 1.885, 3.715); // Top

			// Fifth
			createCrux(0.774, 1.913, 3.998); // Bottom
			createWalls(0.774, 1.913, 0.770, 1.832, 3.998, 4.035);
			createCrux(0.770, 1.832, 4.035); // Top

			// Sixth
			createCrux(0.765, 1.913, 4.200); // Bottom
			createWalls(0.765, 1.913, 0.762, 1.794, 4.200, 4.292);
			createCrux(0.762, 1.794, 4.292); // Top

			// Seventh
			createCrux(0.741, 1.839, 4.734); // Bottom
			createWalls(0.741, 1.839, 0.741, 1.839, 4.734, 4.778);
			createCrux(0.741, 1.839, 4.778);

			// Eighth
			createCrux(0.722, 1.766, 5.271); // Bottom
			createWalls(0.722, 1.766, 0.718, 1.670, 5.271, 5.326);
			createCrux(0.718, 1.670, 5.326); // Top

			// Ninth
			createCrux(0.679, 1.662, 5.749); // Bottom
			createWalls(0.679, 1.662, 0.679, 1.662, 5.749, 5.786);
			createCrux(0.679, 1.662, 5.786); // Top

			// Tenth
			createCrux(0.635, 1.692, 6.098); // Bottom
			createWalls(0.635, 1.692, 0.631, 1.370, 6.098, 6.263);
			createCrux(0.631, 1.370, 6.263); // Top

			// Eleventh
			createCrux(0.550, 1.367, 7.846); // Bottom
			createWalls(0.550, 1.367, 0.550, 1.367, 7.846, 7.889);
			createWalls(0.550, 1.367, 0.408, 1.138, 7.889, 7.926);
			createCrux(0.408, 1.138, 7.926); // Top

			// Twelfth
			createCrux(0.502, 1.257, 8.657); // Bottom
			createWalls(0.502, 1.257, 0.502, 1.257, 8.657, 8.700);
			createCrux(0.502, 1.257, 8.700); // Top

		glPopMatrix();

		// Vertical pieces
		glPushMatrix();

			glColor3f(0.4, 0.3, 0.6);

			// Between horizontals 1 and 2
			glPushMatrix();
									//X      Y      Z  	   depth  height width			
				pilasterStruct p12 = {0.940, 2.040, 0.000, 0.100, 0.333, 0.010};

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
				pilasterStruct p23 = {0.932, 2.380, 0.000, 0.050, 0.333, 0.010};

				multiplyPilasters(13, 0.045, p23);	// Creates first group				
				p23.x = 0.0 - p23.x;				// move to the oposite
				p23.depth = 0.0 - p23.depth;		// x-side.
				multiplyPilasters(13, 0.045, p23);	// creates the second

				glRotatef(90, 0, 1, 0);				// Rotates 90 degrees

				multiplyPilasters(13, 0.045, p23);	// To apply the same
				p23.x = 0.0 - p23.x;				// process to the
				p23.depth = 0.0 - p23.depth;		// other two sides.
				multiplyPilasters(13, 0.045, p23);

				pilasterStruct p23_2 = {0.350, 2.380, 0, 0.050, 0.333, 0.010};

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

				pilasterStruct p67 = {0.800, 4.260, 0, 0.100, 0.511, 0.010};

				multiplyPilasters(11, 0.045, p67);
				p67.x = 0.0 - p67.x;
				p67.depth = 0.0 - p67.depth;
				multiplyPilasters(11, 0.045, p67);

				glRotatef(90, 0, 1, 0);

				multiplyPilasters(11, 0.045, p67);
				p67.x = 0.0 - p67.x;
				p67.depth = 0.0 - p67.depth;
				multiplyPilasters(11, 0.045, p67);

				pilasterStruct p67_2 = {0.250, 4.260, 0, 0.100, 0.511, 0.010};

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
				pilasterStruct p89 = {0.770, 5.749, 0, 0.110, 0.230, 0.010};

				glTranslatef(0,5.749 - 0.115,0);
				glRotatef(180, 1, 0, 0);
				glTranslatef(0,-(5.749 - 0.115),0);

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

				pilasterStruct p89_2 = {0.250, 5.749, 0, 0.110, 0.230, 0.010};

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
				pilasterStruct p1011 = {0.510, 7.848, 0, 0.110, 0.230, 0.010};

				glTranslatef(0,7.848 - 0.110,0);
				glRotatef(180, 1, 0, 0);
				glTranslatef(0,-(7.848 - 0.110),0);

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

				pilasterStruct p1011_2 = {0.160, 7.848, 0, 0.110, 0.230, 0.010};

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

		glPopMatrix();

		// Pinnacle
		glPushMatrix();

			glColor3f(0.5, 0.4, 0.8);
			glTranslatef(0, 9.577, 0); // Move to the top of the tower

			// Main Thorns
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
		   		bezierPath( 0.687, 1.820,				// Base Point
		   					0.319, 1.820, 0.687, 2.781,	// Curve Extremes
		   					0.570, 2.110, 0.640, 2.380);// Curve 
		
				// Curve between the bigger Thorn and the center
		   		bezierPath( 0.515, 0.864,				// Base Point
		   					0.687, 0.864, 0.515, 0.595,	// Curve Extremes
		   					0.580, 0.810, 0.540, 0.720);// Curve 

		   		// Lateral Thorn
		   		createThorn( 	0.687, 0.478,
		   						0.515, 0.311, 0.548, 0.106,
		   						0.600, 0.260, 0.630, 0.230,
		   						0.660, 0.400, 0.660, 0.330);

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

void drawLines(void)
{
	/*------------------------------------------------------------*/
	// Draw the positive side of the lines x,y,z
	if (showAxis)
	{
		glBegin(GL_LINES);
			glColor3f (0.0, 1.0, 0.0);			// Green for x axis
			glVertex3f(0,0,0);
			glVertex3f(1000,0,0);

			glColor3f(1.0,0.0,0.0);				// Red for y axis
			glVertex3f(0,0,0);
			glVertex3f(0,1000,0);
					
			glColor3f(0.0,0.0,1.0);				// Blue for z axis
			glVertex3f(0,0,0);	
			glVertex3f(0,0,1000);
		glEnd();
				
		// Dotted lines for the negative sides of x,y,z
		glEnable(GL_LINE_STIPPLE);				// Enable line stipple to use a 
														// dotted pattern for the lines
		glLineStipple(1, 0x0101);				// Dotted stipple pattern for the lines
		
		glBegin(GL_LINES);				
			glColor3f (0.0, 1.0, 0.0);			// Green for x axis
			glVertex3f(-1000,0,0);
			glVertex3f(0,0,0);
			glColor3f(1.0,0.0,0.0);				// Red for y axis
			glVertex3f(0,0,0);
			glVertex3f(0,-1000,0);
			glColor3f(0.0,0.0,1.0);				// Blue for z axis
			glVertex3f(0,0,0);
			glVertex3f(0,0,-1000);
		glEnd();

		glDisable(GL_LINE_STIPPLE);				// Disable the line stipple
	}

}

// Drawing routine.
void drawScene(void)
{   
	if (!wireFrame)
    	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
    	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Needed for using quadratic objects
	GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();

	glEnable(GL_DEPTH_TEST);

	glPushMatrix();	

		// Make sure it begins at the origin.
		glRotatef(rotX,1.0,0.0,0.0);				// Rotate on x
		glRotatef(rotY,0.0,1.0,0.0);				// Rotate on y
		glRotatef(rotZ,0.0,0.0,1.0);				// Rotate on z
		glTranslatef(X, Y, Z);						// Translates the screen left or right,
													// up or down or zoom in zoom out
		/*------------------------------------------------------------------*/

		// Zoom adjusting
	    glScalef(1 + zoom, 1 + zoom, 1 + zoom);

	 	width = glutGet(GLUT_WINDOW_WIDTH);
	    height = glutGet(GLUT_WINDOW_HEIGHT);

		hw = height/(float) width;
		wh = width/(float) height;

		/* Perspective view */
		glViewport(0, 0, width / 2, height );	
		glPushMatrix();
			//glRotatef(0, 1, 0, 0);
			//glRotatef(0, 0, 1, 0);
			drawLines();
			drawTower();
		glPopMatrix();

		/* Orthogonal projection */
		glMatrixMode(GL_PROJECTION);
	
		glPushMatrix();

			glLoadIdentity();	
			
			glMatrixMode(GL_MODELVIEW);

			/* Right view */		
/*			glPushMatrix();
				glViewport(0, height / 2 + 1, width / 2 + 1, height / 2);
				glOrtho(-orthoSize, orthoSize, 0, 2*orthoSize, 0, 2*orthoSize);
				
				glRotatef(90, 0, -1, 0);
				
				drawTower();
				drawLines();
			glPopMatrix();
*/
			/* Face view */
			glPushMatrix();
				glViewport(width / 2 + 1, height / 2 + 1, width / 2, height / 2);			
				glOrtho(-orthoSize, orthoSize, 0, 2*orthoSize, 0, 2*orthoSize);

				drawTower();
				drawLines();
			glPopMatrix();

			/* Top view */
			glPushMatrix();
				glViewport(width / 2 + 1, 0, width / 2, height / 2);
				glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0, 2*orthoSize);
				
				glRotatef(-90, 1, 0, 0);
				glRotatef(-90, 0, 1, 0);
				glRotatef(180, 0, 0, 1);
				
				drawTower();
				drawLines();
			glPopMatrix();

			glMatrixMode(GL_PROJECTION);

		glPopMatrix();
	
		glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

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
}

// OpenGL window reshape routine.
// The objects are created with dimensions relative to the paramethers
// passed on glOrtho or gluPerspective.
void resize(int w, int h)
{

   	//glViewport(0, 0, w, h);
   	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

   	float ratio = w*0.5/h; // half because the window has double heightZZ

	gluPerspective(5.0f, ratio, 0.1f, 1000.0f);
	// Set the camera
	gluLookAt(	140.0f, 100.0f, -140.0f, 	// eyeX, eyeY, eyeZ
				0.0f, 9.0f,  0.0f,		// centerX, centerY, centerZ
				0.0f, 1.0f,  0.0f);		// upX, upY, upZ

   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
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

		// Default, resets the translations from starting view
		case 'c':				
		case 'C':		
			zoom = 0;
			X = Y = 0.0f;
			Z = 0.0f;
			rotX = 0.0f;
			rotY = 0.0f;
			rotZ = 0.0f;
			
	        glMatrixMode(GL_MODELVIEW);
	        glLoadIdentity(); 
		break;

		// Changes the view from Fill mode to WireFrame
		case 'w':
		case 'W':
			if (!wireFrame)
				wireFrame = true;
			else
				wireFrame = false;
		break;

		// Toggles the view of the axis lines
		case 'l':
		case 'L':
			if (!showAxis)
				showAxis = true;
			else
				showAxis = false;
		break;

		// Zoom in and out:
		case 'z':
			zoom += 0.02 ;
		break;

		case 'Z':
			zoom -= 0.02;
		break;

		// ESC for finishing the execution
		case 27:
			exit(0);			
   }

   glutPostRedisplay();		
}

// called on special key pressed
void specialKey(int key, int x, int y) { 
  
	// The keys below are using the gluLookAt() function for navigation
	// Check which key is pressed
    switch(key) {
		case GLUT_KEY_LEFT :    // Rotate on x axis
			X -= 0.1f;
          break;
        case GLUT_KEY_RIGHT :	// Rotate on x axis (opposite)
			X +=  0.1f;
          break;
        case GLUT_KEY_UP :		// Rotate on y axis 
			Y += 0.1f;
          break;
        case GLUT_KEY_DOWN :	// Rotate on y axis (opposite)
			Y -= 0.1f;
          break;	
		case GLUT_KEY_PAGE_UP:  // Roatae on z axis
			Z -= 0.1f;
		  break;
		case GLUT_KEY_PAGE_DOWN:// Roatae on z axis (opposite)
			Z += 0.1f;
		  break;
    }
	glutPostRedisplay();		// Redraw the scene
}

// Main routine.
int main(int argc, char **argv) 
{
	glutInit(&argc, argv);

   	// Version treatment
   	//glutInitContextVersion(3, 0);
   	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

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
   	glutKeyboardFunc(keyInput);
   	glutSpecialFunc(specialKey);

   	setup(); 
   
   	glutMainLoop(); 
}
