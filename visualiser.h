#ifndef VISUALISER_H
#define VISUALISER_H
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>
#include "wincompat.h"
#include <string.h>
struct ColorVec3f
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
};
int mainWindowInit(int argc, char **argv);
int V_ExecuteDisplay(int argc, char **argv);
void V_Init();
void V_Display();
void V_ReshapeFunc(int w, int h);
void V_DrawArray();
void glowLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat thickness, ColorVec3f color);
int A_InitAudio();
void ERRCHECK(FMOD_RESULT result);
void A_UpdateArray();

#endif // VISUALISER_H
