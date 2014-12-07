#ifndef _TEXTURES_
#define _TEXTURES_

bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);

GLuint loadTexture(Image* image);

void loadAllTextures();

#endif
