#ifndef _TOWER_
#define _TOWER_

typedef struct _pilasterStruct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat depth;
	GLfloat height;
	GLfloat width;

} pilasterStruct;

void createCrux(GLfloat internal, GLfloat external, GLfloat height);

void createWalls(GLfloat intB, GLfloat extB, GLfloat intT, GLfloat extT, GLfloat bottom, GLfloat top);

void createPilaster(pilasterStruct p);

void createPilasterPointy(pilasterStruct p);

void multiplyPilasters(int times, GLfloat space, pilasterStruct p);

void multiplyPointyPilasters(int times, GLfloat space, pilasterStruct p);

void bezierPath(GLfloat Bx, GLfloat By,
	GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y,
	GLfloat C1x, GLfloat C1y, GLfloat C2x, GLfloat C2y);

void createThorn(GLfloat   Tx, GLfloat  Ty, 					// Tip
	GLfloat  P1x, GLfloat  P1y, GLfloat  P2x, GLfloat  P2y,
	GLfloat  C1x, GLfloat  C1y, GLfloat  C2x, GLfloat  C2y,
	GLfloat CT1x, GLfloat CT1y, GLfloat CT2x, GLfloat CT2y);

void mainThornFace();

void mainThornBody();

void createCorePilar(GLfloat top, GLfloat bottom, GLfloat sWall, GLfloat bWall);

void drawTower();

#endif
