#include <cstdio>
#include "gl_core_4_4.h"
bool LoadShaderType(char* filename, GLenum shaderType, unsigned int* output)
{
	bool succeded = true;
	FILE* ShaderFile = fopen(filename, "r");
	if (ShaderFile == 0)
	{
		succeded = false;
	}
	else
	{
		fseek(ShaderFile, 0, SEEK_END);
		int Shader = ftell(ShaderFile);
		fseek(ShaderFile, 0, SEEK_SET);
		char *shaderSource = new char[Shader];
		Shader = fread(shaderSource, 1, Shader, ShaderFile);
		unsigned int ShaderHandle = glCreateShader(shaderType);
		int success = GL_FALSE;
		glShaderSource(ShaderHandle, 1, &shaderSource, &Shader);
		glCompileShader(ShaderHandle);
		int logLenght = 0;
		glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetShaderiv(ShaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetShaderInfoLog(ShaderHandle, infoLogLength, 0, infoLog);
			printf("ERROR: STUFFED UP\n\n");
			printf("%s", infoLog);
			delete[] infoLog;
			succeded = false;
		}
		if (succeded)
		{
			*output = ShaderHandle;
		}
		delete[] shaderSource;
		fclose(ShaderFile);
	}
	return succeded;
}

bool LoadShaders(char* vertex_filename, char* geometry_filename, char* fragmant_filename, GLuint* result)
{
	//unsigned int result =0;
	bool succeded = true;

	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int geometryShader;
	*result = glCreateProgram();
	LoadShaderType(vertex_filename, GL_VERTEX_SHADER, &vertexShader);
	glAttachShader(*result, vertexShader);
	glDeleteShader(vertexShader);
	if (geometry_filename != nullptr)
	{
		LoadShaderType(geometry_filename, GL_GEOMETRY_SHADER, &geometryShader);
		glAttachShader(*result, geometryShader);
		glDeleteShader(geometryShader);
	}

	if (fragmant_filename != nullptr)
	{
		LoadShaderType(fragmant_filename, GL_FRAGMENT_SHADER, &fragmentShader);
		glAttachShader(*result, fragmentShader);
		glDeleteShader(fragmentShader);
	}
	glLinkProgram(*result);

	GLint Success;
	//error checking
	glGetProgramiv(*result, GL_LINK_STATUS, &Success);
	if (Success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(*result, infoLogLength, 0, infoLog);
		printf("ERROR: STUFFED UP\n\n");
		printf("%s", infoLog);
		delete[] infoLog;
		succeded = false;
	}
	return succeded;

}