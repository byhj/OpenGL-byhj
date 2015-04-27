#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <string>
#include <gl/glew.h>
#include "textFileRead.h"

class Shader 
{
public:
	Shader():name("Shader") {};
	Shader(const std::string n):name(n) {};
	~Shader();
public:
	void init();
	void attach(int type, const char *filename);
	void link();
	void use(void) {  glUseProgram(program); }
	void end(void) {  glUseProgram(0);  }

	GLuint get_program()  { return program; }

private:
	std::vector<GLuint> handles; 
	GLuint program;   
	std::string name;   
};


Shader::~Shader()
{
}

void Shader::init()
{
	std::cout << "Compile and Line Shader file" << std::endl;
}

//attach the shader file to opengl shader object

void Shader::attach(int type, const char* filename) 
{
	    //we get the shader file content
		char* mem = textFileRead(filename);

		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
		glCompileShader(handle);
		
		GLint compileSuccess=0;
		GLchar compilerSpew[256];
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
		if (!compileSuccess)
		{
			glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader %s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);
			while(1);;
		}

		handles.push_back(handle); //存储创建的shader handle以供连接所用
}

void Shader::link()
{
    program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) 
	{
		glAttachShader(program, handles[i]); //将前面创建的shader添加到program
	}
	glLinkProgram(program);  //连接shader program
	
	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) 
	{
			glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
			
			while(1);;
	}
	printf("%s Linked Successful\n", name.c_str());
	
}

#endif