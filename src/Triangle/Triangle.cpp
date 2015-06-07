#include <common/common.h>

class TriangleApp: public byhj::Application
{
public:
	TriangleApp():TriangleShader("Triangle Shader") {}
	~TriangleApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();
private:
	GLuint program, vao, ibo, vbo;
	Shader TriangleShader;
};

GL_MAIN(TriangleApp);


const static GLfloat TriangleData[] = 
{
	//Position,           //Color
	-1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
};


void TriangleApp::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void TriangleApp::init_shader()
{
	TriangleShader.init();
	TriangleShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	TriangleShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	TriangleShader.link();
	program = TriangleShader.get_program();
}

void TriangleApp::init_windowInfo()
{
	windowInfo.title = "Triangle Window";
}

void TriangleApp::render()
{
	static const GLfloat color[] = {0.3f, 0.3f, 0.3f, 1.0f };  //清除指定的缓存
	glClearBufferfv(GL_COLOR, 0, color);
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleApp::shutdown()
{
	glDeleteProgram(program);
}

void TriangleApp::init_buffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleData), TriangleData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleApp::init_texture()
{

}

void TriangleApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);         //notice, sizeof(GLfloat) usually is 4
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), BUFFER_OFFSET(3 * sizeof(GLfloat)) );
	glBindVertexArray(0);
}