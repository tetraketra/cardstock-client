#ifndef SDLEXT_GL_H
#define SDLEXT_GL_H

#include "sdlext.h"
#include "window.h"
#include "toolkit/hashtable.h"

typedef unsigned int gl_vbo_t;
typedef unsigned int gl_tex_t;

typedef struct SDLEXT_OpenGL {
    gl_vbo_t vbo; /* Vertex Buffer Obejct */
    hashtable_t* shader_programs;
} SDLEXT_OpenGL;

#endif