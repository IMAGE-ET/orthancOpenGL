//----FUNCTIONS-RELATED-TO-THE-PARTICLES-SYSTEM---------------------------------------

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
