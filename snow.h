#ifndef _SNOW_
#define _SNOW_

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
	bool   Visible;

}PARTICLES;


const int ParticleCount = 8000; // Particle system, amount of snow

static PARTICLES Particle[ParticleCount];

void glCreateParticles (void);


void glUpdateParticles (void);


void glDrawParticles (void);

#endif
