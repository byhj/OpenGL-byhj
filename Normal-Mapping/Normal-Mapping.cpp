#include <common.h>
#include "TextureManager.h"
#include "tangentspace.hpp"
#include "vboindexer.hpp"
#include "objloader.hpp"

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector<glm::vec3> tangents;
std::vector<glm::vec3> bitangents;

std::vector<unsigned short> indices;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;
std::vector<glm::vec3> indexed_tangents;
std::vector<glm::vec3> indexed_bitangents;

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
		GLuint tex_loc, normal_loc;
		GLuint model_loc, view_loc, proj_loc;
	}uniform;

private:
	GLuint program, vao, ebo, vbo, texture;
	Shader Texture2DShader;
	GLuint normal_texture;
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
	uniform.tex_loc = glGetUniformLocation(program, "tex_color");
	glUniform1i(uniform.tex_loc, 0);
	uniform.normal_loc = glGetUniformLocation(program, "tex_normal");
	glUniform1i(uniform.normal_loc, 1);
	uniform.model_loc = glGetUniformLocation(program, "model");
	uniform.view_loc = glGetUniformLocation(program, "view");
	uniform.proj_loc = glGetUniformLocation(program, "proj");

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
	glBindVertexArray(vao);

	// Draw the triangles !
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_texture);
	float time = glfwGetTime();
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );

	//near and far data is accroding to the camera pos
	glm::mat4 proj = glm::perspective(45.0f, getAspect(), 0.1f, 1000.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f) );

	glUniformMatrix4fv(uniform.model_loc, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniform.view_loc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(uniform.proj_loc, 1, GL_FALSE, &proj[0][0]);

	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset
		);

}

void Texture2DApp::shutdown()
{
	glDeleteProgram(program);
}

void Texture2DApp::init_buffer()
{

}

void Texture2DApp::init_texture()
{
	texture = loadTexture("../media/texture/rock_color.tga");
	normal_texture = loadTexture("../media/texture/rock_normal.tga");
}

void Texture2DApp::init_vertexArray()
{
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("../media/object/cube.obj", vertices, uvs, normals);

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	computeTangentBasis(
		vertices, uvs, normals, // input
		tangents, bitangents    // output
		);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	indexVBO_TBN(
		vertices, uvs, normals, tangents, bitangents, 
		indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
		);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	GLuint tangentbuffer;
	glGenBuffers(1, &tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	GLuint bitangentbuffer;
	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
	
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 4th attribute buffer : tangents
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 5th attribute buffer : bitangents
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glVertexAttribPointer(
		4,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);



}