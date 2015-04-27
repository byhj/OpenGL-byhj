#include "common.h"

class TestApp: public byhj::Application
{
public:
	TestApp():TestShader("Test Shader") {}
	~TestApp() {}
    
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
	Shader TestShader;
};

GL_MAIN(TestApp);

void TestApp::init()
{
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void TestApp::init_shader()
{
	TestShader.init();
	TestShader.attach(GL_VERTEX_SHADER, "test.vert");
	TestShader.attach(GL_FRAGMENT_SHADER, "test.frag");
	TestShader.link();
	program = TestShader.get_program();
}
void TestApp::init_windowInfo()
{
	windowInfo.title = "Test Window";
}

void TestApp::render()
{
	static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f };  //清除指定的缓存
	glClearBufferfv(GL_COLOR, 0, red);
	glUseProgram(program);
}

void TestApp::shutdown()
{
	glDeleteProgram(program);
}

void TestApp::init_buffer()
{
}

void TestApp::init_texture()
{
}

void TestApp::init_vertexArray()
{
}