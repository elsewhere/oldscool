#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "console.hpp"
#include "event.hpp"
#include "image.hpp"
#include "lista.hpp"
#include "matikka.h"
#include "matrix.hpp"
#include "vector.hpp"
#include "texture.hpp"
#include "timer.hpp"
#include "mesh.hpp"
#include "font.hpp"
#include "stuff.hpp"
#include "bpm.hpp"
#include "beziercurve.hpp"
#include "hermitecurve.hpp"
#include "spline.hpp"
#include "vertexbuffer.hpp"
#include "primitives.hpp"

extern Console *console;
extern Timer *timer;
extern Lista *texturelist;
extern Lista *eventlist;
extern BPM *beat;
extern BPM *beat2;
extern BPM *beat4;

extern int XRES;
extern int YRES;


#endif