#include <common.h>
#include "TextureManager.h"

const static GLfloat VertexData[] = {
	// Positions        // Texture Coords
	-1.0, -1.0,  0.0,  0.0, 0.0,
	 1.0, -1.0,  0.0,  1.0, 0.0,
	 1.0,  1.0,  0.0,  1.0, 1.0,
	-1.0,  1.0,  0.0,  0.0, 1.0
};

const static GLuint ElementData[] = {  // Note that we start from 0!
	0, 1, 2, // First Triangle
	2, 3, 0  // Second Triangle
};  



class Texture2DApp: public byhj::Application
{
public:
	Texture2DApp():Texture2DShader("Texture2D Shader") {}
	~Texture2DApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();

	struct Uniform 
	{
		GLuint tex_loc;
	}uniform;

private:
	GLuint program, vao, ebo, vbo, texture;
	Shader Texture2DShader;
};

GL_MAIN(Texture2DApp);

void Texture2DApp::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void Texture2DApp::init_shader()
{
	Texture2DShader.init();
	Texture2DShader.attach(GL_VERTEX_SHADER, "texture2d.vert");
	Texture2DShader.attach(GL_FRAGMENT_SHADER, "texture2d.frag");
	Texture2DShader.link();
	program = Texture2DShader.get_program();
	uniform.tex_loc = glGetUniformLocation(program, "tex");
	glUniform1i(uniform.tex_loc, 0);
}

void Texture2DApp::init_windowInfo()
{
	windowInfo.title = "Texture2D Window";
}

void Texture2DApp::render()
{
	static const GLfloat color[] = {0.3f, 0.3f, 0.3f, 1.0f };  //清除指定的缓存
	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, 1);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void Texture2DApp::shutdown()
{
	glDeleteProgram(program);
}

void Texture2DApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ElementData), ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Texture2DApp::init_texture()
{
	/*
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height;
	unsigned char* image = SOIL_load_image("../media/texture/byhj.jpg", &width, &height, 0, SOIL_LOAD_RGB); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
	*/
	texture = 0;
	TextureManager::Inst()->LoadTexture("../media/texture/desert.tga", texture, GL_RGB, GL_RGB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture2DApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);         //notice, sizeof(GLfloat) usually is 4
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)) );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}