#ifndef __VGL_H__
#define __VGL_H__

// #define USE_GL3W

#ifdef USE_GL3W

#include <GL3/gl3.h>
#include <GL3/gl3w.h>

#else

#define GLEW_STATIC

#include <glew.h>

#ifdef _MSC_VER
#  ifdef _DEBUG
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "glew32s.lib") // the suffix "s" for static library
#    else
#      pragma comment (lib, "glew32s.lib")
#    endif
#  else // for release 
#    if (_MSC_VER >= 1600) 
#      pragma comment (lib, "glew32s.lib")
#    else
#      pragma comment (lib, "glew32s.lib")
#    endif
#  endif
#endif

#endif

//#define FREEGLUT_STATIC

#include <freeglut.h>

#ifdef _MSC_VER
#  ifdef _DEBUG
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "freeglut.lib")  // the shared(dynamic) library without suffix "s"
#    else
#      pragma comment (lib, "freeglut.lib")
#    endif
#  else  // for release 
#    if (_MSC_VER >= 1600)
#      pragma comment (lib, "freeglut.lib")
#    else
#      pragma comment (lib, "freeglut.lib")
#    endif
#  endif
#endif

#define BUFFER_OFFSET(x)  ((const void*) (x))

#endif /* __VGL_H__ */
