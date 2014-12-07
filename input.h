#ifndef _INPUT_
#define _INPUT_

void processNormalKeys(unsigned char key, int xx, int yy);

void pressSpecialKey(int key, int xx, int yy);

void releaseSpecialKey(int key, int x, int y);

void mouseMove(int x, int z);

void mouseButton(int button, int state, int x, int z);

#endif
