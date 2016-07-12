#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL\glew.h>

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

void shaderCompilerCheck(GLuint ID);
void shaderLinkCheck(GLuint ID);

#endif