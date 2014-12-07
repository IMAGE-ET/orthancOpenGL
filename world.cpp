#include "orthanc.h"

//----FUNCTIONS-RELATED-TO-THE-WORLD-AND-NOT-TOWER-STUFF----------------------------

// For building the walls of Isengard
float isengardRadius = 26;
float isengardAngle = 0;

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

// Rusty Material
void mRusty()
{
	glDisable(GL_COLOR_MATERIAL);

	GLfloat matAmbient [] = { 0.0, 0.0, 0.0 };
	GLfloat matDiffuse [] = { 1.0, 1.0, 1.0 };
	GLfloat matSpecular[] = { 0.0, 0.0, 0.0 };
	GLfloat matShininess = 0.2*128.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, matSpecular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
   	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

    return;
}
// Fog
void fog()
{
	GLfloat fogColor[] = {0.9f, 0.9f, 1.0f, 1};	//Sets the color for the fog

	glFogfv(GL_FOG_COLOR, fogColor);			//Modes for the fog are linear, exp and exp2
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 40.0f);
	glFogf(GL_FOG_DENSITY, 0.015f);
}

// Draw the positive side of the lines x,y,z
void drawLines(void)
{
	
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

// Creating skybox function
void createSkybox()
{
	glPushMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		// Render the front quad
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
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
// Draw the Ground Triangles Mesh and Shadow
void drawGround()
{
	if(toggleGrass)
	{
		glColor3f(0.4, 0.55, 0.25);

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
	
		// TOWER SHADOW PROJECTION
		glPushMatrix(); 

			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);

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
			
			glColor3f(1, 1, 1);

			glRotatef(45, 0, 1, 0);
			glTranslatef( 0.55, 0.009, -5.15);

			glBegin(GL_QUADS);
				glTexCoord2f(0, 0); 
				glVertex3f(0, 0, 0);
				glTexCoord2f(1, 0); 
				glVertex3f(0, 0, 10.1);
				glTexCoord2f(1, 1); 
				glVertex3f( -10.1, 0, 10.1);
				glTexCoord2f(0, 1); 
				glVertex3f( -10.1, 0, 0);
			glEnd();	

			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

		glPopMatrix();



	}
	else
	{
		// Draw the foor, with a mesh
		glPushMatrix();
			
			glTranslatef(-boxSize, 0, -boxSize);	

			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, _texture_Grass);			

			glBegin(GL_TRIANGLES);

				int x, z;
				
				for (x = 0; x < boxSize*2; x += 2)
				{
					for (z = 0; z < boxSize*2; z += 2)
					{
						glTexCoord2f(0, 0);
						glVertex3f(x,0,z);
						glTexCoord2f(1, 1);
						glVertex3f(x+2,0,z+2);
						glTexCoord2f(0, 1);
						glVertex3f(x,0,z+2);
					}

				}

				for (x = 0; x < boxSize*2; x += 2)
				{
					for (z = 0; z < boxSize*2; z += 2)
					{
						glTexCoord2f(1, 1);
						glVertex3f(x+2,0,z+2);
						glTexCoord2f(0, 0);
						glVertex3f(x,0,z);
						glTexCoord2f(1, 0);
						glVertex3f(x+2,0,z);
					}

				}

			glEnd();
			
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);

		glPopMatrix();
	}
	
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

		glColor3f(0.2,0.2,0.2);
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

	// glPushMatrix();
	// 	// Draw the roads to the tower

	// 	glColor3f(0.8,0.8,0.8);

	// 	glPushMatrix();	
	// 		glScalef(isengardRadius*2, 0.0025, 0.25);
	// 		glutSolidCube(1);
	// 	glPopMatrix();

	// 	glRotatef(45, 0, 1, 0);
	// 	glPushMatrix();	
	// 		glScalef(isengardRadius*2, 0.0025, 0.25);
	// 		glutSolidCube(1);
	// 	glPopMatrix();

	// 	glRotatef(45, 0, 1, 0);
	// 	glPushMatrix();	
	// 		glScalef(isengardRadius*2, 0.0025, 0.25);
	// 		glutSolidCube(1);
	// 	glPopMatrix();

	// 	glRotatef(45, 0, 1, 0);
	// 	glPushMatrix();	
	// 		glScalef(isengardRadius*2, 0.0025, 0.25);
	// 		glutSolidCube(1);
	// 	glPopMatrix();

	// glPopMatrix();
}
