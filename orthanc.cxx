/*--------------------------------------------------------------------
#	Computer Graphics - COMP 371 - Final Project #1
# 	Students: Agustin Mariojouls, Mateus Machado Luna and Nicolas
#	
#	PLEASE READ THE readme.txt FILE FOR MORE INFO
#
--------------------------------------------------------------------*/

// OpenGL includes
#include <GL/glew.h>
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
static float orthoSize = 4;

int width, wh; 
int height, hw;

// Flags for visualization
bool wireFrame = false;			// Flag used for changing the glPolygonMode 
bool showAxis = true;			// Flag used for showing or not the axis lines

void drawTower(void)
{
	glPushMatrix();
		glColor3f(0.0, 0.8, 0.3);
		
		// Base, floor
		glBegin(GL_POLYGON);
			// Points	X     Y  	 Z	
			glVertex3f( -0.70,  0.00, -0.70);
			glVertex3f( -2.25,  0.00, -0.70);
			glVertex3f( -2.25,  0.00,  0.70);
			glVertex3f( -0.70,  0.00,  0.70);
			glVertex3f( -0.70,  0.00,  2.25);
			glVertex3f(  0.70,  0.00,  2.25);
			glVertex3f(  0.70,  0.00,  0.70);
			glVertex3f(  2.25,  0.00,  0.70);
			glVertex3f(  2.25,  0.00, -0.70);
			glVertex3f(  0.70,  0.00, -0.70);
			glVertex3f(  0.70,  0.00, -2.25);
			glVertex3f( -0.70,  0.00, -2.25);
		glEnd();
		
		glColor3f(0.9, 0.8, 0.3);
		
		// Top
		glBegin(GL_POLYGON);
			// Points	X     Y  	 Z	
			glVertex3f( -0.300,  18.00, -0.300);
			glVertex3f( -0.825,  18.00, -0.300);
			glVertex3f( -0.825,  18.00,  0.300);
			glVertex3f( -0.300,  18.00,  0.300);
			glVertex3f( -0.300,  18.00,  0.825);
			glVertex3f(  0.300,  18.00,  0.825);
			glVertex3f(  0.300,  18.00,  0.300);
			glVertex3f(  0.825,  18.00,  0.300);
			glVertex3f(  0.825,  18.00, -0.300);
			glVertex3f(  0.300,  18.00, -0.300);
			glVertex3f(  0.300,  18.00, -0.825);
			glVertex3f( -0.300,  18.00, -0.825);
		glEnd();

		glColor3f(0.3, 0.2, 0.3);

		// Laterals
		glBegin(GL_POLYGON);	
			glVertex3f( -0.300,  18.00, -0.300);
			glVertex3f( -0.825,  18.00, -0.300);	
			glVertex3f( -2.25,  0.00, -0.70);
			glVertex3f( -0.70,  0.00, -0.70);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f( -0.825,  18.00,  0.300);
			glVertex3f( -0.825,  18.00, -0.300);
			glVertex3f( -2.25,  0.00, -0.70);
			glVertex3f( -2.25,  0.00,  0.70);
		glEnd();
		
		glBegin(GL_POLYGON);
			glVertex3f( -0.825,  18.00,  0.300);
			glVertex3f( -0.300,  18.00,  0.300);
			glVertex3f( -0.70,  0.00,  0.70);
			glVertex3f( -2.25,  0.00,  0.70);
		glEnd();	

		glBegin(GL_POLYGON);
			glVertex3f( -0.300,  18.00,  0.825);
			glVertex3f( -0.300,  18.00,  0.300);
			glVertex3f( -0.70,  0.00,  0.70);
			glVertex3f( -0.70,  0.00,  2.25);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f( -0.300,  18.00,  0.825);
			glVertex3f(  0.300,  18.00,  0.825);
			glVertex3f(  0.70,  0.00,  2.25);
			glVertex3f( -0.70,  0.00,  2.25);
		glEnd();

		glBegin(GL_POLYGON);	
			glVertex3f(  0.300,  18.00,  0.300);
			glVertex3f(  0.825,  18.00,  0.300);
			glVertex3f(  2.25,  0.00,  0.70);
			glVertex3f(  0.70,  0.00,  0.70);
		glEnd();

		glBegin(GL_POLYGON);	
			glVertex3f(  0.300,  18.00,  0.300);
			glVertex3f(  0.300,  18.00,  0.825);
			glVertex3f(  0.70,  0.00,  2.25);
			glVertex3f(  0.70,  0.00,  0.70);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f( -0.300,  18.00, -0.825);
			glVertex3f( -0.300,  18.00, -0.300);
			glVertex3f( -0.70,  0.00, -0.70);
			glVertex3f( -0.70,  0.00, -2.25);			
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(  0.300,  18.00, -0.825);
			glVertex3f( -0.300,  18.00,  -0.825);
			glVertex3f( -0.70, 0.00, -2.25);
			glVertex3f(  0.70, 0.00, -2.25);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f( 0.300,  18.00, -0.825);
			glVertex3f( 0.300,  18.00, -0.300);
			glVertex3f( 0.70,  0.00, -0.70);
			glVertex3f( 0.70,  0.00, -2.25);			
		glEnd();

		glBegin(GL_POLYGON);	
			glVertex3f( 0.300,  18.00, -0.300);
			glVertex3f( 0.825,  18.00, -0.300);	
			glVertex3f( 2.25,  0.00, -0.70);
			glVertex3f( 0.70,  0.00, -0.70);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f( 0.825,  18.00,  0.300);
			glVertex3f( 0.825,  18.00, -0.300);
			glVertex3f( 2.25,  0.00, -0.70);
			glVertex3f( 2.25,  0.00,  0.70);
		glEnd();

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
		glViewport(0, 0, width / 2, height / 2);	
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
			glPushMatrix();
				glViewport(0, height / 2 + 1, width / 2 + 1, height / 2);
				glOrtho(-orthoSize, orthoSize, 0, 2*orthoSize, 0, 2*orthoSize);
				
				glRotatef(90, 0, -1, 0);
				
				drawTower();
				drawLines();
			glPopMatrix();

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
   //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// Set Line Antialiasing
}

// OpenGL window reshape routine.
// The objects are created with dimensions relative to the paramethers
// passed on glOrtho or gluPerspective.
void resize(int w, int h)
{

   	glViewport(0, 0, w, h);
   	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

   	float ratio = w*1.0/h;

	gluPerspective(5.0f, ratio, 0.1f, 1000.0f);
	// Set the camera
	gluLookAt(	20.0f, 20.0f, -40.0f, 	// eyeX, eyeY, eyeZ
				0.0f, 1.8f,  0.0f,		// centerX, centerY, centerZ
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
			zoom += 0.1 ;
		break;

		case 'Z':
			zoom -= 0.1;
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
   	glutInitWindowSize(1400, 1400); // redering window size
   	glutInitWindowPosition(100, 100); 
   	glutCreateWindow("Orthanc");

   	// Cals for the created fucntions
   	glutDisplayFunc(drawScene); 
  	glutReshapeFunc(resize);  
   	glutKeyboardFunc(keyInput);
   	glutSpecialFunc(specialKey);

   	glewInit();

   	setup(); 
   
   	glutMainLoop(); 
}
