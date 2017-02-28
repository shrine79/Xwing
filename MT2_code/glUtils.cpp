
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "glut.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

#include "utility.h"

#include "glUtils.h"

/****		Handy types		****/

RGBA White = { 1, 1, 1, 1 };
RGBA Black = { 0, 0, 0, 1 };

void SetColor (RGBA color, GLfloat r, GLfloat g, GLfloat b)
{
  color[0] = Clamp(r, 0, 1);
  color[1] = Clamp(g, 0, 1);
  color[2] = Clamp(b, 0, 1);
  color[3] = 1;
}

void CopyColor (RGBA dest, RGBA source)
{
  int i;

  for (i = 0; i < 4; i++)
    dest[i] = source[i];
}


/****		Misc utilities		****/

void CheckGL ()
{
  int  err;
  char msg[256];

  err = glGetError();
  if (err != GL_NO_ERROR) {
    sprintf(msg, "OpenGL error: %s", (String)gluErrorString(err));
    Fail(msg);
  }
}

/****	Font utilities	****/

void BitmapString (void * font, float x, float y, String msg)
{
  int i;

  FailNull(font, "BitmapString: no font set");

  /* Use the WindowPos extension originally from Mesa so we
     always get window coords without having to set up a
     special ortho projection */
  //glWindowPos2f(x, y);
  glRasterPos2f(x,y);
  for (i = 0; (unsigned int)i < strlen(msg); i++)
    glutBitmapCharacter(font, msg[i]);
}


/****	Handy math	****/

float Min (float f, float limit)
{
  if (f > limit)
    return limit;
  else
    return f;
}

float Max (float f, float limit)
{
  if (f < limit)
    return limit;
  else
    return f;
}

float Clamp (float f, float min, float max)
{
  if (f < min)
    return min;
  else if (f > max)
    return max;
  else
    return f;
}


