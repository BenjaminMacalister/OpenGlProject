#ifndef UTILITY_H_
#define UTILITY_H_

typedef unsigned int GLuint;
bool LoadShaders(char* vertex_filename, char* geometry_filename, char* fragmant_filename, GLuint* result);
typedef unsigned int GLenum;
bool LoadShaderType(char* filename, GLenum shaderType, unsigned int* output);
#endif