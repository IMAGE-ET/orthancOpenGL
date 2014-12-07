/*--------------------------------------------------------------------
#	Computer Graphics - COMP 371 - Final Project #1
# 	Students: Agustin Mariojouls, Mateus Machado Luna and Nicolas
#
#	This is the main file
#
--------------------------------------------------------------------*/

//-INCLUDES-----------------------------------------------------------------------
#include "orthanc.h"

//-FUNCTIONS----------------------------------------------------------------------------//

// This function create the symetric Crux on a y-1.326 and a y-Height position
// The ext refer to the widt of the external points of the crux, and the int to
// the internal points of the crux 

//----FUNCTION-RELATED-TO-THE-CAMERA----------------------------------------------
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

// Drawing routine.
void drawScene(void)
{

	// Place the camera view to the actual position
	glLoadIdentity();

	gluLookAt(
				x, 1.0 + y, z,
				x + lx , 1.0 + y + ly, lz + z,
				0.0, 1.0, 0.0
			 );

	// Wireframe view Toggle
	if (!wireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Main Push,here the things are drawn
	glPushMatrix();

		// Lights and material
		mRusty();
		lAmbient();
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT1);
		
		// Make sure it begins at the origin, rotates the world
		glRotatef(rotX, 1.0, 0.0, 0.0);				// Rotate on x
		glRotatef(rotY, 0.0, 1.0, 0.0);				// Rotate on y
		glRotatef(rotZ, 0.0, 0.0, 1.0);				// Rotate on z
		glTranslatef(X, Y, Z);						// Translates the screen left or right,

		// Zoom adjusting
		glScalef(1 + zoom, 1 + zoom, 1 + zoom);

		// Fog
		if (enableFog)
		{
			fog();
			glEnable(GL_FOG);
		}

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


//-REMAINING-OPENGL-FUNCTIONS---------------------------------------------------
// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);		// Sets the background color
	glShadeModel(GL_FLAT);					// Set the Shading Model 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// Set Line Antialiasing

	// Load both bitmap and png textures;
	loadAllTextures();
		
 	// Load the particles system; 	
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
	glutIdleFunc(update); 					// incremental update
	glutMouseFunc(mouseButton); 			// process mouse button push/release
	glutMotionFunc(mouseMove);				// process mouse dragging motion
	glutKeyboardFunc(processNormalKeys); 	// process standard key clicks
	glutSpecialFunc(pressSpecialKey); 		// process special key pressed
	glutSpecialUpFunc(releaseSpecialKey); 	// process special key releas

	setup();

	glutMainLoop();
}
