#include <cstdio>
#include "gl_core_4_4.h"
bool LoadShaders(char* vertex_filename, char* fragmant_filename, GLuint* result)
{
	//unsigned int result =0;
	bool succeded = false;
	FILE* vertex_file = fopen(vertex_filename, "r");
	FILE* fragmant_file = fopen(fragmant_filename, "r");

	if (vertex_file == 0 || fragmant_file == 0)
	{
	}
	else
	{
		fseek(vertex_file, 0, SEEK_END);
		int vertex_file_length = ftell(vertex_file);
		fseek(vertex_file, 0, SEEK_SET);


		fseek(fragmant_file, 0, SEEK_END);
		int fragmant_file_length = ftell(fragmant_file);
		fseek(fragmant_file, 0, SEEK_SET);

		char *vs_source = new char[vertex_file_length];
		char *fs_source = new char[fragmant_file_length];

		vertex_file_length = fread(vs_source, 1, vertex_file_length, vertex_file);
		fragmant_file_length= fread(fs_source, 1, fragmant_file_length, fragmant_file);

		succeded = true;
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmant_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vs_source, &vertex_file_length);
		glCompileShader(vertex_shader);
		glShaderSource(fragmant_shader, 1, &fs_source, &fragmant_file_length);
		glCompileShader(fragmant_shader);

		*result = glCreateProgram();
		glAttachShader(*result, vertex_shader);
		glAttachShader(*result, fragmant_shader);
		glLinkProgram(*result);

		//error checking
		int success = GL_FALSE;
		glGetProgramiv(*result, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
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
		glDeleteShader(fragmant_shader);
		glDeleteShader(vertex_shader);
		delete[] vs_source;
		delete[] fs_source;
	}
	fclose(vertex_file);
	fclose(fragmant_file);
	return succeded;

}