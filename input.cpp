#include "orthanc.h"

//-FUNCTIONS-RELATED-TO-INPUT-PROCESS---------------------------------------------
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


		// Changes the Snow
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
		case 'g':
		case 'G':
			if (!toggleGrass)
				toggleGrass = true;
			else
				toggleGrass = false;
		break;

		// Toggles the view of the fog
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
			X = Y = Z = 0.0f;
			x = 0.0; y = 5.0; z = -18.0f;
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

void pressSpecialKey(int key, int xx, int yy)
{
	switch (key) 
	{
		case GLUT_KEY_UP: 
			if (z < boxSize - 1)   // Not coliding with the skybox
			{
				deltaMove = 1.0;  
				lz = 1.0f; 				
			}
		break;

		case GLUT_KEY_DOWN:
			if (z > -(boxSize - 1)) // Not coliding with the skybox
			{
				deltaMove = -1.0; 
				lz = 1.0f;
			}  
		break;
		
		case GLUT_KEY_LEFT:
			if (x < boxSize - 1)  	// Not coliding with the skybox
				strafe = 1.0; 
		break;

		case GLUT_KEY_RIGHT: 
			if (x > -(boxSize - 1))	// Not coliding with the skybox
				strafe = -1.0; 
		break;

		case GLUT_KEY_PAGE_UP: 
			if (y < boxSize - 2)	// Not coliding with the skybox
				y++; 
		break;
		
		case GLUT_KEY_PAGE_DOWN: 
			if (y > 0)				// Not coliding with the SkyBox
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
