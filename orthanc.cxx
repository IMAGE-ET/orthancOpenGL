/*--------------------------------------------------------------------
#	Computer Graphics - COMP 371 - Final Project
# 	Students: Agustin Mariojouls, Mateus Machado Luna and Nickauhla
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
GLfloat rotY = 0.0f;	// Rotate screen on y axis
GLfloat rotZ = 0.0f;		// Rotate screen on z axis
/*
static float cx = 0.0f;   	// Camera x
static float cy = 0.0f;   	// Camera y
static float cz = 0.0f;   	// Camera z

static float dx = 0.0f;   	// Line of sight vector  x
static float dy = 0.0f;   	// Line of sight vector y
static float dz = -1.0f;  	// Line of sight vector z
*/
static float angle = 0.0f;
static float zoom = 0.0; // Acumulated zoom factor

// Flags for visualization
bool wireFrame = false;			// Flag used for changing the glPolygonMode 
bool showAxis = false;			// Flag used for showing or not the axis lines
bool viewMode = false;			// Flag for switching between Perspec and Ortho

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

	// Reset transformations
	glLoadIdentity();

	if (!viewMode)	// PERSPECTIVE
	{
		// Set the camera
		gluLookAt(	20.0f, 6.0f, -8.0f, 	// eyeX, eyeY, eyeZ
					0.0f, 0.0f,  0.0f,		// centerX, centerY, centerZ
					0.0f, 1.0f,  0.0f);		// upX, upY, upZ
	}
	else // ORTHO
	{
		// Set the Ortho view initial position
		glTranslatef(0,-6,-24);
		glRotatef(225,0,1,0);
	}

	glEnable(GL_DEPTH_TEST);

	//------------------------------------------------------------------
	// Actual modeling begins here:

	glPushMatrix();	

		// Make sure it begins at the origin.
		glRotatef(rotX,1.0,0.0,0.0);				// Rotate on x
		glRotatef(rotY,0.0,1.0,0.0);				// Rotate on y
		glRotatef(rotZ,0.0,0.0,1.0);				// Rotate on z
		glTranslatef(X, Y, Z);						// Translates the screen left or right,
													// up or down or zoom in zoom out

		/*------------------------------------------------------------*/
		// Draw the positive side of the lines x,y,z

		if (showAxis)
		{
			glBegin(GL_LINES);
				glColor3f (0.0, 1.0, 0.0);			// Green for x axis
				glVertex3f(0,0,0);
				glVertex3f(100,0,0);

				glColor3f(1.0,0.0,0.0);				// Red for y axis
				glVertex3f(0,0,0);
				glVertex3f(0,100,0);
				
				glColor3f(0.0,0.0,1.0);				// Blue for z axis
				glVertex3f(0,0,0);	
				glVertex3f(0,0,100);
			glEnd();
			
			// Dotted lines for the negative sides of x,y,z
			glEnable(GL_LINE_STIPPLE);				// Enable line stipple to use a 
													// dotted pattern for the lines
			glLineStipple(1, 0x0101);				// Dotted stipple pattern for the lines
			glBegin(GL_LINES);				
				glColor3f (0.0, 1.0, 0.0);			// Green for x axis
				glVertex3f(-100,0,0);
				glVertex3f(0,0,0);
				glColor3f(1.0,0.0,0.0);				// Red for y axis
				glVertex3f(0,0,0);
				glVertex3f(0,-100,0);
				glColor3f(0.0,0.0,1.0);				// Blue for z axis
				glVertex3f(0,0,0);
				glVertex3f(0,0,-100);
			glEnd();

			glDisable(GL_LINE_STIPPLE);				// Disable the line stipple
		}
		/*------------------------------------------------------------------*/

		// Zoom adjusting
	    glScalef(1 + zoom, 1 + zoom, 1 + zoom);

		//------------------------------------------------------------------
	    // Hierarchy described on the readme.txt file

	    // Tower
	    glPushMatrix();

		    
		glPopMatrix();

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
   glEnable(GL_LINE_SMOOTH);				// 
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// Set Line Antialiasing
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

   	if (!viewMode)
   	{
   		gluPerspective(75.0f, ratio, 0.1f, 1000.0f);
   	}
   	else
   	{
   		glOrtho(0.0, w, h, 0.0, 0, 1);
   	}

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

		//O and P Toggles between Orthographic and Perspective view
		case 'o':
		case 'O':
			if (!viewMode)
				viewMode = true;
		break;

		case 'p':
		case 'P':
			if (viewMode)
				viewMode = false;
		break;
		/*
		case 'b': //Backward
			cx -= dx*0.1f;
			cz -= dz*0.1f;
			glLoadIdentity();
			gluLookAt(cx, cy, cz, cx , cy + dy, cz + dz, 0.0, 1.0, 0.0);
		break;

		case 'f': //Forward
			cx += dx*0.1f;
			cz += dz*0.1f;
			glLoadIdentity();
			gluLookAt(cx, cy, cz, cx, cy + dy, cz + dz, 0.0, 1.0, 0.0);
		break;
*/
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
   	glutInitWindowSize(1000, 1000); // redering window size
   	glutInitWindowPosition(100, 100); 
   	glutCreateWindow("Final Project - Orthanc");

	//glEnable(GL_LIGHTING);
    //	glEnable(GL_LIGHT0);

   	// Cals for the created fucntions
   	glutDisplayFunc(drawScene); 
   	glutReshapeFunc(resize);  
   	glutKeyboardFunc(keyInput);
   	glutSpecialFunc(specialKey);

   	glewExperimental = GL_TRUE;
   	glewInit();

   	setup(); 
   
   	glutMainLoop(); 
}
