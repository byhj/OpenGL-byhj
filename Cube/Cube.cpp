#include "common.h"

static const GLfloat VertexData[] = 
{
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
};

static const GLushort ElementData[] =
{
	0, 1, 2,
	2, 1, 3,
	2, 3, 4,
	4, 3, 5,

	4, 5, 6,
	6, 5, 7,
	6, 7, 0,
	0, 7, 1,

	6, 0, 2,
	2, 4, 6,
	7, 5, 3,
	7, 3, 1
};

class CubeApp: public byhj::Application
{
public:
	CubeApp():CubeShader("Cube Shader") {}
	~CubeApp() {}

	void init_shader();
	void init_buffer();
	void init_texture();
	void init_vertexArray();
	void init_windowInfo();
	void shutdown();

	void init();
	void render();
public:
    struct	Uniform 
	{
		GLuint mvp_loc;
	}uniform;
private:
	GLuint program, vao, ebo, vbo;
	Shader CubeShader;
};

GL_MAIN(CubeApp);

void CubeApp::init()
{
	glEnable(GL_DEPTH_TEST);
	init_shader();
	init_buffer();
	init_vertexArray();
	init_texture();
	init_windowInfo();
}

void CubeApp::init_shader()
{
	CubeShader.init();
	CubeShader.attach(GL_VERTEX_SHADER, "cube.vert");
	CubeShader.attach(GL_FRAGMENT_SHADER, "cube.frag");
	CubeShader.link();
	program = CubeShader.get_program();
	uniform.mvp_loc = glGetUniformLocation(program, "mvp");
}

void CubeApp::init_windowInfo()
{
	windowInfo.title = "Cube Window";
}

void CubeApp::render()
{
	static const GLfloat color[] = {0.3f, 0.3f, 0.3f, 1.0f };  //���ָ���Ļ���
	static const GLfloat one[] = {1.0f};
	glClearBufferfv(GL_DEPTH, 0, one);
	glClearBufferfv(GL_COLOR, 0, color);

	float time = glfwGetTime();

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
	
	//near and far data is accroding to the camera pos
	glm::mat4 proj = glm::perspective(45.0f, getAspect(), 0.1f, 1000.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f))
		            * glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f) );
	glm::mat4 mvp = proj * view * model;
	glUniformMatrix4fv(uniform.mvp_loc, 1, GL_FALSE, &mvp[0][0]);

	glUseProgram(program);
	glBindVertexArray(vao);
	//glDrawArrays(GL_LINES, 0, 8);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void CubeApp::shutdown()
{
	glDeleteProgram(program);
}

void CubeApp::init_buffer()
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

void CubeApp::init_texture()
{

}

void CubeApp::init_vertexArray()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
}