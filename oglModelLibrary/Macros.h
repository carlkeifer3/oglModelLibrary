
#include <GL\glew.h>

#ifndef GLSL
#define GLSL(version, A) "#version " #version "\n" #A
#endif

#ifndef GENVERTEXARRAYS
#define GENVERTEXARRAYS(n,id)\
	if(GLEW_APPLE_vertex_array_object) glGenVertexArraysAPPLE(1, id);\
		else if (GLEW_ARB_vertex_array_object) glGenVertexArrays(n,id)
#endif

#ifndef BINDVERTEXARRAY
#define BINDVERTEXARRAY(id)\
	if(GLEW_APPLE_vertex_array_object) glBindVertexArrayAPPLE(id);\
		else if (GLEW_ARB_vertex_array_object) glBindVertexArray(id)
#endif